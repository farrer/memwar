
#include <OGRE/OgreString.h>
#include <OGRE/OgreStringConverter.h>

#include "level.h"
#include "mblock.h"
#include <kobold/ogre3d/ogredefparser.h>
#include <kosound/sound.h>

namespace MemWar
{

#define LEVEL_TOKEN_NUM_BLOCKS           "numBlocks"
#define LEVEL_TOKEN_BLOCK_MESH           "blockMesh"
#define LEVEL_TOKEN_BLOCK_POSITION       "blockPosition"
#define LEVEL_SHOW_NEXT_CHALLENGE_INFO   "showNextChallengeInfo"
#define LEVEL_TOKEN_AVAILABLE_CHALLENGE  "availableChallenge"
#define LEVEL_TOKEN_TIME_TWO_STARS       "timeTwoStars"
#define LEVEL_TOKEN_TIME_THREE_STARS     "timeThreeStars"
#define LEVEL_TOKEN_MOVES_TWO_STARS      "movesTwoStars"
#define LEVEL_TOKEN_MOVES_THREE_STARS    "movesThreeStars"

/***********************************************************************
 *                                Level                                *
 ***********************************************************************/
Level::Level(World* world, WorldLevel* worldLevel, 
      Ogre::SceneManager* ogreSceneManager, Goblin::Model3d* field, 
      Ogre::Overlay* ogreOverlay, bool replaying)
{
   int i,j,r=0;
   Kobold::OgreDefParser def;
   Ogre::String key, value;
   Ogre::String blockMesh = BLOCK_DEFAULT_MESH;
   Ogre::String blockName, materialName;
   MemWar::Block* curBlock = NULL;
   MemWar::Block* block1 = NULL;
   MemWar::Block* block2 = NULL;
   bool gotBlock;
   int blockCount = 0;
   int totalBlocks = 0;
   Ogre::Vector2 posXZ;
   bool availableChallenges[TOTAL_CHALLENGE_TYPES];
   bool usedChallenges[TOTAL_CHALLENGE_TYPES];
   bool curShowInfo = false;
   int curChallenges = 0;
   int totalChallenges = 0;

   bool pairUsed[MAX_PAIRS]; /**< To assign normal pairs to the Level */

   /*! The challenge material list */
   //TODO: should be in challenges.h, not here.
   Ogre::String challengeMaterial[] =
   {
      "bomb",
      "thief",
      "llama",
      "alien",
      "zosima",
      "compass",
      "punk"
   };

   pausedOnlyChallenges = false;

   this->guiOverlay = ogreOverlay;
   this->worldLevel = worldLevel;
   this->field = field;

   /* Defaults */
   timeTwoStars = 1000;
   timeThreeStars = 1000;
   movesTwoStars = 1000;
   movesThreeStars = 1000;

   /* Create the challenges */
   challengeList = new Challenges(this);

   /* Create the timer text */
   int mod = Goblin::ScreenInfo::shouldUseDoubleSizedGui() ? 2 : 1;
   textTime = new Goblin::TextBox(Goblin::ScreenInfo::getHalfWindowWidth() - (20 * mod),
                    15, Goblin::ScreenInfo::getHalfWindowWidth() + (20 * mod),40,
                    "00:00", "textTime", guiOverlay, "infoFont", 16);

   /* Create the button for done/ok */
   okButton = new Goblin::Ibutton(guiOverlay, "initial/ok.png", "gui",
                                  "Ok", "infoFontOut", 16);
   okButton->setPosition(-100, Goblin::ScreenInfo::getHalfWindowHeight() + 130);
   okButton->hide();
   curInfoChallenge = NULL;

   /* Open the Level definition */
   if(!def.load(worldLevel->getFileName(), false, false))
   {
      return;
   }
   sceneManager = ogreSceneManager;

   /* Clear the challenges "to-add" info */
   for(i=0; i < TOTAL_CHALLENGE_TYPES; i++)
   {
      availableChallenges[i] = false;
      usedChallenges[i] = false;
      mustShowInfo[i] = false;
   }

   /* Clear the pairs used */
   for(i=0; i < MAX_PAIRS; i++)
   {
      pairUsed[i] = false;
   }

   /* Parse the Level definition */
   while(def.getNextTuple(key, value))
   {
      if(key == LEVEL_TOKEN_NUM_BLOCKS)
      {
         /* Get total number of blocks */
         totalBlocks = Ogre::StringConverter::parseInt(value);
         if(totalBlocks > (MAX_PAIRS*2))
         {
            printf("totalBlocks > MAX_PAIRS*2!\n");
            return;
         }
      }
      else if(key == LEVEL_TOKEN_BLOCK_MESH)
      {
         /* set the current block mesh */
         blockMesh = value; 
      }
      else if(key == LEVEL_TOKEN_BLOCK_POSITION)
      {
         /* Create new block at the positon */
         blockCount++;
         posXZ = Ogre::StringConverter::parseVector2(value);
         posXZ *= BLOCK_POSITION_DELTA;
         blockName = Ogre::String("block") + 
            Ogre::StringConverter::toString(blockCount);

         curBlock = new Block(blockName, blockMesh, 
               ogreSceneManager, field);
         curBlock->setPosition(Ogre::Vector3(posXZ[0], BLOCK_POS_Y, posXZ[1]));
         insert(curBlock);
      }
      else if(key == LEVEL_SHOW_NEXT_CHALLENGE_INFO) {
         curShowInfo = (value == "true");
      }
      else if(key == LEVEL_TOKEN_AVAILABLE_CHALLENGE)
      {
         /* Find challenge */
         for(i=0; i < TOTAL_CHALLENGE_TYPES; i++)
         {
            if(challengeMaterial[i] == value)
            {
               /* Found it, must set as available */
               if(!availableChallenges[i])
               {
                  availableChallenges[i] = true;
                  mustShowInfo[i] = ((curShowInfo) && (!replaying));
                  curShowInfo = false;
                  totalChallenges++;
               }
               break;
            }
         }
      }
      else if(key == LEVEL_TOKEN_TIME_TWO_STARS)
      {
         timeTwoStars = Ogre::StringConverter::parseInt(value);
      }
      else if(key == LEVEL_TOKEN_TIME_THREE_STARS)
      {
         timeThreeStars = Ogre::StringConverter::parseInt(value);
      }
      else if(key == LEVEL_TOKEN_MOVES_TWO_STARS)
      {
         movesTwoStars = Ogre::StringConverter::parseInt(value);
      }
      else if(key == LEVEL_TOKEN_MOVES_THREE_STARS)
      {
         movesThreeStars = Ogre::StringConverter::parseInt(value);
      }
   }

   /* Set pairs */
   for(i=0; i < (totalBlocks / 2); i++)
   {
      gotBlock = false;

      /* Random a challenge */
      if( (totalChallenges > 0) && (curChallenges < totalChallenges) )
      {
         /* Random to see if challenge applies (2/10 chance) */
         //if(getRandomValue(10) <= 2)
         /* FIXME: always add all available challenges */
         if(true)
         {
            /* Let's get a challenge */
            r = getRandomValue(TOTAL_CHALLENGE_TYPES);
            if( (!availableChallenges[r]) || (usedChallenges[r]) )
            {
               /* Not available, let's get another */
               for(j = 0; (j < TOTAL_CHALLENGE_TYPES) && (!gotBlock); j++)
               {
                  if( (availableChallenges[j]) && (!usedChallenges[j]) )
                  {
                     usedChallenges[j] = true;
                     materialName = challengeMaterial[j];
                     r = j + CHALLENGE_OVERHEAD;
                     gotBlock = true;
                  }
               }
            }
            else
            {
               gotBlock = true;
               usedChallenges[r] = true;
               materialName = challengeMaterial[r];
               r += (CHALLENGE_OVERHEAD);
            }
         }
      }

      if(!gotBlock)
      {
         /* Random a Material */
         r = (int)(MAX_PAIRS * (rand() / (RAND_MAX + 1.0)));
         while(pairUsed[r])
         {
            r = (r+1)%MAX_PAIRS;
         }
         pairUsed[r] = true;
         /* Inc it to the correct value, instead as only the index */
         r++;
         materialName = Ogre::String("block") + 
            Ogre::StringConverter::toString(r);
      }

      /* Now, get a random block */
      block1 = getRandomWithoutMaterial();
      if(!block1)
      {
         /* Must leave without set...
          * printf("Error: none without material!\n");*/
         break;
      }
      block1->setImage(r, materialName);
      
      /* And get its pair! */
      block2 = getRandomWithoutMaterial();
      if(!block2)
      {
         break;
      }
      block2->setImage(r, materialName);

      /* Set pair pointers */
      block1->setPairBlock(block2);
      block2->setPairBlock(block1);
   }

   /* Init the pairs */
   pairsDone = 0;

   /* Set title */
   levelTitle = new Goblin::TextTitle(120, 150, 250, 200, 
         worldLevel->getName(), "levelTitle", "infoFont");
   
   /* Reset the score */
   Score::reset(this, world);

   zoomBlock = NULL;

   state = STATE_PLAYING;
}

/***********************************************************************
 *                               ~Level                                *
 ***********************************************************************/
Level::~Level()
{
   /* bye blocks */
   clearList();

   /* Clear challenges */
   delete challengeList;

   /* Delete texts */
   delete textTime;
   if(levelTitle)
   {
      delete levelTitle;
   }

   /* Delete gui */
   delete okButton;
}

/***********************************************************************
 *                            getRandomvalue                           *
 ***********************************************************************/
int Level::getRandomValue(int max)
{
   return((int)(max * (rand() / (RAND_MAX + 1.0))));
}

/***********************************************************************
 *                               getState                              *
 ***********************************************************************/
int Level::getState() 
{
   return state;
}

/***********************************************************************
 *                          getFieldSceneNode                          *
 ***********************************************************************/
Goblin::Model3d* Level::getField()
{
   return field;
}

/***********************************************************************
 *                           decPairsDone                              *
 ***********************************************************************/
void Level::decPairsDone()
{
   pairsDone--;
}

/***********************************************************************
 *                           setPairsDone                              *
 ***********************************************************************/
void Level::setPairsDone(int value)
{
   pairsDone = value;
}

/***********************************************************************
 *                      getRandomWithoutMaterial                       *
 ***********************************************************************/
Block* Level::getRandomWithoutMaterial()
{
   MemWar::Block* b;
   int i;

   /* Get a random number [0,totalBlocks) */
   int bNumber = (int)(getTotal() * (rand() / (RAND_MAX + 1.0)));

   /* get the block; */
   b = static_cast<MemWar::Block*>(getFirst());
   for(i=0; i < bNumber; i++)
   {
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   /* Verify if it is without material */
   if(b->getImageId() == -1)
   {
      return b;
   }
   /* Otherwise, get first without material */
   return getFirstWithoutMaterial();
}

/***********************************************************************
 *                       getFirstWithoutMaterial                       *
 ***********************************************************************/
Block* Level::getFirstWithoutMaterial()
{
   int i;
   MemWar::Block* b = static_cast<MemWar::Block*>(getFirst());

   /* Search all list */
   for(i=0; i < getTotal(); i++)
   {
      if(b->getImageId() == -1)
      {
         return b;
      }
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   return NULL;
}

/***********************************************************************
 *                             isCompleted                             *
 ***********************************************************************/
bool Level::isCompleted()
{
   return( (pairsDone*2) == getTotal());
}

/***********************************************************************
 *                          isAnimationRunning                         *
 ***********************************************************************/
bool Level::isAnimationRunning()
{
   int i;
   MemWar::Block* b = static_cast<MemWar::Block*>(getFirst());

   /* Verify Block's Animation */
   for(i=0; i < getTotal(); i++)
   {
      if(b->isMoving())
      {
         return true;
      }
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   return false;
}

/***********************************************************************
 *                            getTotalPairs                            *
 ***********************************************************************/
int Level::getTotalPairs()
{
   return (getTotal() / 2);
}

/***********************************************************************
 *                        getMatchedPairsCount                         *
 ***********************************************************************/
int Level::getMatchedPairsCount()
{
   return pairsDone;
}

/***********************************************************************
 *                         getSelectedPair                             *
 ***********************************************************************/
Pair* Level::getSelectedPair()
{
   return &selPair;
}

/***********************************************************************
 *                          clearChallenges                            *
 ***********************************************************************/
void Level::clearChallenges()
{
   challengeList->clear();
}

/***********************************************************************
 *                           getSceneManager                           *
 ***********************************************************************/
Ogre::SceneManager* Level::getSceneManager()
{
   return sceneManager;
}

/***********************************************************************
 *                              getOverlay                             *
 ***********************************************************************/
Ogre::Overlay* Level::getOverlay()
{
   return guiOverlay;
}

/***********************************************************************
 *                           getWorldLevel                             *
 ***********************************************************************/
WorldLevel* Level::getWorldLevel()
{
   return worldLevel;
}

/***********************************************************************
 *                              getBlock                               *
 ***********************************************************************/
Block* Level::getBlock(Ogre::SceneNode* scNode)
{
   int i;
   MemWar::Block* b = static_cast<MemWar::Block*>(getFirst());
   for(i = 0; i < getTotal(); i++)
   {
      if(b->compare(scNode))
      {
         /* Found! */
         return b;
      }
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   /* Not found */
   return NULL;
}

/***********************************************************************
 *                               select                                *
 ***********************************************************************/
void Level::select(Ogre::SceneNode* scNode)
{
   /* Must wait title */
   if(levelTitle)
   {
      return;
   }

   /* Get the block related to the scene node */
   MemWar::Block* b = getBlock(scNode);

   if(!b)
   {
      /* Not a block, so quit */
      return;
   }

   /* Verify if can flip it */
   if( (!b->isFaceUp()) && (!b->isMoving()) && (b->isVisible()) &&
       (!b->isMatched()) &&
       ((selPair.getBlock1() == NULL) || selPair.getBlock2() == NULL))
   {
      /* Face it up */
      if(b->setFaceUp())
      {
         Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "tap1.ogg");
         if(selPair.getBlock1() == NULL)
         {
            selPair.setBlock1(b);
            challengeList->firstBlockSelected(b);
         }
         else if(selPair.getBlock2() == NULL)
         {
            selPair.setBlock2(b);
         }

         /* Verify challenge */
         if(challengeList->isChallenge(b->getImageId()))
         {
            /* Add new challenge! */
            Challenge* c = challengeList->add(b->getImageId());
            /* Show challenge card, if needed */
            if (mustShowInfo[b->getImageId() - CHALLENGE_OVERHEAD])
            {
               int mod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
               c->setDisplayingInfo(true);
               curInfoChallenge = c;

               /* Show the challenge info and ok button */
               Goblin::Image* curInfoImage = 
                  curInfoChallenge->getIcon()->getCard();
               curInfoImage->show();
               curInfoImage->setTargetPosition(
                     Goblin::ScreenInfo::getHalfWindowWidth() - (128 * mod),
                     Goblin::ScreenInfo::getHalfWindowHeight() - (128 * mod), 20);
               okButton->setTargetPosition(
                     Goblin::ScreenInfo::getHalfWindowWidth() - (32 * mod),
                     Goblin::ScreenInfo::getHalfWindowHeight() + (130 * mod), 20);
               okButton->show();
               mustShowInfo[b->getImageId() - CHALLENGE_OVERHEAD] = false;

               /* Pause game (and challenges) and change our state. */
               pauseTimer();
               state = STATE_SHOWING_INFO;
            }
         }
         
         /* tell statistics */
         Score::flippedBlock();
      }
   }
   /* Verify if can Zoom on it */
   else if( (b->isFaceUp()) && (!b->isMoving()) && (!b->isFlipping()) &&
            (b->isVisible()) )
   {
      unsetZoomBlock();
      /* Set Zoom */
      zoomBlock = b;
      zoomBlock->setZoom();
   }
}

/***********************************************************************
 *                             unselect                                *
 ***********************************************************************/
void Level::unselect()
{
   if(selPair.getBlock1())
   {
      if(selPair.getBlock1()->isFaceUp())
      {
         selPair.getBlock1()->setFaceDownAfterTimer();
      }
   }
   if(selPair.getBlock2())
   {
      if(selPair.getBlock2()->isFaceUp())
      {
         selPair.getBlock2()->setFaceDownAfterTimer();
      }
   }
   selPair.clear();
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
void Level::update(bool updateSpecials, int mouseX, int mouseY, 
                   bool leftButtonPressed)
{
   int i;
   MemWar::Block* b = static_cast<MemWar::Block*>(getFirst());

   if (state == STATE_SHOWING_INFO)
   {
      if (okButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         /* Set image and button to 'hide-position' */
         curInfoChallenge->getIcon()->getCard()->setTargetPosition(-200, 
               Goblin::ScreenInfo::getHalfWindowHeight() - 128);
         okButton->setTargetPosition(-100,
               Goblin::ScreenInfo::getHalfWindowHeight() + 130);
      }

      if ((okButton->getPosX() == -100) && (!okButton->isUpdating()))
      {
         /* At hide position, must resume game. */
         curInfoChallenge->setDisplayingInfo(false);
         resumeTimer();
         state = STATE_PLAYING;
         curInfoChallenge->getIcon()->getCard()->hide();
         okButton->hide();
         curInfoChallenge = NULL;
      }
   }

   /* Verify Level title */
   if(levelTitle)
   {
      levelTitle->update();
      if(levelTitle->isDone())
      {
         delete levelTitle;
         levelTimer.reset();
         levelTitle = NULL;
      }
      return;
   }

   /* Verify pairs */
   if((selPair.getBlock1()) && (selPair.getBlock2()))
   {
      if(selPair.getBlock1()->getImageId() == selPair.getBlock2()->getImageId())
      {
         /* Match! */
         pairsDone++;
         selPair.getBlock1()->setMatched();
         selPair.getBlock2()->setMatched();

         /* Tell challenges and specials */
         challengeList->pairMatched(&selPair);
         Specials::pairMatched(&selPair);

         /* Add Points to the specials counter */
         if(challengeList->isChallenge(selPair.getBlock1()->getImageId()))
         {
            Specials::addPoints(CHALLENGE_POINTS);
         }
         else
         {
            Specials::addPoints(NORMAL_POINTS);
         }

         /* Done */
         selPair.clear();
      }
      else 
      {
         /* Not matched */

         /* Tell challenges */
         challengeList->pairNotMatched(&selPair);
         Specials::pairNotMatched(&selPair);
         unselect();
      }
   }

   /* Update all blocks */
   for(i=0; i < getTotal(); i++)
   {
      b->update();
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   /* Update challenges */
   if(!isAnimationRunning())
   {
      challengeList->update();
      if(updateSpecials)
      {
         Specials::update();
      }
   }

   /* Update disp text */
   updateTimeText();
   unsetZoomBlock();
}

/***********************************************************************
 *                           unsetZoomBlock                            *
 ***********************************************************************/
void Level::unsetZoomBlock()
{
   if(zoomBlock)
   {
      zoomBlock->unsetZoom();
      zoomBlock = NULL;
   }
}

/***********************************************************************
 *                              shuffle                                *
 ***********************************************************************/
void Level::updateTimeText()
{
   int min, sec;
   unsigned long msec = levelTimer.getMilliseconds();
   Ogre::String txt;

   sec = (int) (msec / 1000) % 60;
   min = (int) (msec / 1000) / 60;

   txt = ((min < 10)?Ogre::String("0"):Ogre::String("")) + 
      Ogre::StringConverter::toString(min) + Ogre::String(":") + 
      ((sec < 10)?Ogre::String("0"):Ogre::String("")) + 
      Ogre::StringConverter::toString(sec);

   textTime->setText(txt);
   
   /* Update the score info */
   Score::setTotalTime(sec+(min*60));
}

/***********************************************************************
 *                              hideUI                                 *
 ***********************************************************************/
void Level::hideUI()
{
   if(textTime != NULL)
   {
      textTime->hide();
   }
}

/***********************************************************************
 *                              shuffle                                *
 ***********************************************************************/
void Level::shuffle()
{
   Ogre::Vector2* positions; /**< All positions defined for curLevel */
   bool* used;               /**< If the position is used or not at the 
                               new definition */
   int i, r;
   Block* b;
   Ogre::Vector3 curPos;

   /* alloc vectors */
   positions = new Ogre::Vector2[getTotal()];
   used = new bool[getTotal()];

   /* Populate with all positions */
   b = static_cast<MemWar::Block*>(getFirst());
   for(i=0; i < getTotal(); i++)
   {
      used[i] = false;
      /* Set position (only using x and z axis) */
      curPos = b->getPosition();
      positions[i][0] = curPos[0];
      positions[i][1] = curPos[2];

      b = static_cast<MemWar::Block*>(b->getNext());
   }

   /* Now, let's randomize the positions! */
   b = static_cast<MemWar::Block*>(getFirst());
   for(i=0; i < getTotal(); i++)
   {
      r = (int)(getTotal() * (rand() / (RAND_MAX + 1.0)));
      while(used[r])
      {
         /* pos used, let's get the next avaiable */
         r = (r+1) % getTotal();
      }

      /* Set the new position */
      used[r] = true;
      b->setTargetPosition(positions[r][0], BLOCK_POS_Y, positions[r][1]);

      /* and get next one! */
      b = static_cast<MemWar::Block*>(b->getNext());
   }

   /* dealloc vectors */
   delete[] positions;
   delete[] used;

}

/***********************************************************************
 *                             pauseTimer                              *
 ***********************************************************************/
void Level::pauseTimer()
{
   levelTimer.pause();
   challengeList->pauseTimer();
}

/***********************************************************************
 *                            resumeTimer                              *
 ***********************************************************************/
void Level::resumeTimer()
{
   levelTimer.resume();
   if (!pausedOnlyChallenges) 
   {
      challengeList->resumeTimer();
   }
}

/***********************************************************************
 *                             pauseTimer                              *
 ***********************************************************************/
void Level::pauseChallengeTimers()
{
   challengeList->pauseTimer();
   pausedOnlyChallenges = true;
}

/***********************************************************************
 *                            resumeTimer                              *
 ***********************************************************************/
void Level::resumeChallengeTimers()
{
   challengeList->resumeTimer();
   pausedOnlyChallenges = false;
}

/***********************************************************************
 *                         getTimeTwoStars                             *
 ***********************************************************************/
int Level::getTimeTwoStars()
{
   return timeTwoStars;
}

/***********************************************************************
 *                        getTimeThreeStars                            *
 ***********************************************************************/
int Level::getTimeThreeStars()
{
   return timeThreeStars;
}

/***********************************************************************
 *                        getMovesTwoStars                             *
 ***********************************************************************/
int Level::getMovesTwoStars()
{
   return movesTwoStars;
}



/***********************************************************************
 *                       getMovesThreeStars                            *
 ***********************************************************************/
int Level::getMovesThreeStars()
{
   return movesThreeStars;
}


}

