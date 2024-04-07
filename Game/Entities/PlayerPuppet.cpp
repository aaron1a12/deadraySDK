#include "stdafx.h"
#include "../Game.h"
#include "PlayerPuppet.h"
#include "Deadray/MeshNode.h"

using namespace Deadray;

PlayerPuppet::PlayerPuppet()
{
	Engine::Get()->log("Player spawned.");

	bIsTickable = true;

	mesh = CreateChildNode<MeshNode>();
	mesh->SetMesh(L"data\\warlock.obj");
	mesh->SetTextureFromFile(L"data\\warlock.jpg");
	mesh->SetPosition(Vector3(0.f, 0.f, 0.f));
	mesh->SetRotation(Vector3(0.f, 0.f, 0.f));

	rot = 0;

	ThreadSingleton::Get()->GetEngine();

	// Listen to input
	// Engine::Get()->EvtInput.AddListener(this);
}

PlayerPuppet::~PlayerPuppet()
{
	Engine::Get()->log("Player killed.");
}

void PlayerPuppet::OnTick(float dt)
{
	rot += 90.f * dt;
	mesh->SetRotation(Vector3(0.f, rot, 0.f));
}