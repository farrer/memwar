
#include "mblock.h"
#include <kosound/sound.h>
#include <kobold/ogre3d/ogrefilereader.h>

namespace MemWar
{

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
Block::Block(Ogre::String name, Ogre::String fileName, 
             Ogre::SceneManager* oSceneManager,
             Goblin::Model3d* field)
      :Goblin::Model3d(name, fileName, "models", oSceneManager, 
            Goblin::Model3d::MODEL_DYNAMIC, field)
{
   /* Set initial sceneNode options */
   node->setInheritScale(false);
   setPositionNow(0.0f, 0.0f, 0.0f);
   setScaleNow(0.52f, 0.52f, 0.52f);
   
   setOrientationNow(0.0f, 0.0f, 180.0f);

   /* Initial values */
   matched = false;
   imageId = -1;
   pairBlock = NULL;
   faceup = false;
   visible = true;
   faceDownAtTimer = false;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
Block::~Block()
{
}

/***********************************************************************
 *                             getImageId                              *
 ***********************************************************************/
int Block::getImageId()
{
   return(imageId);
}

/***********************************************************************
 *                              setImage                               *
 ***********************************************************************/
void Block::setImage(int id, Ogre::String material)
{
   /* change the material to the desired one */
   imageId = id;
   model->setMaterialName(material);
}

/***********************************************************************
 *                             setMatched                              *
 ***********************************************************************/
void Block::setMatched()
{
   matched = true;
}

/***********************************************************************
 *                            unsetMatched                             *
 ***********************************************************************/
void Block::unsetMatched()
{
   matched = false;
}

/***********************************************************************
 *                              isMatched                              *
 ***********************************************************************/
bool Block::isMatched()
{
   return(matched);
}

/***********************************************************************
 *                            changingPos                              *
 ***********************************************************************/
bool Block::changingPos()
{
   return(pos[0].needUpdate() || pos[1].needUpdate() ||
          pos[2].needUpdate());
}

/***********************************************************************
 *                               setZoom                               *
 ***********************************************************************/
void Block::setZoom()
{
   /* FIXME: !! */
   //node->setPosition(Ogre::Vector3(position.x, 2 * BLOCK_POS_Y, position.z));
   //node->setScale(16, 10, 16);
}

/***********************************************************************
 *                              unsetZoom                              *
 ***********************************************************************/
void Block::unsetZoom()
{
   //sceneNode->setPosition(Ogre::Vector3(position.x, BLOCK_POS_Y, position.z));
   //sceneNode->setScale(10, 10, 10);
}

/***********************************************************************
 *                             getPairBlock                            *
 ***********************************************************************/
Block* Block::getPairBlock()
{
   return(pairBlock);
}

/***********************************************************************
 *                             setPairBlock                            *
 ***********************************************************************/
void Block::setPairBlock(Block* b)
{
   pairBlock = b;
}

/***********************************************************************
 *                               compare                               *
 ***********************************************************************/
bool Block::compare(Ogre::SceneNode* scNode)
{
   return(scNode == node);
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
bool Block::update()
{
   /* Verify timer flipps */
   if( (faceDownAtTimer) && (flipTimer.getMilliseconds() >= BLOCK_FLIP_TIMER) )
   {
      /* Face it down */
      faceDownAtTimer = false;
      setFaceDown();
   }

   return Goblin::Model3d::update();
}

/***********************************************************************
 *                            canFlip                                  *
 ***********************************************************************/
bool Block::canFlip() 
{
   if( (isFlipping()) || (isMoving()) || (faceDownAtTimer) )
   {
      return false;
   }

   return true;
}


/***********************************************************************
 *                           setFaceDown                               *
 ***********************************************************************/
bool Block::setFaceDown()
{
   if (faceup) {
      setTargetOrientation(0.0f, 0.0f, 180.0f);
      Kosound::Sound::addSoundEffect(SOUND_NO_LOOP, "tap1.ogg",
            new Kobold::OgreFileReader());
   }
   faceup = false;
   faceDownAtTimer = false;

   return true;
}

/***********************************************************************
 *                            setFaceUp                                *
 ***********************************************************************/
bool Block::setFaceUp()
{
   if (!faceup)
   {
      setTargetOrientation(0.0f, 0.0f, 0.0f);
   }
   faceup = true;
   faceDownAtTimer = false;

   return true;
}

/***********************************************************************
 *                         flipAfterTimer                              *
 ***********************************************************************/
void Block::setFaceDownAfterTimer()
{
   faceDownAtTimer = true;
   flipTimer.reset();
}

/***********************************************************************
 *                     faceDownWithoutAnimation                        *
 ***********************************************************************/
void Block::setFaceDownWithoutAnimation()
{
   setOrientation(0.0f, 0.0f, 180.0f);
   faceup = false;
}

/***********************************************************************
 *                            isFlipping                               *
 ***********************************************************************/
bool Block::isFlipping()
{
   /* Verify if changing orientation */
   return ori[2].needUpdate();
}

/***********************************************************************
 *                              isFaceUp                               *
 ***********************************************************************/
bool Block::isFaceUp()
{
   return faceup;
}

/***********************************************************************
 *                              isMoving                               *
 ***********************************************************************/
bool Block::isMoving()
{
   return changingPos();
}

}

