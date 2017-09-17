
#include "ocarina.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

#define OCARINA_COST   20

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Ocarina::Ocarina():Special(SPECIAL_OCARINA)
{
   cost = OCARINA_COST;
   paused = false;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Ocarina::~Ocarina()
{
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Ocarina::update()
{
   if(!paused)
   {
      /* Stop all challenge timers */
      curLevel->pauseChallengeTimers();
      stopTimer.reset();
      paused = true;
   }
   else if(stopTimer.getMilliseconds() > OCARINA_STOP_TIME)
   {
      /* Resume all challenge timers */
      curLevel->resumeChallengeTimers();
      paused = false;
      /* And done with the special */
      active = false;
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Ocarina::pairNotMatched(Pair* p)
{
   /* Nothing to do. */
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Ocarina::firstBlockSelected(Block* b)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Ocarina::pairMatched(Pair* p)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                             pauseTimer                              *
 ***********************************************************************/
void Ocarina::pauseTimer()
{
   stopTimer.pause();
}

/***********************************************************************
 *                            resumeTimer                              *
 ***********************************************************************/
void Ocarina::resumeTimer()
{
   stopTimer.resume();
}

}

