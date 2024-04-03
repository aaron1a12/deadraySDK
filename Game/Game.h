#include "Main.h"
#include "Deadray.h"
#include "Entities/PlayerPuppet.h"

using namespace Deadray;

class Game : public Node {
public:
	PlayerPuppet* player;

	Game(Engine* engine);
	~Game();
};