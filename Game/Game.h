#pragma once

#include "Main.h"
#include "Deadray.h"
#include "Deadray/GameManager.h"

#define GAME_CLASS 100173325

class PlayerPuppet;

class Game : public Deadray::GameManager {
public:
	PlayerPuppet* player;

public:
	Game();
	~Game();

	virtual uint32 GetNodeType() override {return GAME_CLASS;}
};

REGISTER_NODE_TYPE(GAME_CLASS, Game);