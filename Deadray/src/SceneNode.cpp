#include "stdafx.h"
#include "Deadray/Core.h"
#include "Deadray/SceneNode.h"
#include "Deadray/Engine.h"

using namespace Deadray;

SceneNode::SceneNode(Node* parent) : Node(parent)
{
	GetEngine()->log("Scene node created");

	/*localPosition = Vector3(0.0, 0.0, 0.0);
	localRotation = Vector3(0.0, 0.0, 0.0);
	localScale = Vector3(1.0, 1.0, 1.0);*/

	worldMatrix = new D3DXMATRIXA16;
	UpdateMatrix();

	/*

	CreateAttachNode<Controller>();
	CreateAttachNode<InventoryManager>();

	Controller* input = new Controller(this);
	Controller* input = new Controller(this);*/
}

SceneNode::~SceneNode(void)
{
	delete worldMatrix;
}

Vector3 SceneNode::GetRotation()
{
	return localRotation;
}

void SceneNode::SetPosition(Vector3 position)
{
	localPosition = position;
	UpdateMatrix();


	/*D3DXMatrixTransformation(&worldMatrix, CONST D3DXVECTOR3 *pScalingCenter,
      CONST D3DXQUATERNION *pScalingRotation, CONST D3DXVECTOR3 *pScaling,
      CONST D3DXVECTOR3 *pRotationCenter, CONST D3DXQUATERNION *pRotation,
      CONST D3DXVECTOR3 *pTranslation);*/
}

void SceneNode::SetRotation(Vector3 rotation)
{
	localRotation = rotation;
	UpdateMatrix();

	//GetEngine()->log("SRotation");
}

void SceneNode::UpdateMatrix()
{

	//GetEngine()->log("Setting matrix translation: %f, %f, %f", localPosition.x, localPosition.y, localPosition.z);
	// Reset
	D3DXMatrixIdentity(worldMatrix);

	// Translate

	D3DXMATRIXA16 translation;
	D3DXMatrixTranslation(&translation, localPosition.x, localPosition.y, localPosition.z);

	// Rotate

	// Convert to radians
	float x = localRotation.x * PI_180;
	float y = localRotation.y * PI_180;
	float z = localRotation.z * PI_180;

	D3DXMATRIXA16 rotation;
	D3DXMatrixRotationYawPitchRoll(&rotation, y, x, z);

	//D3DXMatrixRotationQuaternion(&rotationMatrix, &cameraOrientationQuat);

	D3DXMatrixMultiply(worldMatrix, &rotation, &translation);
}

D3DXMATRIXA16 SceneNode::GetWorldMatrix()
{
	return *worldMatrix;
}