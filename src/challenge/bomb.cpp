
#include "bomb.h"
#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"
#include <kosound/sound.h>
#include <kobold/ogre3d/ogrefilereader.h>

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Bomb::Bomb(Level* l):Challenge(CHALLENGE_BOMB, l)
{
   remaining = BOMB_MAX_TIME / 1000;
   bombTimer.reset();
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Bomb::~Bomb()
{
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Bomb::getRemainingTime() 
{
   return remaining;
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Bomb::update()
{
   long time = bombTimer.getMilliseconds();
   remaining = ((BOMB_MAX_TIME - time) / 1000) + 1;

   /* Verify timer to explode! */
   if(time > BOMB_MAX_TIME)
   {
      /* Time expired, must apply the Bomb to the level */
      apply();
      active = false;
      remaining = 0;
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Bomb::pairNotMatched(Pair* p)
{
   /* Nothing to do, as expecting the Bomb pair */
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Bomb::firstBlockSelected(Block* b)
{
   /* Nothing to do either. */
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Bomb::pairMatched(Pair* p)
{
   /* Verify if we got the Bomb pair */
   if(p->getBlock1()->getImageId() == CHALLENGE_BOMB)
   {
      /* Got it! Must disable the timer */
      active = false;
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "bomb_off.ogg",
            new Kobold::OgreFileReader());
   }
}

/***********************************************************************
 *                                apply                                *
 ***********************************************************************/
void Bomb::apply()
{
   Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "bomb.ogg", 
         new Kobold::OgreFileReader());

   curLevel->shuffle();
}

/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void Bomb::unApply()
{
}

/***********************************************************************
 *                              pauseTimer                             *
 ***********************************************************************/
void Bomb::pauseTimer()
{
   bombTimer.pause();
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Bomb::resumeTimer()
{
   bombTimer.resume();
}

}

