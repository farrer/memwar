
#ifndef _memwar_Level_h
#define _memwar_Level_h

#include <OGRE/Ogre.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreTextAreaOverlayElement.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>

#include "mblock.h"
#include "pair.h"
#include "score.h"
#include "world.h"

#include <kobold/timer.h>
#include <goblin/textbox.h>
#include <goblin/texttitle.h>
#include <goblin/ibutton.h>

#include "../challenge/challenge.h"
#include "../special/special.h"

namespace MemWar
{

#define MAX_PAIRS  20

/*! A Level is a game table with turnable blocks. */
class Level: public Kobold::List
{
   public:

      enum {
         STATE_PLAYING,
         STATE_PAUSED,
         STATE_SHOWING_INFO,
         STATE_STATISTICS
      };

      /*! Constructor
       * \param fileName -> name of file to use
       * \param replaying if replaying. In this case won't show any 
       *        challenge's info */
      Level(World* world, WorldLevel* worldLevel, 
            Ogre::SceneManager* ogreSceneManager,
            Goblin::Model3d* field, Ogre::Overlay* ogreOverlay,
            bool replaying);
      /*! Destructor */
      ~Level();

      /*! \return Field scene node used at the level */
      Goblin::Model3d* getField();

      /*! Received from the input that someone selected this sceneNode */
      void select(Ogre::SceneNode* scNode);

      /*! Unselect current nodes */
      void unselect();

      /*! Unset the zoom block, if defined. */
      void unsetZoomBlock();

      /*! Verify if the Level is completed or not
       * \return true when Level is completed. */
      bool isCompleted();

      /*! Update the Level, doing related changes, etc. */
      void update(bool updateSpecials, int mouseX, int mouseY, 
            bool leftButtonPressed);

      /*! Descrement the counter of pairs done */
      void decPairsDone();

      /*! Set the number of pairs done */
      void setPairsDone(int value);

      /*! Get the total number of pairs at the Level */
      int getTotalPairs();

      /*! Get the number of matched pairs done */
      int getMatchedPairsCount();

      /*! Shuffle all pieces from the Level */
      void shuffle();

      /*! Verify if some animation is running at the Level
       * \return true if animation is running */
      bool isAnimationRunning();

      /*! Clear (and remove) all active challenges */
      void clearChallenges();

      /*! Get the selected pair
       * \return pointer to the selected pair */
      Pair* getSelectedPair();

      /*! Get the scene manager pointer
       * \return pointer to the scene manager used */
      Ogre::SceneManager* getSceneManager();

      /*! Get the overlay related to the gui
       * \return pointer to the overlay */
      Ogre::Overlay* getOverlay();

      /*! Pause the Level and all challenge timers */
      void pauseTimer();
      /*! Resume the Level and all challenge timers */
      void resumeTimer();

      /*! Pause all active challenge timers. */
      void pauseChallengeTimers();
      /*! Resume all active challenge timers */
      void resumeChallengeTimers();

      /*! \return current level state enum constant. */
      int getState();

      /*! Hide the UI related to the level */
      void hideUI();

      /*! \return maximum time (in seconds) to obtain two stars. */
      int getTimeTwoStars();

      /*! \return maximum time (in seconds) to obtain three stars. */
      int getTimeThreeStars();

      /*! \return maximum number of moves to obtain two stars. */
      int getMovesTwoStars();

      /*! \return maximum number of moves to obtain three stars. */
      int getMovesThreeStars();

      /*! \return level's information at world */
      WorldLevel* getWorldLevel();

   protected:

      /*! Get a random value from [0, to max] */
      int getRandomValue(int max);
      
      /*! Get the block related to the scene node
       * \param scNode -> pointer to the sceneNode to get related block
       * \return pointer to the block found or NULL */
      Block* getBlock(Ogre::SceneNode* scNode);

      /*! Get first block without a defined material
       * \return pointer to block found or NULL */
      Block* getFirstWithoutMaterial();
      /*! Get a random block without material
       * \return pointer to the block or NULL */
      Block* getRandomWithoutMaterial();

      /*! Update the text with timer */
      void updateTimeText();

      int pairsDone;             /**< number of pairs currently done */
      WorldLevel* worldLevel;    /*< Level info from world.**/

      Goblin::Model3d* field; /**< Field scene node used. */

      Ogre::SceneManager* sceneManager; /**< Pointer to the scene manager */

      Pair selPair;              /**< Current Selected pair */

      Kobold::Timer levelTimer;  /**< The main timer of the Level */

      Challenges* challengeList; /**< All active challenges */

      Block* zoomBlock;          /**< Current block with zoom */

      Ogre::Overlay* guiOverlay;  /**< Overlay for the Level GUI */
      Goblin::TextBox* textTime;  /**< Level time text */

      Goblin::TextTitle* levelTitle;/**< Level title to display */
      Goblin::Ibutton* okButton; /**< Button for ok actions */
      Challenge* curInfoChallenge; /**< Current displaying info challenge */
   
      bool mustShowInfo[TOTAL_CHALLENGE_TYPES]; /**< If must show info for a
                                                   challenge when it appears */

      bool pausedOnlyChallenges; /**< If paused all or only challenges */

      int state; /**< Current level state */

      int timeTwoStars;   /**< Maximum time (in s) to obtain two stars */
      int timeThreeStars; /**< Maximum time (in s) to obtain three stars */
      int movesTwoStars; /**< Maximum number of moves to obtain two stars */ 
      int movesThreeStars; /**< Maximum number of moves to obtain three stars*/
   
};

}

#endif

