#ifndef _memwar_ocarina_h
#define _memwar_ocarina_h

#include <kobold/timer.h>

#include "special.h"

namespace MemWar
{

#define OCARINA_STOP_TIME   30000 /**< 30 seconds */

/*! Ocarina special: Stop all active challenge timers for 30 seconds. */
class Ocarina: public Special
{
   public:
      /*! Constructor */
      Ocarina();
      /*! Destructor */
      ~Ocarina();

      /*! Update the ocarina special state */
      void update();
      /*! When selected the first block */
      void firstBlockSelected(Block* b);
      /*! When selected two not matched blocks */
      void pairNotMatched(Pair* p);
      /*! When selected a pair of two blocks */
      void pairMatched(Pair* p);
      /*! Pause special timer */
      void pauseTimer();
      /*! Resume special timer */
      void resumeTimer();

   protected:
      bool paused;             /**< If paused all game timers */
      Kobold::Timer stopTimer; /**< Timer to count stop time */

};

}

#endif

