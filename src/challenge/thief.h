#ifndef _memwar_thief_h
#define _memwar_thief_h

#include <OGRE/OgreTimer.h>
#include "challenge.h"

namespace MemWar
{

#define THIEF_MAX_TIME    5000 /**< 5 seconds */
#define THIEF_NUM_PAIRS      3 /**< 3 pairs */

/*! Thief challenge block: If the user fail to do the thief pair in 
 * THIEF_MAX_TIME seconds, it will have THIEF_NUM_PAIRS flipped and 
 * changed position. */
class Thief: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Thief(Level* l);
      /*! Destructor */
      ~Thief();

      /*! Update the thief challenge state */
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

      /*! @return remaining time to steal */
      int getRemainingTime();

   protected:
      /*! Apply the thief to the level, after failed */
      void apply();
      
      Kobold::Timer tTimer;  /**< The thief timer */
      int remaining;      /**< Remaining time to steal */

};

}

#endif

