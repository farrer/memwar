
#include "punk.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Punk::Punk(Level* l):Challenge(CHALLENGE_PUNK, l)
{
   osTimer.reset();
   remaining = PUNK_MATCH_TIME / 1000;
   curMax = PUNK_MATCH_TIME;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Punk::~Punk()
{
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Punk::getRemainingTime() 
{
   return remaining;
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Punk::update()
{
   long time = osTimer.getMilliseconds();
   remaining = ((curMax - time) / 1000) + 1;

   /* Verify timer, to shuffle level */
   if(time > curMax)
   {
      curLevel->shuffle();
      osTimer.reset();
      remaining = PUNK_MAX_TIME / 1000;
      curMax = PUNK_MAX_TIME;
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Punk::firstBlockSelected(Block* b)
{
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Punk::pairMatched(Pair* p)
{
   /* Verify if we got the punk pair */
   if(p->getBlock1()->getImageId() == CHALLENGE_PUNK)
   {
      /* Got it! Must disable the challenge */
      active = false;
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Punk::pairNotMatched(Pair* p)
{
}

/***********************************************************************
 *                              pauseTimer                             *
 ***********************************************************************/
void Punk::pauseTimer()
{
   osTimer.pause();
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Punk::resumeTimer()
{
   osTimer.resume();
}

}

