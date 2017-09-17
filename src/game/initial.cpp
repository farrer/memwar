
#include "initial.h"
#include "options.h"

#include <iostream>
using namespace std;

namespace MemWar
{
   
#define INITIAL_NEW_IMAGE      "initial/new.png"
#define INITIAL_OPTIONS_IMAGE  "initial/options.png"
#define INITIAL_INFO_IMAGE     "initial/info.png"
#define INITIAL_QUIT_IMAGE     "initial/exit.png"
#define INITIAL_RETURN_IMAGE   "initial/return.png"

#define INITIAL_TITLE_IMAGE    "initial/title.png"
#define INITIAL_DNTEAM_IMAGE   "initial/dnteam.png"
#define INITIAL_CREDITS_IMAGE  "scores/credits.png"

#define OPT_MUSIC_ON           "initial/music_on.png"
#define OPT_MUSIC_OFF          "initial/music_off.png"
#define OPT_SOUND_ON           "initial/sndfx_on.png"
#define OPT_SOUND_OFF          "initial/sndfx_off.png"

#define GUI_GROUP              "gui"
   
#define HIDDEN_POS -200

#define INITIAL_Y  100

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Initial::Initial()
{
   state = INITIAL_STATE_DEFAULT;
   
   /* Create the overlays */
   ogreOverlay = Ogre::OverlayManager::getSingletonPtr()->create("InitialOv");
   ogreOverlay->setZOrder(647);
   ogreOverlay->show();
   
   /* Open and set Images */
   logo = new Goblin::Image(ogreOverlay, INITIAL_TITLE_IMAGE, GUI_GROUP);
   logo->setPosition(Goblin::ScreenInfo::getHalfWindowWidth() - 128, -200);

   dnteam = new Goblin::Image(ogreOverlay, INITIAL_DNTEAM_IMAGE, GUI_GROUP);
   dnteam->setPosition(Goblin::ScreenInfo::getWindowWidth() + 80,
                       Goblin::ScreenInfo::getWindowHeight() + 80);
   
   newButton = new Goblin::Ibutton(ogreOverlay, INITIAL_NEW_IMAGE, GUI_GROUP, 
                                   "Play", "infoFontOut", 16);
   newButton->setPosition(HIDDEN_POS, HIDDEN_POS);

   optionsButton = new Goblin::Ibutton(ogreOverlay, INITIAL_OPTIONS_IMAGE, 
         GUI_GROUP, "Options", "infoFontOut", 16);
   optionsButton->setPosition(HIDDEN_POS, HIDDEN_POS);

   infoButton = new Goblin::Ibutton(ogreOverlay, INITIAL_INFO_IMAGE, GUI_GROUP,
                                    "Credits", "infoFontOut", 16);
   infoButton->setPosition(HIDDEN_POS, HIDDEN_POS);


#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   quitButton = new Goblin::Ibutton(ogreOverlay, INITIAL_QUIT_IMAGE, GUI_GROUP,
                                    "Quit", "infoFontOut", 16);
   quitButton->setPosition(HIDDEN_POS, HIDDEN_POS);
#endif

   /* Reusable buttons */
   returnButton = new Goblin::Ibutton(ogreOverlay, INITIAL_RETURN_IMAGE, 
         GUI_GROUP, "Back", "infoFontOut", 16);
   returnButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   returnButton->hide();

   /* Credits */
   creditsImage = new Goblin::Ibutton(ogreOverlay, INITIAL_CREDITS_IMAGE, 
         GUI_GROUP);
   creditsImage->setPosition(-400, -400);
   creditsImage->hide();

   /* Options Buttons */
   musicOnButton = new Goblin::Ibutton(ogreOverlay, OPT_MUSIC_ON, GUI_GROUP,
         "Music On", "infoFontOut", 16);
   musicOnButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   musicOnButton->hide();

   musicOffButton = new Goblin::Ibutton(ogreOverlay, OPT_MUSIC_OFF, GUI_GROUP,
         "Music Off", "infoFontOut", 16);
   musicOffButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   musicOffButton->hide();

