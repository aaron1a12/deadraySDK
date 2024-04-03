#include "stdafx.h"
#include "Game.h"

Game::Game(Engine* engine) : Node(engine) 
{
	Scene* myScene = new Scene(engine);
	engine->SwitchScene( myScene );

	// Alternatively, you may 
	player = (PlayerPuppet*)myScene->CreateNodeByType(100);
	//player = myScene->CreateChildNode<PlayerPuppet>();

	player->SetPosition(Vector3(0,0,0));

	
}

Game::~Game()
{
	GetEngine()->log("Shutting down game...");

	delete player;
}