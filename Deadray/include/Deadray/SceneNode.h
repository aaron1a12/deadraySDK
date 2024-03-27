#ifndef   DEADRAY_ACTOR_H
#define   DEADRAY_ACTOR_H

#include "Deadray/Core.h"

namespace Deadray {

	class DEADRAY_API SceneNode : public Node {

	private:
		// Transform information

		Vector3 localPosition;
		Vector3 localRotation;
		Vector3 localScale;

		//D3DXQUATERNION localRotationQuat;

		D3DXMATRIXA16* worldMatrix;

	public:
		SceneNode(Node* parent);
		~SceneNode();

		virtual NodeType GetNodeType() override {
			return Types::SceneNode;
		}

	public:

		Vector3 GetRotation();
		void SetPosition(Vector3 position);
		void SetRotation(Vector3 rotation);
		void SetScale(Vector3 scale);
		//

		void UpdateMatrix();
		D3DXMATRIXA16 GetWorldMatrix();
	};

}

#endif