   soundOnButton = new Goblin::Ibutton(ogreOverlay, OPT_SOUND_ON, GUI_GROUP,
         "Soundfx On", "infoFontOut", 16);
   soundOnButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   soundOnButton->hide();

   soundOffButton = new Goblin::Ibutton(ogreOverlay, OPT_SOUND_OFF, GUI_GROUP,
         "Soundfx Off", "infoFontOut", 16);
   soundOffButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   soundOffButton->hide();

   close(true);
}
   
/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Initial::~Initial()
{
   /* Clear images */
   delete newButton;
   delete optionsButton;
   delete infoButton;
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   delete quitButton;
#endif
   delete returnButton;
   delete soundOnButton;
   delete soundOffButton;
   delete musicOnButton;
   delete musicOffButton;

   delete dnteam;
   delete logo;
   delete creditsImage;
   
   /* Bye Overlay */
   Ogre::OverlayManager::getSingletonPtr()->destroy(ogreOverlay);
}
   
/***********************************************************************
 *                                 open                                *
 ***********************************************************************/
void Initial::open()
{
   int sizeMod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   state = INITIAL_STATE_DEFAULT;
   dnteam->show();
   dnteam->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() - 60 * sizeMod,
         Goblin::ScreenInfo::getWindowHeight() - 60 * sizeMod);
   logo->show();
   logo->setTargetPosition(
         Goblin::ScreenInfo::getHalfWindowWidth() - 128 * sizeMod,
         10 * sizeMod, 20);
   ogreOverlay->show();
   
   /* Define buttons */
   int y = INITIAL_Y * sizeMod;
   int incY = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 160 : 80;
   newButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
         (32 * sizeMod), y);
   newButton->show();
   y += incY;
   optionsButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                    (32 * sizeMod), y);
   optionsButton->show();

   y += incY;
   infoButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                    (32 * sizeMod), y);
   infoButton->show();

   
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   /* No quit button for IOS! */
   y += incY;
   quitButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                 (32 * sizeMod), y);
   quitButton->show();
#endif
}
   
/***********************************************************************
 *                                close                                *
 ***********************************************************************/
void Initial::close(bool hideBackground)
{
   if(isRunning())
   {
      state = INITIAL_STATE_CLOSED;
      newButton->hide();
      optionsButton->hide();
      infoButton->hide();
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
      quitButton->hide();
#endif
      returnButton->hide();
   }

   if(hideBackground)
   {
      dnteam->hide();
      logo->hide();
      ogreOverlay->hide();
   }
}

/***********************************************************************
 *                        setOptionButtonsByState                      *
 ***********************************************************************/
void Initial::setOptionButtonsByState()
{
   if(Options::isMusicEnabled())
   {
      musicOnButton->show();
      musicOffButton->hide();
   }
   else 
   {
      musicOnButton->hide();
      musicOffButton->show();
   }

   if(Options::isSoundEnabled())
   {
      soundOnButton->show();
      soundOffButton->hide();
   }
   else 
   {
      soundOnButton->hide();
      soundOffButton->show();
   }
}

/***********************************************************************
 *                             showOptions                             *
 ***********************************************************************/
void Initial::showOptions()
{
   state = INITIAL_STATE_OPTIONS;
   setOptionButtonsByState();
   
   int incY = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 160 : 80;
   int sizeMod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   int y = INITIAL_Y * sizeMod;

   /* Options and back buttons to visible position */
   musicOnButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                    (32 * sizeMod), y);
   musicOffButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                     (32 * sizeMod), y);
   y += incY;
   soundOnButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                    (32 * sizeMod), y);
   soundOffButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                     (32 * sizeMod), y);
   y += incY;
   returnButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                                   (32 * sizeMod), y);
   returnButton->show();

   /* Initial Screen Buttons to hidden ones */
   hideInitialButtons();
}

