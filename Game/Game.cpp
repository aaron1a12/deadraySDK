#include "stdafx.h"
#include "Entities/PlayerPuppet.h"
#include "Game.h"

using namespace Deadray;

Game::Game()
{
	Engine* eng = Engine::Get();
	eng->log("\nGame Manager initializing...!\n");
	
	Scene* myScene = new Scene();
	eng->SwitchScene( myScene );

	// Alternatively, you may create it dynamically
	// player = (PlayerPuppet*)myScene->CreateNodeByType(100);
	player = myScene->CreateChildNode<PlayerPuppet>();

	//player->SetPosition(Vector3(0,0,0));	
}

Game::~Game()
{
	Deadray::Engine::Get()->log("Shutting down game...");

	delete player;
}