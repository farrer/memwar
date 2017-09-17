
#ifndef _memwar_score_h
#define _memwar_score_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/OgreRenderWindow.h>

#include <goblin/image.h>
#include <goblin/ibutton.h>
#include "world.h"

namespace MemWar
{

class Level;

/*! The score class, used to display scores about a level
 * (Top and Current ones) or information about how to unlock a locked level. */
class Score
{
   public:

      enum ScoreReturn
      {
         RETURN_OTHER,
         RETURN_NEXT_LEVEL,
         RETURN_SHOW_LIST,
         RETURN_REPLAY_LEVEL,
         RETURN_PLAY_LEVEL,
         RETURN_CANCEL
      };
 
      /*! Init the score structure to use over game 
       * \param ogreOverlay overlay to use. */
      static void init(Ogre::Overlay* ogreOverlay);
      /*! Finish the use of the score (usually, on game quit) */
      static void finish();

      /*! Display the score of a level on the screen before it starts */
      static void display(Ogre::String levelTitle, int totalTime, 
            int totalMoves, int totalStars);
      /*! Display the score on screen, after the current level is finished. */
      static void display();
      /*! Display the screen with how to unlock this locked level */
      static void displayAsLocked(Ogre::String levelLockedInfo);

      /*! Treat the score screen for input
       * \return ScoreReturn constant */
      static int treat(int mouseX, int mouseY, bool leftButtonPressed);
   
      /*! Return true when displaying scores */
      static bool isDisplaying();
   
      /*! Reset all current statistics */
      static void reset(Level* level, World* world);
   
      /*! increment total flips counter */
      static void flippedBlock();
      /*! increment total challenges losses */
      static void lostChallenge();
      /*! increment used special */
      static void usedSpecial();
   
      /*! Set the total time elapsed on level */
      static void setTotalTime(int seconds);

      /*! Directly hide the score (wihtou animations). */
      static void hide();
   
   protected:
      /*! Send all score elements to its hide position. */
      static void sendToHidePosition();

      /*! \return filename representing the number of stars. */
      static Ogre::String getStarsFileName(int stars);
      
      /*! Do the display of score screen, previous to level launch or after 
       * finished it
       * \param afterLevel true if after finishing the level, false to call
       *                   previous to launch it.
       * \param locked if to display locked info instead of score.
       * \param levelInfo if locked, how to unlock it, otherwise level title.
       * \param timeRecord if broke a time record
       * \param movesRecord if broke a moves record */
      static void doDisplay(bool afterLevel, bool locked, 
            Ogre::String levelInfo, int totalTimeStars, bool timeRecord, 
            int totalMovesStars, bool movesRecord, int totalStars);

   private:
      Score(){};
   
      static int totalTime;           /**< Total time elapsed on the level */
      static int totalFlips;          /**< Total Block flips on level */
      static int totalChallengesLost; /**< Number of "missed" challenges */
      static int totalSpecials;       /**< Number of specials used on level */
    
      static bool displayed;          /**< If is displaying */
   
      static Goblin::Image* backImage;/**< background to the score display */
      static Goblin::Image* lockedBackImage;/**< background to locked display */
      static Goblin::Ibutton* replayButton; /**< Button to play level again */
      static Goblin::Ibutton* playButton; /**< Button to play level  */
      static Goblin::Ibutton* cancelButton; /**< Button to cancel  */
      static Goblin::Ibutton* nextButton; /**< Button to go to next level */
      static Goblin::Ibutton* listButton; /**< Button to go to level's list */

      static Goblin::TextBox* textTitle; /**< Text with score title */

      static Goblin::TextBox* textTime; /**< Text with elapsed time */
      static Goblin::Image* starsTime; /**< Stars obtained with time */

      static Goblin::TextBox* textMoves; /**< Text with total moves */
      static Goblin::Image* starsMoves; /**< Stars obtained with move */
      
      static Goblin::Image* starsLevel; /**< Stars for the level */

      static Ogre::Overlay* ogreOverlay; /**< Overlay used. */

      static Level* curLevel; /**< Current level  */
      static World* curWorld; /**< Current World */

      static int treatReturn; /**< What treat should return after hide. */
};
   
}

#endif

