#ifndef _memwar_world_h
#define _memwar_world_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <goblin/ibutton.h>

#include "selector.h"

namespace MemWar
{

class World;

#define UNDEFINED_WORLD_LEVEL_VALUE  65535

/** Level information for its world. This class keeps info about needed
 * achievements, current obtained scores and level filename. */
class WorldLevel : public Selectable
{
   public:
      /*! Constructor */
      WorldLevel(int totalLevels);
      /*! Destructor */
      ~WorldLevel();

      /*! Set level's vector index */
      void setIndex(int i);

      /*! \return if current level is locked */
      bool isLocked();

      /*! Set current better time to finish the level
       * \param time time in seconds. */
      void setScoreTime(int time);
      /*! Set current minimum moves to finish the level
       * \param moves ammount of moves taken */
      void setScoreMoves(int moves);
      /*! Set current better number of stars obtained for the whole level
       * \param stars amount of stars (from 0 to 3) */
      void setScoreStars(int stars);

      /*! \return current minimum time (in seconds) to finish the level */
      int getScoreTime();
      /*! \return current number of stars obtained for the level */
      int getScoreStars();
      /*! \return current minimum amount of moves to finish the level */
      int getScoreMoves();

      /*! With each up-to-date levels information, try to unlock the current
       * if needed stars were obtained.
       * \note usually called after a level was finished (and a new star 
       * record was obtained in it).
       * \param world the world where this level is. */
      void tryToUnlock(World* world);

      /*! Unlock the level, without checking pre-requisites. */
      void unlock();

      /*! Set the needed stars in a level to unlock the current level.
       * Could be called multiple times, usually for distinct levelIndex. */
      void setNeededStars(int levelIndex, int neededStars);

      /** \return level index on world vector */
      int getIndex();

      /*! Define information describeing what is needed to unlock the level */
      void setInfoOnHowToUnlock(Ogre::String info);

      /*! \return information describeing what is needed to unlock the level */
      Ogre::String getInfoOnHowToUnlock();

   private:
      /*! Total stars obtained in the level (if any) */
      int totalStars;
      /*! Better level finish time (in seconds) */
      int time;
      /*! Minimum moves obtained at the level.  */
      int moves;
      /*! If the level is currently locked */
      bool locked;
      /*! Vector with amount of stars needed for each world's level to unlock
       * the current one. */
      int* neededStars;
      /*! Total levels avaiable in the world. */
      int totalLevels;
      /*! Level index on world vector. */
      int index;
      /*! Text describing what is need to unlock the level */
      Ogre::String infoOnHowToUnlock;
};

/*! A world is a set of levels. Each level in the world could be locked by 
 * needed stars obtained in previous levels. */
class World : public Selector
{
   public:
      /*! Constructor 
       * \param definitionFile filename of world's definition.
       * \param scoresFile filename of scores' file. 
       * \param newButtonFileName button filename to use as new level button.
       * \param ogreOverlay overlay to use. */
      World(Ogre::String definitionFile, Ogre::String scoresFile,
            Ogre::String newButtonFileName, Ogre::Overlay* ogreOverlay);
      /*! Destructor */
      ~World();

      /*! Save current user scores to world scores file. */
      void saveScores();

      /*! \return Get selected level, after treat RETURN_OPEN_LEVEL. */
      WorldLevel* getSelectedLevel();

      /*! \return next level, if available and unlocked */
      WorldLevel* getNextLevel(WorldLevel* level);

      /* Try to unlock currently locked levels. */
      void tryToUnlock();

   protected:
      /*! Create a selectable object.
       * \return Selectable created */
      Selectable* createSelectable();

      /*! Delete a selectable object.
       * \param selectable pointer of object to delete */
      void deleteSelectable(Selectable* selectable);

   private:

      /*! Check if need to change a level button image (after a new 
       * score set) */
      void checkButtonImages();

      /*! World's name */
      Ogre::String name;
      /*! Scores FileName */
      Ogre::String scoresFileName;
      /*! Button image to use as 'new' level */
      Ogre::String newButtonFileName;
      
};

}

#endif

