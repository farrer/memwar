
#include "thief.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

#include <kosound/sound.h>

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Thief::Thief(Level* l):Challenge(CHALLENGE_THIEF, l)
{
   tTimer.reset();
   remaining = THIEF_MAX_TIME;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Thief::~Thief()
{
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Thief::getRemainingTime() 
{
   return remaining;
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Thief::update()
{
   long time = tTimer.getMilliseconds();
   remaining = ((THIEF_MAX_TIME - time) / 1000) + 1;

   /* Verify timer to do the thief pair */
   if(time > THIEF_MAX_TIME)
   {
      /* Time expired, must apply the thief to the level */
      apply();
      active = false;
      remaining = 0;
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Thief::pairNotMatched(Pair* p)
{
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Thief::firstBlockSelected(Block* b)
{
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Thief::pairMatched(Pair* p)
{
   /* Verify if we got the thief pair */
   if(p->getBlock1()->getImageId() == CHALLENGE_THIEF)
   {
      /* Got it! Must end the challenge */
      active = false;
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "police.ogg");
   }
}

/***********************************************************************
 *                                apply                                *
 ***********************************************************************/
void Thief::apply()
{
   MemWar::Block* b = (MemWar::Block*)curLevel->getFirst();
   int i, totalBlocks = curLevel->getTotal();

   Pair* curPairs = new Pair[THIEF_NUM_PAIRS];
   int numPairs = 0;

   /* Get all needed pairs (if any, and if can) */
   for(i=0; ((i < totalBlocks) && (numPairs < THIEF_NUM_PAIRS)); i++)
   {
      if( (b->isMatched()) && (!inPair(b, curPairs, numPairs)) )
      {
         /* Got an matched */
         curPairs[numPairs].setBlock1(b);
         curPairs[numPairs].setBlock2(b->getPairBlock());
         numPairs++;
      }

      b = (MemWar::Block*)b->getNext();
   }

   /* Let's unMatch and repositionate all pairs */
   reposPairs(curPairs, numPairs, true);

   Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "thief.ogg");

   
   delete[] curPairs;
}

/***********************************************************************
 *                              pauseTimer                             *
 ***********************************************************************/
void Thief::pauseTimer()
{
   tTimer.pause();
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Thief::resumeTimer()
{
   tTimer.resume();
}

}

