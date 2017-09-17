
#include "../../src/game/memwar.h"

Goblin::BaseApp* createApplication()
{
   return new MemWar::MemWarMain();
}

