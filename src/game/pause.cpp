#include "pause.h"

using namespace MemWar;

/****************************************************************************
 *                               Constructor                                *
 ****************************************************************************/
Pause::Pause(Ogre::Overlay* ogreOverlay)
{
   /* Set parameters */
   this->ogreOverlay = ogreOverlay;
   this->paused = false;
   this->retValue = RETURN_NONE;

   /* Create buttons */
   this->pauseButton = new Goblin::Ibutton(ogreOverlay, "initial/pause.png", 
         "gui");
   this->pauseButton->setPosition(Goblin::ScreenInfo::getWindowWidth() + 100, 
         32);
   this->pauseButton->hide();

   this->resumeButton = new Goblin::Ibutton(ogreOverlay,
         "initial/next.png", "gui", "Resume", "infoFontOut", 16);
   this->resumeButton->setPosition(Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() - 200);
   this->resumeButton->hide();

   this->restartButton = new Goblin::Ibutton(ogreOverlay, "initial/replay.png",
         "gui", "Restart", "infoFontOut", 16);
   this->restartButton->setPosition(Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight());
   this->restartButton->hide();

   this->exitButton = new Goblin::Ibutton(ogreOverlay,
         "initial/exit.png", "gui", "Exit", "infoFontOut", 16);
   this->exitButton->setPosition(Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() + 200);
   this->exitButton->hide();
}

/****************************************************************************
 *                                Destructor                                *
 ****************************************************************************/
Pause::~Pause()
{
   delete this->pauseButton;
   delete this->resumeButton;
   delete this->restartButton;
   delete this->exitButton;
}

/****************************************************************************
 *                                isPaused                                  *
 ****************************************************************************/
bool Pause::isPaused()
{
   return this->paused;
}

/****************************************************************************
 *                                  reset                                   *
 ****************************************************************************/
void Pause::reset()
{
   this->retValue = RETURN_NONE;
   resume();
}

/****************************************************************************
 *                                  resume                                  *
 ****************************************************************************/
void Pause::resume()
{
   int mod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   this->paused = false;
   this->pauseButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth()-(32 * mod),
         32 * mod);
   this->pauseButton->show();

   this->resumeButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() - 200);
   this->restartButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight());
   this->exitButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() + 200);
}

/****************************************************************************
 *                                  pause                                   *
 ****************************************************************************/
void Pause::pause()
{
   int mod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   this->paused = true;
   this->pauseButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         32 * mod);

   this->resumeButton->setTargetPosition(
         Goblin::ScreenInfo::getHalfWindowWidth() -
         (32 * mod),
         Goblin::ScreenInfo::getHalfWindowHeight() - 200);
   this->resumeButton->show();
   this->restartButton->setTargetPosition(
         Goblin::ScreenInfo::getHalfWindowWidth() -
         (32 * mod),
         Goblin::ScreenInfo::getHalfWindowHeight());
   this->restartButton->show();
   this->exitButton->setTargetPosition(
         Goblin::ScreenInfo::getHalfWindowWidth() -
         (32 * mod),
         Goblin::ScreenInfo::getHalfWindowHeight() + 200);
   this->exitButton->show();
}

/****************************************************************************
 *                                  hide                                    *
 ****************************************************************************/
void Pause::hide()
{
   this->pauseButton->hide();
   this->resumeButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() - 100);
   this->restartButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight());
   this->exitButton->setTargetPosition(
         Goblin::ScreenInfo::getWindowWidth() + 100,
         Goblin::ScreenInfo::getHalfWindowHeight() + 100);
}

/****************************************************************************
 *                                  treat                                   *
 ****************************************************************************/
int Pause::treat(int mouseX, int mouseY, bool leftButtonPressed)
{

   if( (this->retValue != RETURN_NONE) && 
       (!pauseButton->isUpdating()) &&
       (!resumeButton->isUpdating()) &&
       (!exitButton->isUpdating()) )
   {
      /* Animation ended, must return what is needed*/
      int ret = this->retValue;
      this->retValue = RETURN_NONE;
      return ret;
   }

   if(this->pauseButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
                  IBUTTON_EVENT_PRESSED)
   {
      pause();
      this->retValue = RETURN_PAUSE;
      return RETURN_OTHER;
   }
   else if(this->resumeButton->verifyEvents(mouseX, mouseY, 
            leftButtonPressed) == IBUTTON_EVENT_PRESSED)
   {
      resume();
      this->retValue = RETURN_RESUME;
      return RETURN_OTHER;
   }
   else if(this->restartButton->verifyEvents(mouseX, mouseY,
               leftButtonPressed) == IBUTTON_EVENT_PRESSED)
   {
      hide();
      this->retValue = RETURN_RESTART;
      return RETURN_OTHER;
   }
   else if(this->exitButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
                        IBUTTON_EVENT_PRESSED)
   {
      hide();
      this->retValue = RETURN_EXIT;
      return RETURN_OTHER;
   }

   return RETURN_NONE;
}

