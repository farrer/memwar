
#ifndef _memwar_Block_h
#define _memwar_Block_h

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreTimer.h>

#include <kobold/list.h>
#include <kobold/timer.h>
#include <goblin/model3d.h>

namespace MemWar
{

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS ||\
    OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
   #define ANGLE_DELTA    10.0f
   #define BLOCK_NEXT_POSITION_FRAMES 40
#else
   #define ANGLE_DELTA    20.0f   /**< delta on Block rotations */
   #define BLOCK_NEXT_POSITION_FRAMES   20 /**< Number of frames to next pos */
#endif

#define BLOCK_DEFAULT_MESH "Block/Block.mesh" /**< default model to use */

#define BLOCK_FLIP_TIMER   500 /**< 0.5s */

#define BLOCK_POSITION_DELTA   0.8f

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
   #define BLOCK_POS_Y 1.5f
#else
   #define BLOCK_POS_Y 0.75f
#endif



/*! The Block class represents a simple turnable Block on memory game */
class Block: public Kobold::ListElement, public Goblin::Model3d
{
   public:
      /*! Constructor
       * \param name -> Block unique name
       * \param fileName -> fileName of the model used
       * \param oSceneManager -> pointer to the scene manager */
      Block(Ogre::String name, Ogre::String fileName, 
             Ogre::SceneManager* oSceneManager,
             Goblin::Model3d* field);
      /*! Destructor */
      ~Block();

      /*! Get the Block image type */
      int getImageId();

      /*! Set the Block as currenctly matched */
      void setMatched();
      /*! Set the Block as unmatched */
      void unsetMatched();
      /*! Get if the Block is currenctly matched or not
       * \return true if matched */
      bool isMatched();

      /*! Return true if changing position */
      bool changingPos();

      /*! Compare the Block's scene node with scNode
       * \param scNode -> scene node to compare object with
       * \return true if scNode belongs to object */
      bool compare(Ogre::SceneNode* scNode);

      /*! Update a Block (its turnable angle etc) */
      bool update();

      /*! \return if the block can flip. */
      bool canFlip();

      /*! Set the block to be facing up.
       * \return if could set. */
      bool setFaceUp();

      /*! Set the block to be facing down
       * \return if could set. */
      bool setFaceDown();

      /*! Set the block to face down after time */
      void setFaceDownAfterTimer();

      /*! Face the block down, without any animations. */
      void setFaceDownWithoutAnimation();

      /*! Verify if is currently flipping or not
       * \return true if flipping */
      bool isFlipping();

      /*! Verify if the Block is with the face turned up or down
       * \return true if the face is turned up */
      bool isFaceUp();

      /*! Set the Block image
       * \param id -> image id constant
       * \param material -> image material name */
      void setImage(int id, Ogre::String material);

      /*! Get the Block that makes a pair with this one
       * \return -> pointer to the Block */
      Block* getPairBlock();
      /*! Set the Block that makes a pair with this one
       * \param b -> pointer to the Block */
      void setPairBlock(Block* b);

      /*! Verify if a Block is moving on Y=0 plane */
      bool isMoving();

      /*! Set to zoom into the Block */
      void setZoom();

      /*! Unset the zoom into the Block */
      void unsetZoom();

      /*! Return the Block scene node
       * \return pointer to the scene node */
      Ogre::SceneNode* getSceneNode(){return(node);};

   protected:   
      int imageId;                     /**< The image id */
      bool matched;                    /**< If Block is already matched */
      bool faceup;                     /**< If face is up or not */

      Kobold::Timer flipTimer;         /**< Timer to flip */
      bool faceDownAtTimer;            /**< Toggle timer flip */

      Block* pairBlock; /**< Pointer to the Block paired with this one */
};


}

#endif

