#pragma once

#include "Deadray.h"
#include "Deadray/SceneNode.h"

#define PLAYER_PUPPET_CLASS 626299013

// Forward declarations
namespace Deadray {
	class MeshNode;
}

using namespace Deadray;

class PlayerPuppet : public Deadray::SceneNode {
private:
	// Player mesh
	MeshNode* mesh;

	float rot;

public:
	PlayerPuppet();
	~PlayerPuppet();

	virtual void OnTick(float dt) override;

	virtual uint32 GetNodeType() override {return PLAYER_PUPPET_CLASS;}
};

REGISTER_NODE_TYPE(PLAYER_PUPPET_CLASS, PlayerPuppet);