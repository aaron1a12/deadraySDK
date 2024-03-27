#include "Main.h"
#include "Deadray.h"

using namespace Deadray;

class Game : public Node {
public:
	Game(Engine* engine) : Node(engine) 
	{
		Scene* myScene = new Scene(engine);
		engine->SwitchScene( myScene );

	}

	~Game()
	{
		GetEngine()->log("Shutting down game...");
		GetEngine()->log("Shutting down game...");
	}
};