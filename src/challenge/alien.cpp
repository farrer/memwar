
#include "alien.h"

#include "../game/mblock.h"
#include "../game/level.h"
#include "../game/pair.h"

#include <kosound/sound.h>
#include <kobold/ogre3d/ogrefilereader.h>

namespace MemWar
{

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Alien::Alien(Level* l):Challenge(CHALLENGE_ALIEN, l)
{
   int i;

   succeeded = false;
   failed = false;

   /* Nullify currently unused */
   for(i=0; i < ALIEN_ABDUCT_PAIRS*2; i++)
   {
      alienBlock[i] = NULL;
   }

   /* Verify current number of pairs. if not sufficient, must 
    * invalidate the challenge */
   if(curLevel->getTotalPairs() < ALIEN_ABDUCT_PAIRS)
   {
      active = false;
   }
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Alien::~Alien()
{
   /* Note: no need to delete alien blocks here,
    * as if they were created, they are attached
    * to the level itself, and will be deleted when
    * the level ends. */
}

/***********************************************************************
 *                         getRemainingTime                            *
 ***********************************************************************/
int Alien::getRemainingTime() 
{
   return -1;
}

/***********************************************************************
 *                                update                               *
 ***********************************************************************/
void Alien::update()
{
   if(succeeded)
   {
      /* Verify if level is finished (note: pairs-5) */
      if(curLevel->getMatchedPairsCount() + 5 == curLevel->getTotalPairs())
      {
         /* set the level as the remaining 5 pairs! */
         doAfterLevel();
         active = false;
      }
   }
   else if((failed) && (!displayingInfo))
   {
      /* Must apply the penalty and quit */
      applyPenalty();
      active = false;
   }
}

/***********************************************************************
 *                           pairNotMatched                            *
 ***********************************************************************/
void Alien::pairNotMatched(Pair* p)
{
   if(!succeeded)
   {
      failed = true;
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "alien1.ogg",
            new Kobold::OgreFileReader());
   }
}

/***********************************************************************
 *                          firstBlockSelected                         *
 ***********************************************************************/
void Alien::firstBlockSelected(Block* b)
{
   /* If not succeeded and here: failed to find the Alien */
   if(!succeeded)
   {
      failed = true;
   }
}

/***********************************************************************
 *                             pairMatched                             *
 ***********************************************************************/
void Alien::pairMatched(Pair* p)
{
   /* Verify if we got the Alien pair 
    * (and not already got, to avoid double abduction) */
   if( (!succeeded) && (p->getBlock1()->getImageId() == CHALLENGE_ALIEN) )
   {
      /* Got it, so must abduct the blocks */
      abduct();
      succeeded = true;
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "alien3.ogg",
            new Kobold::OgreFileReader());
   }
}

/***********************************************************************
 *                               abduct                                *
 ***********************************************************************/
void Alien::abduct()
{
   MemWar::Block* b = (MemWar::Block*)curLevel->getFirst();
   int i, totalBlocks = curLevel->getTotal();
   int numPairs=0;

   /* Verify current number of pairs. if not sufficient, must 
    * invalidate the challenge */
   if(curLevel->getTotalPairs() - curLevel->getMatchedPairsCount() < 
         ALIEN_ABDUCT_PAIRS)
   {
      active = false;
      return;
   }

   /* Get all needed pairs to abduct */
   for(i=0; ((i < totalBlocks) && (numPairs < ALIEN_ABDUCT_PAIRS)); i++)
   {
      if( (!b->isMatched()) && (!inPair(b, abductedPairs, numPairs)) )
      {
         /* Abduct the block and its pair */
         abductedPairs[numPairs].setBlock1(b);
         abductedPairs[numPairs].setBlock2(b->getPairBlock());
         numPairs++;

         /* And stop rendering them! */
         b->hide();
         b->getPairBlock()->hide();
      }
      b = (MemWar::Block*)b->getNext();
   }
}

/***********************************************************************
 *                            applyPenalty                             *
 ***********************************************************************/
void Alien::applyPenalty()
{
   curLevel->shuffle();
}

/***********************************************************************
 *                                unApply                              *
 ***********************************************************************/
void Alien::unApply()
{
   int i;

   /* Take back the abducted blocks */
   if(succeeded)
   {
      for(i=0; i < ALIEN_ABDUCT_PAIRS; i++)
      {
         abductedPairs[i].getBlock1()->show();
         abductedPairs[i].getBlock2()->show();
      }
   }
}

/***********************************************************************
 *                           doAfterLevel                              *
 ***********************************************************************/
void Alien::doAfterLevel()
{
   Ogre::String blockName;
   Ogre::String blockMaterial;
   MemWar::Block* b = (MemWar::Block*)curLevel->getFirst();
   MemWar::Block* baux;
   int b1, b2;
   int i, totalBlocks = curLevel->getTotal();
   Ogre::SceneManager* sceneManager = curLevel->getSceneManager();

   /* Remove from level all done pairs */
   for(i=0; (i < totalBlocks); i++)
   {
      if( (b->isMatched()) && (!inPair(b, abductedPairs, ALIEN_ABDUCT_PAIRS)) )
      {
         baux = b;
         b = (MemWar::Block*)b->getNext();
         curLevel->remove(baux);
      }
      else 
      {
         b = (MemWar::Block*)b->getNext();
      }
   }

   /* Make AlienBlocks at abtucted pairs positions */
   curLevel->setPairsDone(0);
   for(i=0; i < ALIEN_ABDUCT_PAIRS; i++)
   {
      b1 = i*2;
      b2 = (i*2)+1;

      blockMaterial = Ogre::String("blockAlienMaterial") + 
         Ogre::StringConverter::toString(i+1);

      /* Create the block 1 */
      blockName = Ogre::String("AlienBck")+Ogre::StringConverter::toString(b1);
      alienBlock[b1] = new Block(blockName,ALIEN_BLOCK_MESH,sceneManager,
            curLevel->getField());
      alienBlock[b1]->setPositionNow(
            abductedPairs[i].getBlock1()->getPosition());
      alienBlock[b1]->setImage(i, blockMaterial);
      
      /* Create the block 2 */
      blockName = Ogre::String("AlienBck")+Ogre::StringConverter::toString(b2);
      alienBlock[b2] = new Block(blockName,ALIEN_BLOCK_MESH,sceneManager,
            curLevel->getField());
      alienBlock[b2]->setPositionNow(
            abductedPairs[i].getBlock2()->getPosition());
      alienBlock[b2]->setImage(i, blockMaterial);
      
      /* Set as pairs */
      alienBlock[b1]->setPairBlock(alienBlock[b2]);
      alienBlock[b2]->setPairBlock(alienBlock[b1]);

      /* Remove the abtucted one */
      curLevel->remove(abductedPairs[i].getBlock1());
      curLevel->remove(abductedPairs[i].getBlock2());

      /* Insert the new Alien block on list */
      curLevel->insert(alienBlock[b1]);
      curLevel->insert(alienBlock[b2]);
   }
}

}

