
#include "chrono.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Chrono::Chrono(Level* l):Challenge(CHALLENGE_ZOSIMA, l)
{
   curPairs = new Pair[CHRONO_NUM_PAIRS];
   numPairs = 0;
   failPair = false;
   remaining = CHRONO_MAX_TIME / 1000;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Chrono::~Chrono()
{
   delete[] curPairs;
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Chrono::getRemainingTime() 
{
   return remaining;
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Chrono::update()
{
   if(failPair)
   {
      long time = chronoTimer.getMilliseconds();
      remaining = ((CHRONO_MAX_TIME - time) / 1000) + 1;

      /* Verify timer to do the pairs */
      if( (time > CHRONO_MAX_TIME) && (numPairs < CHRONO_NUM_PAIRS) )
      {
         /* Time expired, and couldn't must apply the chrono to the level */
         apply();
         active = false;
         remaining = 0;
      }
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Chrono::pairNotMatched(Pair* p)
{
   if(!failPair)
   {
      /* User failed, let's start the timer! */
      chronoTimer.reset();
      failPair = true;
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Chrono::firstBlockSelected(Block* b)
{
   if(!failPair)
   {
      failPair = true;
   }
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Chrono::pairMatched(Pair* p)
{
   if(!failPair)
   {
      /* Verify if we got the chrono pair */
      if(p->getBlock1()->getImageId() == CHALLENGE_ZOSIMA)
      {
         /* Got it! Must disable the timer */
         active = false;
      }
   }
   else
   {
      /* Let's count pairs */
      numPairs++;

      /* Verify number of pairs */
      if(numPairs >= CHRONO_NUM_PAIRS)
      {
         /* Done! */
         active = false;
      }
   }
}

/***********************************************************************
 *                                apply                                *
 ***********************************************************************/
void Chrono::apply()
{
   MemWar::Block* b = (MemWar::Block*)curLevel->getFirst();
   int i, totalBlocks = curLevel->getTotal();

   /* Must rest pair counter, as pairs should changed by another
    * challenge effect between the time it was made and now. */
   numPairs = 0;

   /* Get pairs to unmatch */
   for(i=0; ((i < totalBlocks) && (numPairs < CHRONO_NUM_PAIRS)); i++)
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
}

/***********************************************************************
 *                              pauseTimer                             *
 ***********************************************************************/
void Chrono::pauseTimer()
{
   chronoTimer.pause();
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Chrono::resumeTimer()
{
   chronoTimer.resume();
}

}