/***********************************************************************
 *                        hideInitialButtons                           *
 ***********************************************************************/
void Initial::hideInitialButtons()
{
   newButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   optionsButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   infoButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   quitButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
#endif

}

/***********************************************************************
 *                            closeOptions                             *
 ***********************************************************************/
void Initial::closeOptions()
{
   state = INITIAL_STATE_DEFAULT;

   musicOnButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   musicOffButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   soundOnButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   soundOffButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   returnButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS); 
   returnButton->show();

   /* Set and save options. Each option is enabled if its respective 
   * enabled button is visible. */
   Options::setSoundOptions(soundOnButton->isVisible(),
                       musicOnButton->isVisible());
   Options::save();
   Options::applySoundOptions();

   open();
}

   
/***********************************************************************
 *                             verifyAction                            *
 ***********************************************************************/
int Initial::verifyAction(int mouseX, int mouseY, bool leftButtonPressed)
{
   dnteam->update();
   logo->update();

   if(state == INITIAL_STATE_CREDITS)
   {
      /* Update initial screen buttons */
      newButton->update();
      optionsButton->update();
      infoButton->update();
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
      quitButton->update();
#endif

      /* Check click on credits image to close it. */
      if(creditsImage->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         creditsImage->setTargetPosition(-400, -400);
         state = INITIAL_STATE_DEFAULT;
         open();
      }
   }
   else if(state == INITIAL_STATE_DEFAULT)
   {
      /* Update buttons to hide position, if needed */
      creditsImage->update();
      if(!creditsImage->isUpdating())
      {
         creditsImage->hide();
      }
      if(returnButton->isVisible())
      {
         musicOnButton->update();
         musicOffButton->update();
         soundOnButton->update();
         soundOffButton->update();
         returnButton->update();

         /* Hide when at hidden pos */
         if(returnButton->getPosX() == HIDDEN_POS)
         {
            musicOnButton->hide();
            musicOffButton->hide();
            soundOnButton->hide();
            soundOffButton->hide();
            returnButton->hide();
         }
      }

      if(newButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         close(false);
         return INITIAL_STATE_NEW;
      }
      if(optionsButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         showOptions();
         return INITIAL_STATE_OPTIONS;
      }
      if(infoButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         state = INITIAL_STATE_CREDITS;
         int sizeMod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
         int pX = Goblin::ScreenInfo::getHalfWindowWidth() - (256 * sizeMod);
         int pY = Goblin::ScreenInfo::getHalfWindowHeight() - (128 * sizeMod);
         creditsImage->show();
         creditsImage->setTargetPosition(pX, pY);
         hideInitialButtons();
         return INITIAL_STATE_DEFAULT;
      }
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
      if(quitButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         close(true);
         return INITIAL_STATE_QUIT;
      }
#endif
   }
   else if(state == INITIAL_STATE_OPTIONS)
   {
      if(newButton->isVisible())
      {
         newButton->update();
         optionsButton->update();
         infoButton->update();
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
         quitButton->update();
#endif

         if(newButton->getPosX() == HIDDEN_POS)
         {
            newButton->hide();
            optionsButton->hide();
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
            quitButton->hide();
#endif
         }
      }

      if(returnButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         closeOptions();
      }
      if(musicOnButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         musicOffButton->show();
         musicOnButton->hide();
      }
      if(musicOffButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         musicOnButton->show();
         musicOffButton->hide();
      }
      if(soundOnButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         soundOffButton->show();
         soundOnButton->hide();
      }
      if(soundOffButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         soundOnButton->show();
         soundOffButton->hide();
      }
   }
   
   return INITIAL_STATE_DEFAULT;
}
   
/***********************************************************************
 *                              isRunning                              *
 ***********************************************************************/
bool Initial::isRunning()
{
   return state == INITIAL_STATE_DEFAULT || state == INITIAL_STATE_OPTIONS ||
          state == INITIAL_STATE_CREDITS;
}
   
}


