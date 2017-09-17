
#include "compass.h"


#include <goblin/camera.h>

#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Compass::Compass(Level* l):Challenge(CHALLENGE_COMPASS, l)
{
   failed = false;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Compass::~Compass()
{
   unApply();
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Compass::getRemainingTime() 
{
   return -1;
}



/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Compass::update()
{
   /* Verify camera */
   if(failed)
   {
       /* FIXME: only necessary if camera is free.
        * Since we'll let it fixed, no need to verify. */      
   }
}

/***********************************************************************
 *                                unApply                              *
 ***********************************************************************/
void Compass::unApply()
{
   if(failed)
   {
      /* set back the camera */
      Goblin::Camera::setTarget(Goblin::Camera::getCenterX(), 
            Goblin::Camera::getCenterY(), Goblin::Camera::getCenterZ(),
            0.0f, Goblin::Camera::getTheta(), Goblin::Camera::getZoom(),
            0.0f, 10.0f, 0.0f);
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Compass::pairNotMatched(Pair* p)
{
   if(!failed)
   {
      /* Failed to do compass pair. */
      failed = true;
      /* Apply camera rotation! */
      Goblin::Camera::setTarget(Goblin::Camera::getCenterX(), 
            Goblin::Camera::getCenterY(), Goblin::Camera::getCenterZ(),
            180.0f, Goblin::Camera::getTheta(), Goblin::Camera::getZoom(),
            0.0f, 10.0f, 0.0f);
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Compass::firstBlockSelected(Block* b)
{
   /* If not failed and here, means the ostrich attacks...
    * so you've failed to find the compass */
   if(!failed)
   {
      Goblin::Camera::setPhi(180);
      failed = true;
   }
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Compass::pairMatched(Pair* p)
{
   /* Verify if we got the compass pair */
   if(p->getBlock1()->getImageId() == CHALLENGE_COMPASS)
   {
      /* Got it! Must disable the timer */
      active = false;
      /* And set back the camera! */
      Goblin::Camera::setPhi(0);
   }
}

}

