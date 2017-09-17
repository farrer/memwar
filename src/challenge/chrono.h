#ifndef _memwar_chrono_h
#define _memwar_chrono_h

#include <OGRE/OgreTimer.h>

#include "challenge.h"

namespace MemWar
{

#define CHRONO_MAX_TIME   30000 /**< 30 seconds */
#define CHRONO_NUM_PAIRS      3 /**< 3 pairs */

/*! Chronometer challenge block: If the user fail to do the pair, it will have
 * CHRONO_MAX_TIME seconds to do CHRONO_NUM_PAIRS. If fail, CHRONO_NUM_PAIRS
 * of current done pairs are flipped back and have their position changed. */ 
class Chrono: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Chrono(Level* l);
      /*! Destructor */
      ~Chrono();

      /*! Update the chrono challenge state */
      void update();
      /*! When selected the first block */
      void firstBlockSelected(Block* b);
      /*! When selected two not matched blocks */
      void pairNotMatched(Pair* p);
      /*! When selected a pair of two blocks */
      void pairMatched(Pair* p);
      /*! When removed from list */
      void unApply(){};

      /*! Pause the timer */
      void pauseTimer();
      /*! Resume the timer */
      void resumeTimer();

      /*! @return remaining time to end */
      int getRemainingTime();

   protected:
      /*! Apply the chrono to the level, after failed */
      void apply();
      
      int numPairs;       /**< Number of pairs done */
      Pair* curPairs;     /**< Current done pairs */
      bool failPair;      /**< If failed to get the chronometer pair */
      Kobold::Timer chronoTimer;  /**< The chrono timer */
      int remaining;      /**< Remaining time to end */

};

}

#endif

