#ifndef   DEADRAY_CAMERA_H
#define   DEADRAY_CAMERA_H

#include "Deadray/Core.h"
#include "Deadray/SceneNode.h"

namespace Deadray {
	class DEADRAY_API Camera : public SceneNode {

	private:
	public:
		Camera(Node* parent, void* extra = nullptr);
		~Camera();

		virtual NodeType GetNodeType() override {
			return Types::Camera;
		}

		virtual void OnTick(float dt) override;		
	};

}

#endif