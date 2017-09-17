
#include "wind.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

#define WIND_COST   12

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Wind::Wind():Special(SPECIAL_WIND)
{
   cost = WIND_COST;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Wind::~Wind()
{
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Wind::update()
{
   /* Remove all challenges active from level */
   curLevel->clearChallenges();
   active = false;
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Wind::pairNotMatched(Pair* p)
{
   /* Nothing to do. */
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Wind::firstBlockSelected(Block* b)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Wind::pairMatched(Pair* p)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                             pauseTimer                              *
 ***********************************************************************/
void Wind::pauseTimer()
{
}

/***********************************************************************
 *                            resumeTimer                              *
 ***********************************************************************/
void Wind::resumeTimer()
{
}

}

