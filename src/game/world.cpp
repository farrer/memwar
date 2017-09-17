#include "world.h"

#include <OGRE/OgreStringConverter.h>
#include <kobold/defparser.h>
#include <kobold/userinfo.h>

#include <iostream>

using namespace std;
using namespace MemWar;

#define DEFAULT_BUTTON_PLUS_SPACE_SIZE 72

/* Commom tokens */
#define TOKEN_LEVEL          "level"

/* Tokens for world's definition file */
#define TOKEN_TOTAL_LEVELS   "totalLevels"
#define TOKEN_NAME           "name"
#define TOKEN_LEVEL_FILE     "levelFile"
#define TOKEN_NEEDED_STARS   "neededStars"

/* Tokens for world's score file */
#define TOKEN_LOCKED         "locked"
#define TOKEN_TRUE           "true"
#define TOKEN_FALSE          "false"
#define TOKEN_TOTAL_STARS    "totalStars"
#define TOKEN_TIME           "time"
#define TOKEN_MOVES          "moves"
#define TOKEN_UNLOCK_INFO    "unlockInfo"

#define BUTTON_ONE_STAR      "initial/star1.png"
#define BUTTON_TWO_STARS     "initial/star2.png"
#define BUTTON_THREE_STARS   "initial/star3.png"
#define BUTTON_LOCKED        "initial/locked.png"

#define DEFAULT_UNLOCK_INFO  "You need at least one star in\nthe previous level."

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                WorldLevel                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/****************************************************************************
 *                               Constructor                                *
 ****************************************************************************/
WorldLevel::WorldLevel(int totalLevels)
{
   this->totalLevels = totalLevels;
   this->totalStars = 0;
   this->time = UNDEFINED_WORLD_LEVEL_VALUE;
   this->moves = UNDEFINED_WORLD_LEVEL_VALUE;
   this->infoOnHowToUnlock = DEFAULT_UNLOCK_INFO;

   /* Set as initially unlocked without any needed stars. */
   this->locked = false;
   this->neededStars =  new int[totalLevels];
   for(int i = 0; i < totalLevels; i++)
   {
      this->neededStars[i] = 0;;
   }
}

/****************************************************************************
 *                                Destructor                                *
 ****************************************************************************/
WorldLevel::~WorldLevel()
{
   delete[] neededStars;
}


/****************************************************************************
 *                                setIndex                                  *
 ****************************************************************************/
void WorldLevel::setIndex(int i)
{
   this->index = i;
}



/****************************************************************************
 *                                 isLocked                                 *
 ****************************************************************************/
bool WorldLevel::isLocked()
{
   return locked;
}

/****************************************************************************
 *                                getIndex                                  *
 ****************************************************************************/
int WorldLevel::getIndex()
{
   return index;
}

/****************************************************************************
 *                          setInfoOnHowToUnlock                            *
 ****************************************************************************/
void WorldLevel::setInfoOnHowToUnlock(Ogre::String info)
{
   this->infoOnHowToUnlock = info;
}

/****************************************************************************
 *                          getInfoOnHowToUnlock                            *
 ****************************************************************************/
Ogre::String WorldLevel::getInfoOnHowToUnlock()
{
   return this->infoOnHowToUnlock;
}

/****************************************************************************
 *                              setScoreTime                                *
 ****************************************************************************/
void WorldLevel::setScoreTime(int time)
{
   this->time = time;
}

/****************************************************************************
 *                              setScoreMoves                               *
 ****************************************************************************/
void WorldLevel::setScoreMoves(int moves)
{
   this->moves = moves;
}

/****************************************************************************
 *                              setScoreStars                               *
 ****************************************************************************/
void WorldLevel::setScoreStars(int stars)
{
   this->totalStars = stars;
}

/****************************************************************************
 *                              getScoreTime                                *
 ****************************************************************************/
int WorldLevel::getScoreTime()
{
   return time;
}

/****************************************************************************
 *                              getScoreStars                               *
 ****************************************************************************/
int WorldLevel::getScoreStars()
{
   return totalStars;
}

/****************************************************************************
 *                              getScoreMoves                               *
 ****************************************************************************/
int WorldLevel::getScoreMoves()
{
   return moves;
}

/****************************************************************************
 *                             setNeededStars                               *
 ****************************************************************************/
