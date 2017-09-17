#ifndef _memwar_compass_h
#define _memwar_compass_h

#include <OGRE/OgreTimer.h>

#include "challenge.h"

namespace MemWar
{

/*! Compass challenge block: If the user fail to do the pair, it will rotate
 * the camera to be down-up. If user do the pair, the camera will be back to
 * normal. */
class Compass: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Compass(Level* l);
      /*! Destructor */
      ~Compass();

      /*! Update the compass challenge state */
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

      /*! @return < 0 as compass is not time affected. */
      int getRemainingTime();

   protected:
      bool failed;  /**< if failed the challenge */
};

}

#endif

