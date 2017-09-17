
#include "challenge.h"

#include "../game/mblock.h"
#include "../game/pair.h"
#include "../game/level.h"

#include "alien.h"
#include "bomb.h"
#include "chrono.h"
#include "compass.h"
#include "punk.h"
#include "thief.h"

namespace MemWar
{

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                  Challenge                            //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Challenge::Challenge(int t, Level* l)
{
   type=t;
   active=true;
   curLevel=l;
   icon = NULL;
   displayingInfo = false;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
Challenge::~Challenge()
{
   if(icon)
   {
      delete(icon);
   }
}

/***********************************************************************
 *                           setDisplayingInfo                         *
 ***********************************************************************/
void Challenge::setDisplayingInfo(bool displaying)
{
   displayingInfo = displaying;
}

/***********************************************************************
 *                                getIcon                              *
 ***********************************************************************/
ChallengeInfo* Challenge::getIcon()
{
   return icon;
}

/***********************************************************************
 *                                getType                              *
 ***********************************************************************/
int Challenge::getType()
{
   return type;
}

/***********************************************************************
 *                               isActive                              *
 ***********************************************************************/
bool Challenge::isActive()
{
   return active;
}

/***********************************************************************
 *                          setIconPosition                            *
 ***********************************************************************/
void Challenge::setIconPosition(int i)
{
   int mod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   icon->setTargetPosition((2 + ((i % 5) * 26 * mod)),
                           (10 + ( i / 5) * 26 * mod));
}

/***********************************************************************
 *                               setIcon                               *
 ***********************************************************************/
void Challenge::setIcon(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
            Ogre::String imageFileName, bool useTime)
{
   int mod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   if(icon)
   {
      delete(icon);
      icon = NULL;
   }
   icon = new MemWar::ChallengeInfo(ogreOverlay, x, y,
                                    imageFileName, useTime, this);
   icon->setDimensions(24 * mod, 24 * mod);
}

/***********************************************************************
 *                                inPair                               *
 ***********************************************************************/
bool Challenge::inPair(Block* b, Pair* pairs, int totalpairs)
{
   int i;
   for(i=0; i < totalpairs; i++)
   {
      if( (pairs[i].getBlock1() == b) ||
          (pairs[i].getBlock2() == b) )
      {
         return(true);
      }
   }

   return(false);
}

/***********************************************************************
 *                             repospairs                              *
 ***********************************************************************/
void Challenge::reposPairs(Pair* pairs, int totalpairs, bool unMatch)
{
   int i, p, r;
   Ogre::Vector2* positions;
   Ogre::Vector3 pos;
   bool* used;
   Block* b=NULL;
   int totalBlocks = totalpairs*2;

   if(totalpairs == 0)
   {
      /* No need to repositionate */
      return;
   }

   /* Get the positions */
   positions = new Ogre::Vector2[totalBlocks];
   used = new bool[totalBlocks];
   for(i=0; i<totalBlocks; i++)
   {
      p = i / 2;
      if((i % 2) == 0)
      {
         pos = pairs[p].getBlock1()->getPosition();
         if(unMatch)
         {
            curLevel->decPairsDone();
         }
      }
      else
      {
         pos = pairs[p].getBlock2()->getPosition();
      }
      positions[i][0] = pos[0];
      positions[i][1] = pos[2];
      used[i] = false;
   }

   /* Let's randomize them */
   for(i=0; i < totalBlocks; i++)
   {
      /* Get random position */
      r = (int)(totalBlocks * (rand() / (RAND_MAX + 1.0)));
      while(used[r])
      {
         r = (r+1)%(totalBlocks);
      }
      used[r] = true;

      /* Set the new Block position */
      p = i / 2;
      if((i % 2) == 0)
      {
         b = pairs[p].getBlock1();
      }
      else
      {
         b = pairs[p].getBlock2();
      }
      b->setTargetPosition(positions[r][0], BLOCK_POS_Y, positions[r][1]);
      if(unMatch)
      {
         b->unsetMatched();
         if(b->isFaceUp())
         {
            b->setFaceDownWithoutAnimation();
         }
      }
   }

   /* Done. Free the mallocs! */
   delete[] positions;
   delete[] used;
}


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                  Challenges                           //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Challenges::Challenges(Level* l)
{
   curLevel = l;
   guiOverlay = curLevel->getOverlay();
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Challenges::~Challenges()
{
   /* Destroy the list */
   clearList();
}

/***********************************************************************
 *                             isChallenge                             *
 ***********************************************************************/
bool Challenges::isChallenge(int type)
{
   return( (type == CHALLENGE_BOMB) || (type == CHALLENGE_THIEF) ||
           (type == CHALLENGE_PUNK) || (type == CHALLENGE_ALIEN) || 
           (type == CHALLENGE_LLAMA) || (type == CHALLENGE_ZOSIMA) ||
           (type == CHALLENGE_COMPASS) );
}

/***********************************************************************
 *                              pauseTimer                            *
 ***********************************************************************/
void Challenges::pauseTimer()
{
   int i;
   Challenge* chal = static_cast<Challenge*>(getFirst());

   for(i=0; i < getTotal(); i++)
   {
      if(chal->isActive())
      {
         chal->pauseTimer();
      }
      chal = static_cast<Challenge*>(chal->getNext());
   }
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Challenges::resumeTimer()
{
   int i;
   Challenge* chal = static_cast<Challenge*>(getFirst());

   for(i=0; i < getTotal(); i++)
   {
      if(chal->isActive())
      {
         chal->resumeTimer();
      }
      chal = static_cast<Challenge*>(chal->getNext());
   }
}


/***********************************************************************
 *                                 add                                 *
 ***********************************************************************/
Challenge* Challenges::add(int type)
{
   Challenge* chal = NULL;
   Ogre::String iconFile = "";
   bool useTime = false;

   /* Verify if the Challenge already is active */
   chal = find(type);
   if(chal != NULL)
   {
      /* No need to add! */
      return chal;
   }

   switch(type)
   {
      case CHALLENGE_BOMB:
      {
         chal = (Challenge*) new Bomb(curLevel);
         iconFile = ICON_CHALLENGE_BOMB;
         useTime = true;
      }
      break;
      case CHALLENGE_THIEF:
      {
         chal = (Challenge*) new Thief(curLevel);
         iconFile = ICON_CHALLENGE_THIEF;
         useTime = true;
      }
      break;
      case CHALLENGE_LLAMA:
      {
      }
      break;
      case CHALLENGE_ALIEN:
      {
         chal = (Challenge*) new Alien(curLevel);
         iconFile = ICON_CHALLENGE_ALIEN;
         useTime = false;
      }
      break;
      case CHALLENGE_ZOSIMA:
      {
         chal = (Challenge*)new Chrono(curLevel);
         iconFile = ICON_CHALLENGE_ZOSIMA;
         useTime = true;
      }
      break;
      case CHALLENGE_COMPASS:
      {
         chal = (Challenge*)new Compass(curLevel);
         iconFile = ICON_CHALLENGE_COMPASS;
         useTime = false;
      }
      break;
      case CHALLENGE_PUNK:
      {
         chal = (Challenge*) new Punk(curLevel);
         iconFile = ICON_CHALLENGE_PUNK;
         useTime = true;
      }
      break;
   }

   /* Insert it on list and set image */
   if(chal)
   {
      chal->setIcon(guiOverlay, getTotal(), 10, iconFile, useTime);
      chal->setIconPosition(getTotal());
      insert((Kobold::ListElement*)chal);
   }

   return chal;
}

/***********************************************************************
 *                            removeInactive                           *
 ***********************************************************************/
void Challenges::removeInactive()
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());
   Challenge* aux;

   for(i=0; i < getTotal(); i++)
   {
      if(!c->isActive())
      {
         if(c->getIcon()->getPosX() == -100)
         {
            /* At hide position, must remove from list */
            aux = c;
            c = static_cast<Challenge*>(c->getNext());
            remove(aux);
         } 
         else if(c->getIcon()->getTargetPosX() != -100)
         {
            /* Not at hide and not updating: must send to hide position */
            c->getIcon()->setTargetPosition(-100, -60);
            c = static_cast<Challenge*>(c->getNext());
         }
      }
      else
      {
         c = static_cast<Challenge*>(c->getNext());
      }
   }
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Challenges::update()
{
   int i;
   int iActive = 0;

   removeInactive();

   Challenge* c = static_cast<Challenge*>(getFirst());

   for(i=0; i < getTotal(); i++)
   {
      /* Only need to update if no animation is running,
       * otherwise, must wait the animation end. */
      if(curLevel->isAnimationRunning())
      {
         break;
      }

      if(c->isActive())
      {
         /* Update the Challenge */
         c->update();
         c->setIconPosition(getTotal() - iActive - 1);
         
         /*! Update icon */
         if (c->getIcon() != NULL) {
            c->getIcon()->update();
         }
         c = static_cast<Challenge*>(c->getNext());
         iActive++;
      } 
      else 
      {
         /* Just update its icon */
         if (c->getIcon() != NULL) {
            c->getIcon()->update();
         }
      }
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Challenges::firstBlockSelected(Block* b)
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());

   /* Tell all active Challenges! */
   for(i=0; i < getTotal(); i++)
   {
      if(c->isActive())
      {
         c->firstBlockSelected(b);
      }
      c = static_cast<Challenge*>(c->getNext());
   }
}

/***********************************************************************
 *                           PairNotMatched                            *
 ***********************************************************************/
void Challenges::pairNotMatched(Pair* p)
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());

   /* Tell all Challenges! */
   for(i=0; i < getTotal(); i++)
   {
      if(c->isActive())
      {
         c->pairNotMatched(p);
      }
      c = static_cast<Challenge*>(c->getNext());
   }
}

/***********************************************************************
 *                             PairMatched                             *
 ***********************************************************************/
void Challenges::pairMatched(Pair* p)
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());

   /* Tell all Challenges! */
   for(i=0; i < getTotal(); i++)
   {
      if(c->isActive())
      {
         c->pairMatched(p);
      }
      c = static_cast<Challenge*>(c->getNext());
   }
}

/***********************************************************************
 *                                find                                 *
 ***********************************************************************/
Challenge* Challenges::find(int type)
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());

   /* search all list */
   for(i=0; i < getTotal(); i++)
   {
      if((c->getType() == type) && (c->isActive()))
      {
         return c;
      }
      c = static_cast<Challenge*>(c->getNext());
   }

   return NULL;
}

/***********************************************************************
 *                              clear                                  *
 ***********************************************************************/
void Challenges::clear()
{
   int i;
   Challenge* c = static_cast<Challenge*>(getFirst());

   /* Unapply all challenges */
   for(i=0; i < getTotal(); i++)
   {
      c->unApply();
      c = static_cast<Challenge*>(c->getNext());
   }

   /* Clear the list */
   clearList();
}

}

