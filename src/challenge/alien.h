#ifndef _memwar_alien_h
#define _memwar_alien_h

#include <OGRE/OgreTimer.h>
#include <goblin/camera.h>

#include "challenge.h"
#include "../game/pair.h"


namespace MemWar
{

#define ALIEN_ABDUCT_PAIRS   5   /**< Number of pairs the alien will abduct */
#define ALIEN_BLOCK_MESH  "block/block_alien.mesh"

/*! Alien challenge block: If the user fail to do the pair, the level is
 * shuffled. Otherwise, if done the pair, 5 not matched pairs
 * are removed from the level, until the user finished it. When the level is
 * finished, the user will have to end with 5 ALIEN-style blocks. */
class Alien: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Alien(Level* l);
      /*! Destructor */
      ~Alien();

      /*! Update the alien challenge state */
      void update();
      /*! When selected the first block */
      void firstBlockSelected(Block* b);
      /*! When selected two not matched blocks */
      void pairNotMatched(Pair* p);
      /*! When selected a pair of two blocks */
      void pairMatched(Pair* p);
      /*! When removed from list */
      void unApply();

      /*! Not used */
      void pauseTimer(){};
      /*! Not used */
      void resumeTimer(){};

      /*! @return < 0 : alien isn't time affected */
      int getRemainingTime();

   protected:

      /*! Do the pairs abduct */
      void abduct();

      /*! Apply the penalty when failed */
      void applyPenalty();

      /*! Do the "mini-level" with all remainder pairs. */
      void doAfterLevel();

      bool succeeded;  /**< if failed the challenge */
      bool failed;     /**< if failed the challenge */

      Pair abductedPairs[ALIEN_ABDUCT_PAIRS];  /**< Pairs abducted */

      Block* alienBlock[ALIEN_ABDUCT_PAIRS*2]; /**< Alien blocks after match */

};

}

#endif

