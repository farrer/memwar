#ifndef _memwar_Special_h
#define _memwar_Special_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/OgreRenderWindow.h>

#include <kobold/list.h>
#include <goblin/image.h>
#include <goblin/ibutton.h>
#include <goblin/screeninfo.h>

namespace MemWar
{

#define TOTAL_SPECIAL_TYPES     5
#define SPECIAL_OVERHEAD      500

enum
{
   SPECIAL_WIND = SPECIAL_OVERHEAD,
   SPECIAL_OCARINA,
   SPECIAL_QUANTUM_PHYSICS,
   SPECIAL_ELEPHANT,
   SPECIAL_CRYSTAL_BALL
};

#define ICON_SPECIAL_WIND              "Specials/wind.png"
#define ICON_SPECIAL_OCARINA           "Specials/ocarina.png"
#define ICON_SPECIAL_QUANTUM_PHYSICS   "Specials/quantum.png"
#define ICON_SPECIAL_ELEPHANT          "Specials/elephant.png"
#define ICON_SPECIAL_CRYSTAL_BALL      "Specials/crystalball.png"

#define NORMAL_POINTS      1  /**< points received doing a common Pair */
#define CHALLENGE_POINTS   4  /**< points received doing a challenge Pair */

/* Forward declaration */
class Level; 
class Block;
class Pair;

/*! The basic virtual Special class
 * \note: You must change blocks/etc only at the update function,
 *        not at those *Selected and *Matched ones, to avoid
 *        strange behaviours of concurrent Specials. */
class Special: public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param t -> Special id constant */
      Special(int t);
      /*! Destructor */
      virtual ~Special();

      /*! Set the current level
       * \param pointer to the current level */
      void setLevel(MemWar::Level* l);

      /*! Get the Special type constant */
      int getType();

      /*! Get cost to use the challenge
       * \return number of points to use challenge */
      int getCost();

      /*! Called when selected the first block
       * \param b -> block selected */
      virtual void firstBlockSelected(Block* b)=0;

      /*! Called when selected 2 blocks, not Pairs
       * \param p -> Pair selected */
      virtual void pairNotMatched(Pair* p)=0;

      /*! Called when matched a Pair with two blocks
       * \param p -> Pair selected */
      virtual void pairMatched(Pair* p)=0;

      /*! Verify if the Special is active or not
       * \return true when it's active */
      bool isActive();

      /*! Activate the Special */
      void activate();

      /*! Do a step on the Special (called each frame) */
      virtual void update()=0;

      /*! Set the Special icon (SpecialInfo)
       * \param ogreOverlay -> ogre overlay used
       * \param x -> x 'left' coordinate
       * \param y -> y 'up' coordinate 
       * \param imageFileName -> file name of the Special image
       * \param useTime -> if will set remaining time or not  */
      void setIcon(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
                   Ogre::String imageFileName, Ogre::String imageBWFileName);

      /*! Set the icon position
       * \param x -> x position
       * \param y - > y position */
      void setIconPosition(int x, int y);

      /*! Set icon style based on avaiable
       * \param totalPoints -> number of points available */
      void setAvailable(int totalPoints);

      /*! Get the Special image */
      Goblin::Ibutton* getIcon();
      /*! Get the Special b&w image */
      Goblin::Ibutton* getIconBw();

      /*! Get the card image */
      Goblin::Ibutton* getCardImage();
      
      /*! Set card image */
      void setCard(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y,
            Ogre::String imageFileName);

      /*! Pause the Special timer */
      virtual void pauseTimer()=0;
      /*! Resume the Special timer */
      virtual void resumeTimer()=0;
 
   protected:
      /*! Verify if the block is in the Pair list
       * \param b -> block to verify
       * \param Pairs -> vector with Pairs
       * \param totalPairs -> number of Pairs in vector
       * \return true if is. */
      bool inPair(Block* b, Pair* Pairs, int totalPairs);

      int type;               /**< Challenge type */
      int cost;               /**< Cost to use the challenge */
      bool active;            /**< If is active or not */
      Level* curLevel;        /**< Pointer to the current level */
      Goblin::Ibutton* icon;    /**< Colored Special's Image */
      Goblin::Ibutton* iconBw;  /**< Black& White Special's image */
      Goblin::Ibutton* card;    /**< Special's card */
};

/*! The Specials is just an static list of Specials */
class Specials
{
   public:
      /*! Constructor */
      static void init();
      /*! Destructor */
      static void finish();

      /*! Set the current level
       * \param pointer to the current level */
      static void setLevel(MemWar::Level* l);

      /*! Add points to the Specials counter
       * \param p -> number of points to add */
      static void addPoints(int p);
      /*! Get current points at the Specials
       * \return -> avaialble points at Special */
      static int getPoints();

      /*! Get total Specials on list */
      static int getTotal();

      /*! Get first Special on list */
      static Special* getFirst();

      /*! Add a Special to the list
       * \param type -> type of the Special to add */
      static void add(int type);

      /*! Remove all inactive Specials from list */
      static void removeInactive();

      /*! Update all active Specials, removing inactive ones from list */
      static void update();

      /*! Called when selected the first block
       * \param b -> block selected */
      static void firstBlockSelected(Block* b);

      /*! Called when selected 2 blocks, not Pairs
       * \param p -> Pair selected */
      static void pairNotMatched(Pair* p);

      /*! Called when matched a Pair with two blocks
       * \param p -> Pair selected */
      static void pairMatched(Pair* p);

      /*! Use a Special (if have enought points to it)
       * \param type -> Special type constant */
      static void activate(int type);

      /*! Pause all Special timers */
      static void pauseTimer();

      /*! Resume all Special timers */
      static void resumeTimer();

      /*! Get the cancel image pointer */
      static Goblin::Ibutton* getCancelImage();

      /*! Reset the icons positons and dimensions */
      static void resetIcons();
   
      /*! Hide the available Special icons bar */
      static void hideIcons();
      /*! Display the available icons bar */
      static void showIcons();

   protected:
      /*! Find first Special of type
       * \param type -> type of the Special to find 
       * \return pointer to the Special found or NULL */
      static Special* find(int type);

   private:
      Specials(){};

      static Level* curLevel;           /**< Pointer to the level related */
      static Ogre::Overlay* guiOverlay; /**< The overlay for Specials  */
      static Goblin::Ibutton* cancel;   /**< Cancel Button */
      static int curPoints;             /**< Current Points */
      static Kobold::List* list;        /**< current list */
};

}

#endif

