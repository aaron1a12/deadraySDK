#include "Deadray.h"
#include "Deadray/SceneNode.h"

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
	PlayerPuppet(Node* parent);
	~PlayerPuppet();

	virtual void OnTick(float dt) override;

	virtual uint32 GetNodeType() override {return 100;}
};