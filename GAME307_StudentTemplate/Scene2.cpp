#include "Scene2.h"
#include "Scene2.h"

Scene2::Scene2(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	// create a NPC
	fygar1 = nullptr;
	fygar2 = nullptr;
	fygar3 = nullptr;
	fygar4 = nullptr;
	pooka = nullptr;
}

Scene2::~Scene2() {}

bool Scene2::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to Dig-Dug

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("Dig-Dug.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	// Set up characters, choose good values for the constructor
	// or use the defaults, like this
	fygar1 = new Character();

	if (!fygar1->OnCreate(this) || !fygar1->setTextureWith("fygar.png") )
	{
		return false;
	}

	seek = Seek(fygar1->getBody(), game->getPlayer(), 2.5f);

	fygar2 = new Character();

	if (!fygar2->OnCreate(this) || !fygar2->setTextureWith("fygar.png"))
	{
		return false;
	}

	flee = Flee(fygar2->getBody(), game->getPlayer(), 1.5f);

	fygar3 = new Character();

	if (!fygar3->OnCreate(this) || !fygar3->setTextureWith("fygar.png"))
	{
		return false;
	}

	velmatch = VelocityMatch(fygar3->getBody(), game->getPlayer(), 2.0f);

	fygar4 = new Character();

	if (!fygar4->OnCreate(this) || !fygar4->setTextureWith("fygar.png"))
	{
		return false;
	}

	fygar4->getBody()->setRotation(5.5f);
	align = Align(fygar4->getBody(), game->getPlayer(), 1.0f, 1.0f, 1.5f, 2.0f, 0.4f);

	pooka = new Character();

	if (!pooka->OnCreate(this) || !pooka->setTextureWith("pooka.png"))
	{
		return false;
	}
	pooka->getBody()->setmaxspeed(2);
	// end of character set ups

	noderend = NodeRenderer(renderer, projectionMatrix);
	noderend.CreateNodes(graph, 25, 15);

	return true;
}

void Scene2::OnDestroy() 
{
	if (fygar1)
	{
		fygar1->OnDestroy();
		delete fygar1;
	}
	if (fygar2)
	{
		fygar2->OnDestroy();
		delete fygar2;
	}
	if (fygar3)
	{
		fygar3->OnDestroy();
		delete fygar3;
	}
	if (fygar4)
	{
		fygar4->OnDestroy();
		delete fygar4;
	}
	if (pooka)
	{
		pooka->OnDestroy();
		delete pooka;
	}
}

void Scene2::Update(const float deltaTime) {

	if (path.currentNode >= path.nodes.size()) {
		auto pookapos = pooka->getBody()->getPos();
		std::cout << "Goal reached: \n";
		auto playerpos = game->getPlayer()->getPos();
		path = findpath(graph, graph.getNearestNode({pookapos.x, pookapos.y}), graph.getNearestNode({playerpos.x, playerpos.y}));
	}

	// Adjust for current velocity to create the final steering output
	// Calculate and apply any steering for npc's
	auto steer = seek.getSteering();
	fygar1->Update(deltaTime, steer);

	steer = flee.getSteering();
	fygar2->Update(deltaTime, steer);

	steer = velmatch.getSteering();
	fygar3->Update(deltaTime, steer);

	steer = align.getSteering();
	fygar4->Update(deltaTime, steer);

	steer = path.getsteering(game->getPlayer());
	pooka->Update(deltaTime, steer);

	// Update player
	game->getPlayer()->Update(deltaTime);
}

void Scene2::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	noderend.render(graph);
	// render any npc's
	fygar1->render(0.2f);
	fygar2->render(0.2f);
	fygar3->render(0.2f);
	fygar4->render(0.2f);
	pooka->render(0.2f);
	// render the player
	game->RenderPlayer(0.25f);

	SDL_RenderPresent(renderer);
}

void Scene2::HandleEvents(const SDL_Event& event)
{
	// send events to npc's as needed

	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
