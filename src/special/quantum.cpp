
#include "quantum.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

#define QUANTUM_COST   10

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Quantum::Quantum():Special(SPECIAL_QUANTUM_PHYSICS)
{
   cost = QUANTUM_COST;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Quantum::~Quantum()
{
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Quantum::update()
{
   /* Apply the pair (if any) */
   apply();
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Quantum::pairNotMatched(Pair* p)
{
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Quantum::firstBlockSelected(Block* b)
{
   /* Apply it (doing the next pair) */
   apply();
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Quantum::pairMatched(Pair* p)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                                  apply                              *
 ***********************************************************************/
void Quantum::apply()
{
   Pair* p = curLevel->getSelectedPair();
   Block* pairBlock;

   if( (p) && (p->getBlock1()) && (!p->getBlock2()) )
   {
      /* Do the pair */
      pairBlock = p->getBlock1()->getPairBlock();
      if(!pairBlock->isMatched())
      {
         /* Done with special */
         active = false;
         curLevel->select(pairBlock->getSceneNode());
      }
   }
}


/***********************************************************************
 *                             pauseTimer                              *
 ***********************************************************************/
void Quantum::pauseTimer()
{
}

/***********************************************************************
 *                            resumeTimer                              *
 ***********************************************************************/
void Quantum::resumeTimer()
{
}

}

