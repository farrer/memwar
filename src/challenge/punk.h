#ifndef _memwar_punk_h
#define _memwar_punk_h

#include <OGRE/OgreTimer.h>

#include "challenge.h"

namespace MemWar
{

#define PUNK_MATCH_TIME   12000 /**< 12 seconds */
#define PUNK_MAX_TIME     30000 /**< 30 seconds */

/*! Punk challenge block: If fail to do the pair in PUNK_MATCH_TIME, every 
 * PUNK_MAX_TIME the board will be shuffled. */
class Punk: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Punk(Level* l);
      /*! Destructor */
      ~Punk();

      /*! Update the punk challenge state */
      void update();

      /*! When selected the first block */
      void firstBlockSelected(Block* b);
      /*! When selected two not matched blocks */
      void pairNotMatched(Pair* p);
      /*! When selected a pair of two blocks */
      void pairMatched(Pair* p);
      /*! When removed from list */
      void unApply(){};

      /*! Pause the bomb timer */
      void pauseTimer();
      /*! Resume the bomb timer */
      void resumeTimer();

      /*! @return remaining time to end */
      int getRemainingTime();

   protected:
      Kobold::Timer osTimer;  /**< The punk timer */
      int remaining;          /**< seconds before end */
      int curMax;             /**< current maximun in question */

};

}

#endif

