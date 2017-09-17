#ifndef _memwar_selector_h
#define _memwar_selector_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/OgreRenderWindow.h>

#include <kobold/list.h>
#include <goblin/image.h>
#include <goblin/ibutton.h>

#include "special.h"

namespace MemWar
{

/*! The special card used */
class SpecialCard : public Kobold::ListElement
{
   public:
      Goblin::Ibutton* cardImage;     /**< Image of the card */
      Goblin::Ibutton* icon;          /**< Icon */
      Goblin::Ibutton* iconBw;        /**< B&W Icon */
      Special* cardSpecial; /**< Special related to the card */
};

/*! The special selector class, used to select from available specials */
class SpecialSelector : public Kobold::List
{
   public:
      /* Constructor */
      SpecialSelector();
      /*! Destructor */
      ~SpecialSelector();

      /*! Verify action on the special's selector
       * \return true when done selecting. */
      bool verifyAction(int mouseX, int mouseY, bool leftButtonPressed);

   protected:
      /*! Set all icons positions */
      void setIcons();
      /*! Set new curSpecial */
      void setCurrent(SpecialCard* c);

      SpecialCard* curSpecial; /**< Current Selected special */
      Goblin::Ibutton* cancel;   /**< Pointer to the specials cancel image */
};

}


#endif

