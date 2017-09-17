
#include "special.h"

#include "../game/mblock.h"
#include "../game/pair.h"
#include "../game/level.h"
#include "../game/score.h"

#include "ocarina.h"
#include "quantum.h"
#include "wind.h"

namespace MemWar
{

#define ICON_SPECIALS_CANCEL   "initial/exit.png"

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                  Special                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Special::Special(int t)
{
   type = t;
   active = false;
   curLevel = NULL;
   cost = 1;
   icon = NULL;
   iconBw = NULL;
   card = NULL;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
Special::~Special()
{
   if(icon)
   {
      delete(icon);
   }
   if(iconBw)
   {
      delete(iconBw);
   }
   if(card)
   {
      delete(card);
   }
}

/***********************************************************************
 *                              setLevel                               *
 ***********************************************************************/
void Special::setLevel(MemWar::Level* l)
{
   curLevel = l;
}

/***********************************************************************
 *                             getCardImage                            *
 ***********************************************************************/
Goblin::Ibutton* Special::getCardImage()
{
   return(card);
}

/***********************************************************************
 *                                getType                              *
 ***********************************************************************/
int Special::getType()
{
   return(type);
}

/***********************************************************************
 *                                getCost                              *
 ***********************************************************************/
int Special::getCost()
{
   return(cost);
}

/***********************************************************************
 *                             setAvailable                            *
 ***********************************************************************/
void Special::setAvailable(int totalPoints)
{
   if( (totalPoints >= cost) && (!active) )
   {
      icon->show();
      iconBw->hide();
   }
   else
   {
      icon->hide();
      iconBw->show();
   }
}

/***********************************************************************
 *                               isActive                              *
 ***********************************************************************/
bool Special::isActive()
{
   return(active);
}

/***********************************************************************
 *                               activate                              *
 ***********************************************************************/
void Special::activate()
{
   Score::usedSpecial();
   active = true;
}

/***********************************************************************
 *                          setIconPosition                            *
 ***********************************************************************/
void Special::setIconPosition(int x, int y)
{
   icon->setPosition(x, y);
   icon->setDimensions(24, 24);
   iconBw->setPosition(x, y);
   iconBw->setDimensions(24, 24);
}

/***********************************************************************
 *                               getIcon                               *
 ***********************************************************************/
Goblin::Ibutton* Special::getIcon()
{
   return(icon);
}

/***********************************************************************
 *                              getIconBw                              *
 ***********************************************************************/
Goblin::Ibutton* Special::getIconBw()
{
   return(iconBw);
}

/***********************************************************************
 *                               setIcon                               *
 ***********************************************************************/
void Special::setIcon(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
            Ogre::String imageFileName, Ogre::String imageBWFileName)
{
   if(icon)
   {
      delete(icon);
      icon = NULL;
   }
   icon = new Goblin::Ibutton(ogreOverlay, imageFileName, "gui");
   icon->setPosition(x, y);
   icon->setDimensions(24, 24);
   icon->hide();

   if(iconBw)
   {
      delete(iconBw);
   }
   iconBw = new Goblin::Ibutton(ogreOverlay, imageBWFileName, "gui");
   iconBw->setDimensions(24, 24);
   iconBw->setPosition(x, y);
   iconBw->show();
}


/***********************************************************************
 *                               setCard                               *
 ***********************************************************************/
void Special::setCard(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
            Ogre::String imageFileName)
{
   if(card)
   {
      delete(card);
   }
   card = new Goblin::Ibutton(ogreOverlay, imageFileName, "gui");
   card->setDimensions(256, 256);
   card->setMouseOverDimensions(260, 260);
   card->setPosition(x, y);
   card->hide();
}

/***********************************************************************
 *                                inPair                               *
 ***********************************************************************/
bool Special::inPair(Block* b, Pair* pairs, int totalPairs)
{
   int i;
   for(i=0; i < totalPairs; i++)
   {
      if( (pairs[i].getBlock1() == b) ||
          (pairs[i].getBlock2() == b) )
      {
         return(true);
      }
   }

   return(false);
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                 Specials                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
void Specials::init()
{
   curLevel = NULL;
   guiOverlay = Ogre::OverlayManager::getSingletonPtr()->create("Specials");
   guiOverlay->setZOrder(648);
   guiOverlay->show();

   curPoints = 0;

   list = new Kobold::List();

   cancel = new Goblin::Ibutton(guiOverlay, ICON_SPECIALS_CANCEL, "gui", 
                                "Cancel", "infoFontOut", 16);
   cancel->setPosition(10, 310); 
   cancel->setDimensions(64, 64);
   cancel->setMouseOverDimensions(65, 65);
   cancel->hide();

   /* Add all Specials to the list
    * FIXME: only add the available ones!! */
   int i;
   for(i=0; i < TOTAL_SPECIAL_TYPES; i++)
   {
      add(i+SPECIAL_OVERHEAD);
   }
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
void Specials::finish()
{
   /* Destroy the list */
   delete(list);

   /* And the gui overlay */
   if(cancel)
   {
      delete(cancel);
   }
   Ogre::OverlayManager::getSingletonPtr()->destroy(guiOverlay);
}

/***********************************************************************
 *                              addPoints                              *
 ***********************************************************************/
void Specials::addPoints(int p)
{
   curPoints += p;
}

/***********************************************************************
 *                              getPoints                              *
 ***********************************************************************/
int Specials::getPoints()
{
   return(curPoints);
}

/***********************************************************************
 *                              getTotal                               *
 ***********************************************************************/
int Specials::getTotal()
{
   if(list)
   {
      return(list->getTotal());
   }
   return(0);
}

/***********************************************************************
 *                            getCancelImage                           *
 ***********************************************************************/
Goblin::Ibutton* Specials::getCancelImage()
{
   return(cancel);
}

/***********************************************************************
 *                              getFirst                               *
 ***********************************************************************/
Special* Specials::getFirst()
{
   if(list)
   {
      return((Special*)list->getFirst());
   }
   return(NULL);
}

/***********************************************************************
 *                              setLevel                               *
 ***********************************************************************/
void Specials::setLevel(MemWar::Level* l)
{
   int i;
   int total = list->getTotal();
   Special* sp = (Special*)list->getFirst();
   for(i=0; i < total; i++)
   {
      sp->setLevel(l);
      sp = (Special*)sp->getNext();
   }
   curLevel = l;
}

/***********************************************************************
 *                                 add                                 *
 ***********************************************************************/
void Specials::add(int type)
{
   Special* sp = NULL;
   Ogre::String iconFile = "";
   Ogre::String iconBWFile = "";
   Ogre::String cardFile = "";

   /* Verify if the Special already is added */
   if(find(type))
   {
      /* No need to add! */
      return;
   }

   switch(type)
   {
      case SPECIAL_WIND:
      {
         sp = (Special*)new Wind();
         iconFile = "specials/wind.png";
         iconBWFile = "specials/wind_bw.png";
         cardFile = "specials/cards/wind.png";
      }
      break;
      case SPECIAL_OCARINA:
      {
         sp = (Special*) new Ocarina();
         iconFile = "specials/ocarina.png";
         iconBWFile = "specials/ocarina_bw.png";
         cardFile = "specials/cards/ocarina.png";
      }
      break;
      case SPECIAL_QUANTUM_PHYSICS:
      {
         sp = (Special*)new Quantum();
         iconFile = "specials/quantum.png";
         iconBWFile = "specials/quantum_bw.png";
         cardFile = "specials/cards/quantum.png";
      }
      break;
      case SPECIAL_ELEPHANT:
      break;
      case SPECIAL_CRYSTAL_BALL:
      break;
   }

   /* Insert it on list and set image */
   if(sp)
   {
      sp->setLevel(curLevel);
      sp->setIcon(guiOverlay, 290 - (getTotal() * 26), 10,
                  iconFile, iconBWFile);
      sp->setCard(guiOverlay, 31, 50, cardFile);
      list->insert((Kobold::ListElement*)sp);
   }
}

/***********************************************************************
 *                              resetIcons                             *
 ***********************************************************************/
void Specials::resetIcons()
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();

   for(i=0; i < total; i++)
   {
      s->setIconPosition(290-((total-i-1)*26), 10);
      s->setAvailable(curPoints);
      
      /* next */
      s = (Special*)s->getNext();
   }  
}

/***********************************************************************
 *                            removeInactive                           *
 ***********************************************************************/
void Specials::removeInactive()
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();
   Special* aux;

   for(i=0; i < total; i++)
   {
      if(!s->isActive())
      {
         aux = s;
         s = (Special*)s->getNext();
         list->remove(aux);
      }
      else
      {
         s = (Special*)s->getNext();
      }
   }
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Specials::update()
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();

   for(i=0; i < total; i++)
   {
      /* Only need to update if no animation is running,
       * otherwise, must wait the animation end. */
      if(curLevel->isAnimationRunning())
      {
         break;
      }

      /* Update available */
      s->setAvailable(curPoints);

      if(s->isActive())
      {
         /* Update the Special */
         s->update();
      }
      s = (Special*)s->getNext();
   }
}

/***********************************************************************
 *                               activate                              *
 ***********************************************************************/
void Specials::activate(int type)
{
   Special* sp = find(type);
   if(sp)
   {
      if(sp->getCost() <= curPoints)
      {
         curPoints -= sp->getCost();
         sp->activate();
      }
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Specials::firstBlockSelected(Block* b)
{
   int i;
   int total = list->getTotal();;
   Special* s = (Special*)list->getFirst();

   /* Tell all Specials! */
   for(i=0; i < total; i++)
   {
      s->firstBlockSelected(b);
      s = (Special*)s->getNext();
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Specials::pairNotMatched(Pair* p)
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();

   /* Tell all Specials! */
   for(i=0; i < total; i++)
   {
      s->pairNotMatched(p);
      s = (Special*)s->getNext();
   }
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Specials::pairMatched(Pair* p)
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();

   /* Tell all Specials! */
   for(i=0; i < total; i++)
   {
      s->pairMatched(p);
      s = (Special*)s->getNext();
   }
}

/***********************************************************************
 *                                find                                 *
 ***********************************************************************/
Special* Specials::find(int type)
{
   int i;
   int total = list->getTotal();
   Special* s = (Special*)list->getFirst();

   /* search all list */
   for(i=0; i < total; i++)
   {
      if(s->getType() == type)
      {
         return(s);
      }
      s = (Special*)s->getNext();
   }

   return(NULL);
}

/***********************************************************************
 *                              pauseTimer                            *
 ***********************************************************************/
void Specials::pauseTimer()
{
   int i;
   int total = list->getTotal();
   Special* sp = (Special*)list->getFirst();

   for(i=0; i < total; i++)
   {
      sp->pauseTimer();
      sp = (Special*)sp->getNext();
   }
}

/***********************************************************************
 *                             resumeTimer                             *
 ***********************************************************************/
void Specials::resumeTimer()
{
   int i;
   int total = list->getTotal();
   Special* sp = (Special*)list->getFirst();

   for(i=0; i < total; i++)
   {
      sp->resumeTimer();
      sp = (Special*)sp->getNext();
   }
}
   
/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Specials::hideIcons()
{
   guiOverlay->hide();
}
 
/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Specials::showIcons()
{
   guiOverlay->show();
}

/***********************************************************************
 *                            static members                           *
 ***********************************************************************/
Kobold::List* Specials::list = NULL;
Level* Specials::curLevel = NULL;
Ogre::Overlay* Specials::guiOverlay = NULL;
Goblin::Ibutton* Specials::cancel = NULL;
int Specials::curPoints = 0;

}


