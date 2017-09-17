#ifndef _memwar_pause_h
#define _memwar_pause_h

#include <goblin/ibutton.h>
#include <goblin/screeninfo.h>

namespace MemWar
{

/*! The Pause class implements the interection to pause the game and show
 * a pause screen to the user - in which he/she can continue the game or exit
 * it. */
class Pause
{
   public:
      /*! Constructor */
      Pause(Ogre::Overlay* ogreOverlay);
      /*! Destructor */
      ~Pause();

      enum
      {
         /*! Nothing happens, must go check otherwhere */
         RETURN_NONE,
         /*! Something happen internally */
         RETURN_OTHER,
         /*! Should pause the game */
         RETURN_PAUSE,
         /*! Should return to the paused game */
         RETURN_RESUME,
         /*! Should restart current paused game */
         RETURN_RESTART,
         /*! Should exit from the paused game */
         RETURN_EXIT
      };

      /*! Hide everithing */
      void hide();

      /*! \return if is at paused state */
      bool isPaused();

      /*! Reset the controller (usually before initing a new game */
      void reset();

      /*! Treat the pause, entering or exiting from it.
       * \return RETURN_ constant */
      int treat(int mouseX, int mouseY, bool leftButtonPressed);

   private:

      /*! Resume the paused game. */
      void resume();

      /*! Enter pause mode */
      void pause();

      Ogre::Overlay* ogreOverlay; /**< Overlay used at the selector. */

      bool paused; /**< if game is paused or not */

      /*! return value for treat, to wait for all animations ended */
      int retValue;

      Goblin::Ibutton* pauseButton;
      Goblin::Ibutton* resumeButton;
      Goblin::Ibutton* restartButton;
      Goblin::Ibutton* exitButton;
};

};

#endif

