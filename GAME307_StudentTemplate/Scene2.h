#ifndef Scene2_H
#define Scene2_H

#include <MMath.h>
#include <VMath.h>
#include "Scene.h"
#include "Character.h"
#include "Seek.h"
#include "Flee.h"
#include "VelocityMatch.h"
#include "Align.h"
#include "Graph.h"
#include "NodeRenderer.h"

using namespace MATH;
class Scene2 : public Scene {
private:
	SDL_Window* window;
	float xAxis = 25.0f;
	float yAxis = 15.0f;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Matrix4 inverseProjection;
	Graph graph;
	Path path;
	NodeRenderer noderend;
	Character* fygar1;
	Character* fygar2;
	Character* fygar3;
	Character* fygar4;
	Character* pooka;
	Seek seek;
	Flee flee;
	VelocityMatch velmatch;
	Align align;

public:
	Scene2(SDL_Window* sdlWindow, GameManager* game_);
	~Scene2();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
	Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
};

#endif

