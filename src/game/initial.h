#ifndef _memwar_Initial_h
#define _memwar_Initial_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlayContainer.h>

#include <goblin/image.h>
#include <goblin/ibutton.h>
#include <goblin/screeninfo.h>

#include "options.h"

namespace MemWar
{
   
enum
{
   INITIAL_STATE_DEFAULT,
   INITIAL_STATE_NEW,
   INITIAL_STATE_OPTIONS,
   INITIAL_STATE_CREDITS,
   INITIAL_STATE_QUIT,
   INITIAL_STATE_CLOSED
};

/*! The Initial class implements the Initial game menu */
class Initial
{
   public:
      /*! Constructor */
      Initial();
      /*! Destructor */
      ~Initial();
   
      /*! Open the Initial menu for use */
      void open();
   
      /*! Close the Initial menu 
       * \param hideBackground if will hide the background (title and dnteam), 
       *                       or keep them visible. */
      void close(bool hideBackground);
   
      /*! Verify if Initial menu is running or not */
      bool isRunning();
   
      /*! Verify action on Initial menu
       * \return -> current menu state */
      int verifyAction(int mouseX, int mouseY, bool leftButtonPressed);

   protected:
      /*! Show options menu. */
      void showOptions();
      /*! Close the options, save them and show again the main menu */
      void closeOptions();
      /*! Show/hide buttons according to option state */
      void setOptionButtonsByState();
      /*! Hide initial screen related buttons */
      void hideInitialButtons();
  
      int state;
   
      /* Initial */
      Goblin::Ibutton* newButton;
      Goblin::Ibutton* optionsButton;
      Goblin::Ibutton* infoButton;
      Goblin::Ibutton* quitButton;

      Goblin::Image* logo;
      Goblin::Image* dnteam;

      /* Credits */
      Goblin::Ibutton* creditsImage;

      /* Reusable */
      Goblin::Ibutton* returnButton;

      /* Options */
      Goblin::Ibutton* musicOnButton;
      Goblin::Ibutton* musicOffButton;
      Goblin::Ibutton* soundOnButton;
      Goblin::Ibutton* soundOffButton;
   
      Ogre::Overlay* ogreOverlay;
};

}


#endif

