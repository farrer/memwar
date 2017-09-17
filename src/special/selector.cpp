
#include "selector.h"

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
SpecialSelector::SpecialSelector()
{
    SpecialCard* card;
    Special* s = static_cast<Special*>(Specials::getFirst());

    int i;
    int t = Specials::getTotal();

    cancel = NULL;
    curSpecial = NULL;

    /* Get all available specials */
    for(i=0; i < t; i++)
    {
       s->getIcon()->hide();
       s->getIconBw()->hide();
       
       if( (s->getCost() <= Specials::getPoints()) &&
           (!s->isActive()) )
       {
          card = new SpecialCard();
          card->cardSpecial = s;
          card->cardImage = s->getCardImage();
          card->icon = s->getIcon();
          card->iconBw = s->getIconBw();
          curSpecial = card;

          insert(card);
       }
       
       s = static_cast<Special*>(s->getNext());
    }

    if(curSpecial)
    {
       curSpecial->cardImage->show();
       cancel = Specials::getCancelImage();
       cancel->show();
       setIcons();
    }
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
SpecialSelector::~SpecialSelector()
{
   /* Hide cancel image */
   if(cancel)
   {
      cancel->hide();
   }

   /* Clear available specials list */
   clearList();
}

/***********************************************************************
 *                               setIcons                              *
 ***********************************************************************/
void SpecialSelector::setIcons()
{
   int i;
   SpecialCard* c = static_cast<SpecialCard*>(getFirst());
   for(i=0; i < getTotal(); i++)
   {
      if(c == curSpecial)
      {
         c->icon->show();
         c->iconBw->hide();
      }
      else
      {
         c->icon->hide();
         c->iconBw->show();
      }

      /* Set dimensions */
      c->icon->setPosition(78 + (i*68), 315);
      c->icon->setDimensions(50, 50);
      c->iconBw->setPosition(78 + (i*68), 315);
      c->iconBw->setDimensions(50, 50);

      /* Next! */
      c = static_cast<SpecialCard*>(c->getNext());
   }
}

/***********************************************************************
 *                             setCurrent                              *
 ***********************************************************************/
void SpecialSelector::setCurrent(SpecialCard* c)
{
   if(c != curSpecial)
   {
      /* Show new */
      c->icon->show();
      c->iconBw->hide();
      c->cardImage->show();

      /* Hide Current */
      curSpecial->icon->hide();
      curSpecial->iconBw->show();
      curSpecial->cardImage->hide();

      /* Make Current */
      curSpecial = c;
   }
}

/***********************************************************************
 *                             verifyAction                            *
 ***********************************************************************/
bool SpecialSelector::verifyAction(int mouseX, int mouseY, 
                                   bool leftButtonPressed)
{
   if(getTotal() <= 0)
   {
      /* No avaiable specials, done. */
      return(true);
   }

   /* verify cancel button */
   if(cancel->verifyEvents(mouseX, mouseY, leftButtonPressed) == 
         IBUTTON_EVENT_PRESSED)
   {
      /* Canceled the use. Just return. */
      curSpecial->cardImage->hide();
      Specials::resetIcons();
      return(true);
   }
   /* Let's verify if clicked on the card itself */
   else if(curSpecial->cardImage->verifyEvents(mouseX,mouseY,leftButtonPressed) 
         == IBUTTON_EVENT_PRESSED)
   {
      Specials::activate(curSpecial->cardSpecial->getType());
      curSpecial->cardImage->hide();
      Specials::resetIcons();
      return true;
   }
   else
   {
      /* Verify card change */
      int i;
      SpecialCard* c = static_cast<SpecialCard*>(getFirst());
      for(i=0; i < getTotal(); i++)
      {
         if( (c != curSpecial) && 
             (c->iconBw->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
              IBUTTON_EVENT_PRESSED) )
         {
            setCurrent(c);
            break;
         }

         c = static_cast<SpecialCard*>(c->getNext());
      }
   }

   return false;
}

}