void WorldLevel::setNeededStars(int levelIndex, int neededStars)
{
   if(levelIndex < totalLevels)
   {
      /* If defined needed stars, should be locked until check. */
      this->locked = true;
      this->neededStars[levelIndex] = neededStars;
   }
}

/****************************************************************************
 *                               tryToUnlock                                *
 ****************************************************************************/
void WorldLevel::tryToUnlock(World* world)
{
   if(!locked)
   {
      /* No need to unlock a already unlocked level. */
      return;
   }
   for(int i = 0; i < totalLevels; i++)
   {
      WorldLevel* level = (WorldLevel*) world->getByIndex(i);
      if(neededStars[i] > level->getScoreStars())
      {
         /* Still locked. */
         return;
      }
   }
   /* All pre-requisites met: unlocked! */
   this->locked = false;
}

/****************************************************************************
 *                                 unlock                                   *
 ****************************************************************************/
void WorldLevel::unlock()
{
   this->locked = false;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                   World                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/****************************************************************************
 *                               Constructor                                *
 ****************************************************************************/
World::World(Ogre::String definitionFile, Ogre::String scoresFile,
      Ogre::String newButtonFileName, Ogre::Overlay* ogreOverlay) :
   Selector(ogreOverlay)
{
   Kobold::DefParser def;
   Ogre::String key, value;

   WorldLevel* curLevel = NULL;

   /* Clean things */
   this->scoresFileName = scoresFile;
   this->ogreOverlay = ogreOverlay;
   this->newButtonFileName = newButtonFileName;

   /* Try to load world definiton's file. */
   if(!def.load(definitionFile))
   {
      return;
   }

   int curIndex = -1;

   while(def.getNextTuple(key, value))
   {
      if(key == TOKEN_TOTAL_LEVELS)
      {
         /* Define the level's vectors. */
         beginCreation(Ogre::StringConverter::parseInt(value));
      }
      else if(key == TOKEN_NAME)
      {
         /* Define world name */
         this->name = value;
      }
      else if(key == TOKEN_LEVEL)
      {
         /* Create and define a new world level */
         curIndex++;
         curLevel = (WorldLevel*) addNextSelectable(value);
         curLevel->setIndex(curIndex);
      }
      else if(key == TOKEN_LEVEL_FILE)
      {
         /* Define current level's name. */
         curLevel->setFileName(value);
      }
      else if(key == TOKEN_NEEDED_STARS)
      {
         /* Some needed stars for current level. Should be defined as a pair
          * of level index needed stars, separated by comma, a single
          * pair per definition.  */
         int levelIndex=0, neededStars=0; 
         sscanf(value.c_str(), "%d,%d", &levelIndex, &neededStars);
         curLevel->setNeededStars(levelIndex, neededStars);
      }
      else if(key == TOKEN_UNLOCK_INFO)
      {
         curLevel->setInfoOnHowToUnlock(Ogre::StringUtil::replaceAll(
                  value, "|", "\n"));
      }
   }
   endCreation();

   /* Try to load scores file */
   if(def.load(Kobold::UserInfo::getUserHome() + scoresFile, true))
   {

      int curLevelIndex = -1;
      while(def.getNextTuple(key, value))
      {
         if(key == TOKEN_LEVEL)
         {
            /* Definition for next level started */
            curLevelIndex++;
            curLevel = (WorldLevel*) getByIndex(curLevelIndex);
         }
         else if(key == TOKEN_LOCKED)
         {
            if(value == TOKEN_FALSE)
            {
               /* Level is unlocked. */
               curLevel->unlock();
            }
         }
         else if(key == TOKEN_TOTAL_STARS)
         {
            curLevel->setScoreStars(Ogre::StringConverter::parseInt(value));
         }
         else if(key == TOKEN_TIME)
         {
            curLevel->setScoreTime(Ogre::StringConverter::parseInt(value));
         }
         else if(key == TOKEN_MOVES)
         {
            curLevel->setScoreMoves(Ogre::StringConverter::parseInt(value));
         }
      }
   }

   /* Define level's buttons. */
   for(int i = 0; i < getTotal(); i++)
   {
      curLevel = (WorldLevel*) getByIndex(i);
      if(curLevel->isLocked())
      {
         curLevel->createButton(BUTTON_LOCKED, "gui", ogreOverlay);
      }
      else 
      {
         switch(curLevel->getScoreStars())
         {
            case 1:
               curLevel->createButton(BUTTON_ONE_STAR, "gui", ogreOverlay);
            break;
            case 2:
               curLevel->createButton(BUTTON_TWO_STARS, "gui", ogreOverlay);
            break;
            case 3:
               curLevel->createButton(BUTTON_THREE_STARS, "gui", ogreOverlay);
            break;
            default:
               curLevel->createButton(this->newButtonFileName, "levels", 
                     ogreOverlay);
            break;
         }
      }
   }
}

/****************************************************************************
 *                               Destructor                                 *
 ****************************************************************************/
World::~World()
{
   clearSelectables();
}

/****************************************************************************
 *                               SaveScores                                 *
 ****************************************************************************/
void World::saveScores()
{
   ofstream file;

   /* Open the file */
   file.open((Kobold::UserInfo::getUserHome() + scoresFileName).c_str(),
         ios::out | ios::binary);
   if(!file)
   {
      return;
   }

   /* Save each level's score. */
   for(int i = 0; i < getTotal(); i++)
   {
      WorldLevel* level = (WorldLevel*) getByIndex(i);
      file << TOKEN_LEVEL << " = " << endl;
      file << TOKEN_LOCKED << " = " << 
         (level->isLocked() ? TOKEN_TRUE : TOKEN_FALSE) << endl;
      file << TOKEN_TOTAL_STARS << " = " << level->getScoreStars() << endl;
      file << TOKEN_TIME << " = " << level->getScoreTime() << endl;
      file << TOKEN_MOVES << " = " << level->getScoreMoves() << endl;
   }

   /* Close it */
   file.close();

   /* Check if need to change a button image */
   checkButtonImages();
}

/****************************************************************************
 *                            checkButtonImages                             *
 ****************************************************************************/
void World::checkButtonImages()
{
   for(int i = 0; i < getTotal(); i++)
   {
      /* Verify if will need to reload button image */
      WorldLevel* level = (WorldLevel*) getByIndex(i);
      if( (level->getScoreStars() == 1) && 
          (level->getButton()->getFileName() != BUTTON_ONE_STAR) ) 
      {
         level->createButton(BUTTON_ONE_STAR, "gui", ogreOverlay);
      }
      else if((level->getScoreStars() == 2) &&
              (level->getButton()->getFileName() != BUTTON_TWO_STARS)) 
      {
         level->createButton(BUTTON_TWO_STARS, "gui", ogreOverlay);
      }
      else if((level->getScoreStars() == 3) &&
              (level->getButton()->getFileName() != BUTTON_THREE_STARS)) 
      {
         level->createButton(BUTTON_THREE_STARS, "gui", ogreOverlay);
      }
      else if((!level->isLocked()) && 
              (level->getButton()->getFileName() == BUTTON_LOCKED))
      {
         level->createButton(this->newButtonFileName, "levels", ogreOverlay);
      }

   }
}

/****************************************************************************
 *                             getSelectedLevel                             *
 ****************************************************************************/
WorldLevel* World::getSelectedLevel()
{
   return (WorldLevel*) getSelected();
}

/****************************************************************************
 *                               getNextLevel                               *
 ****************************************************************************/
WorldLevel* World::getNextLevel(WorldLevel* level)
{
   if((level != NULL) && ((level->getIndex() + 1) < getTotal()))
   {
      WorldLevel* next = (WorldLevel*) getByIndex(level->getIndex() + 1);
      if(!next->isLocked())
      {
         return next;
      }
   }

   return NULL;
}

/****************************************************************************
 *                               tryToUnlock                                *
 ****************************************************************************/
void World::tryToUnlock()
{
   for(int i = 0; i < getTotal(); i++)
   {
      WorldLevel* level = (WorldLevel*) getByIndex(i);
      level->tryToUnlock(this);
   }
}

/****************************************************************************
 *                             createSelectable                             *
 ****************************************************************************/
Selectable* World::createSelectable()
{
   return (Selectable*) new WorldLevel(getTotal());
}

/****************************************************************************
 *                             deleteSelectable                             *
 ****************************************************************************/
void World::deleteSelectable(Selectable* selectable)
{
   WorldLevel* level = (WorldLevel*) selectable;
   delete level;
}

