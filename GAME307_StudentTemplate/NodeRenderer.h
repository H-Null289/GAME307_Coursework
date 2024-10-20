#pragma once
#include <SDL.h>
#include "Matrix.h"
#include "Graph.h"
#include <array>

using namespace MATH;

class NodeRenderer
{
private:
    SDL_Renderer* renderer;         // SDL renderer
    Matrix4 projectionMatrix;       // Projection matrix to convert game coordinates to screen coordinates
    static const int gridSize = 8;
    std::array<std::array<Graph::Node*, gridSize>, gridSize> nodes;

public:
    NodeRenderer() = default;
    NodeRenderer(SDL_Renderer* renderer, Matrix4 projectionMatrix)
        : renderer(renderer), projectionMatrix(projectionMatrix)
    {}

    void CreateNodes(Graph& graph, float screenWidth, float screenHeight) {
        // Calculate the spacing between nodes based on the screen size
        float xSpacing = screenWidth / (gridSize - 1);
        float ySpacing = screenHeight / (gridSize - 1);

        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                // Calculate the position of the node
                float x = i * xSpacing;
                float y = j * ySpacing;

                // Add the node to the graph
                Graph::Node& node = graph.addNode({ x, y });
                nodes[i][j] = &node;
            }
        }

        // Add connections to all adjacent nodes, including diagonals
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                // Check each of the eight possible directions
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        // Skip the current node (di == 0 && dj == 0)
                        if (di == 0 && dj == 0) {
                            continue;
                        }

                        // Calculate neighbor coordinates
                        int ni = i + di;
                        int nj = j + dj;

                        // Check if the neighbor is within bounds
                        if (ni >= 0 && ni < gridSize && nj >= 0 && nj < gridSize) {
                            // Add the connection between the current node and the neighbor
                            graph.addConnect(*nodes[i][j], *nodes[ni][nj]);
                        }
                    }
                }
            }
        }
    }

    void render(Graph& graph, float nodeSize = 10.0f, float scale = 1.0f,
        SDL_Color nodeColor = { 255, 0, 0, 255 },
        SDL_Color lineColor = { 0, 255, 0, 255 })
    {
        {
            // Render connections first
            SDL_SetRenderDrawColor(renderer, lineColor.r, lineColor.g, lineColor.b, lineColor.a);

            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    Graph::Node* node = nodes[i][j];
                    Vec3 screenCoords = projectionMatrix * MATH::Vec3{ node->pos.x,node->pos.y,0 };

                    // Get the list of connections for the current node from the graph
                    const auto& connections = graph.getConnections(*node);

                    // Draw lines to each connected node
                    for (const auto& connection : connections) {
                        Graph::Node* connectedNode = (connection->node1 == node) ? connection->node2 : connection->node1;
                        Vec3 connectedScreenCoords = projectionMatrix * MATH::Vec3{ connectedNode->pos.x,connectedNode->pos.y,0 };

                        SDL_RenderDrawLine(renderer,
                            static_cast<int>(screenCoords.x),
                            static_cast<int>(screenCoords.y),
                            static_cast<int>(connectedScreenCoords.x),
                            static_cast<int>(connectedScreenCoords.y));
                    }
                }
            }

            // Render the nodes as filled rectangles
            SDL_SetRenderDrawColor(renderer, nodeColor.r, nodeColor.g, nodeColor.b, nodeColor.a);

            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    Graph::Node* node = nodes[i][j];
                    Vec3 screenCoords = projectionMatrix * MATH::Vec3{ node->pos.x,node->pos.y,0 };;

                    // Calculate the scaled dimensions
                    float scaledSize = nodeSize * scale;
                    SDL_Rect square;
                    square.x = static_cast<int>(screenCoords.x - 0.5f * scaledSize);
                    square.y = static_cast<int>(screenCoords.y - 0.5f * scaledSize);
                    square.w = static_cast<int>(scaledSize);
                    square.h = static_cast<int>(scaledSize);

                    // Render the filled rectangle for the node
                    SDL_RenderFillRect(renderer, &square);
                }
            }
        }
    };
};