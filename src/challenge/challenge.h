#ifndef _memwar_challenge_h
#define _memwar_challenge_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/OgreRenderWindow.h>


#include <kobold/list.h>
#include <goblin/image.h>
#include <kobold/timer.h>

#include "challengeinfo.h"

namespace MemWar
{

#define TOTAL_CHALLENGE_TYPES   7
#define CHALLENGE_OVERHEAD    200

enum
{
   CHALLENGE_BOMB = CHALLENGE_OVERHEAD,
   CHALLENGE_THIEF,
   CHALLENGE_LLAMA,
   CHALLENGE_ALIEN,
   CHALLENGE_ZOSIMA,
   CHALLENGE_COMPASS,
   CHALLENGE_PUNK
};

#define CHALLENGES_DIR              "challenges/"
#define CHALLENGE_CARDS_DIR         "challenges/cards/"

#define ICON_CHALLENGE_BOMB         "bomb.png"
#define ICON_CHALLENGE_THIEF        "thief.png"
#define ICON_CHALLENGE_LLAMA        "llama.png"
#define ICON_CHALLENGE_ALIEN        "alien.png"
#define ICON_CHALLENGE_ZOSIMA       "zosima.png"
#define ICON_CHALLENGE_COMPASS      "compass.png"
#define ICON_CHALLENGE_PUNK         "punk.png"

/* Forward declaration */
class Level; 
class Block;
class Pair;

/*! The basic virtual challenge class
 * \note: You must change blocks/etc only at the update function,
 *        not at those *Selected and *Matched ones, to avoid
 *        strange behaviours of concurrent challenges. */
class Challenge: public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param t -> challenge id constant
       * \param l -> pointer to the current level */
      Challenge(int t, Level* l);
      /*! Destructor */
      virtual ~Challenge();

      /*! Get the challenge type constant */
      int getType();

      /*! Called when selected the first block
       * \param b -> block selected */
      virtual void firstBlockSelected(Block* b)=0;

      /*! Called when selected 2 blocks, not Pairs
       * \param p -> Pair selected */
      virtual void pairNotMatched(Pair* p)=0;

      /*! Called when matched a Pair with two blocks
       * \param p -> Pair selected */
      virtual void pairMatched(Pair* p)=0;

      /*! Called when removed an active challenge from list  */
      virtual void unApply()=0;

      /*! @return any remaining time (in seconds) the user should be aware of.
       *          < 0 to not show anything. */
      virtual int getRemainingTime()=0;

      /*! Verify if the challenge is active or not
       * \return true when it's active */
      bool isActive();

      /*! Do a step on the challenge (called each frame) */
      virtual void update()=0;

      /*! Pause the challenge timer */
      virtual void pauseTimer()=0;
      /*! Resume the challenge timer */
      virtual void resumeTimer()=0;

      /*! Set the challenge icon (challengeInfo)
       * \param ogreOverlay -> ogre overlay used
       * \param x -> x 'left' coordinate
       * \param y -> y 'up' coordinate 
       * \param imageFileName -> file name of the challenge image
       * \param useTime -> if will set remaining time or not  */
      void setIcon(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
            Ogre::String imageFileName, bool useTime);

      /*! Set the icon position
       * \param i -> index on list */
      void setIconPosition(int i);

      /*! Set if currently displaying challenge info */
      void setDisplayingInfo(bool displaying);

      /*! \return icon information. */
      ChallengeInfo* getIcon();

   protected:
      /*! Verify if the block is in the Pair list
       * \param b -> block to verify
       * \param Pairs -> vector with Pairs
       * \param totalPairs -> number of Pairs in vector
       * \return true if is. */
      bool inPair(Block* b, Pair* pairs, int totalPairs);
      /*! Repositionate all Pairs on the vector
       * \param Pairs -> vector with Pairs to repositionate
       * \param totalPairs -> total Pairs on vector
       * \param unMatch -> true to unMatch the Pairs */
      void reposPairs(Pair* pairs, int totalPairs, bool unMatch);

      int type;               /**< Challenge type */
      bool active;            /**< If is active or not */
      bool displayingInfo;    /**< If is displying info bout challenge */
      Level* curLevel;        /**< Pointer to the current level */
      ChallengeInfo* icon;    /**< Image of the challenge */
};

/*! The challenges is just the list of challenges */
class Challenges: public Kobold::List
{
   public:
      /*! Constructor
       * \param curLevel -> pointer to the current level */
      Challenges(Level* l);
      /*! Destructor */
      ~Challenges();

      /*! Add a challenge to the list
       * \param type -> type of the challenge to add */
      Challenge* add(int type);

      /*! Verify if an block type is a challenge or not
       * \param type -> block image type
       * \return true if is challenge, false otherwise. */
      bool isChallenge(int type);

      /*! Remove all inactive challenges from list */
      void removeInactive();

      /*! Update all active challenges, removing inactive ones from list */
      void update();

      /*! Called when selected the first block
       * \param b -> block selected */
      void firstBlockSelected(Block* b);

      /*! Called when selected 2 blocks, not Pairs
       * \param p -> Pair selected */
      void pairNotMatched(Pair* p);

      /*! Called when matched a Pair with two blocks
       * \param p -> Pair selected */
      void pairMatched(Pair* p);

      /*! Clear and unApply all active challenges */
      void clear();

      /*! Pause all challenge timers */
      void pauseTimer();

      /*! Resume all challenge timers */
      void resumeTimer();

   protected:
      /*! Find first challenge of type
       * \param type -> type of the challenge to find 
       * \return pointer to the challenge found or NULL */
      Challenge* find(int type);

      Level* curLevel;  /**< Pointer to the level related */
      Ogre::Overlay* guiOverlay;   /**< The overlay for challenges  */
};

}

#endif

