#ifndef _memwar_wind_h
#define _memwar_wind_h

#include "special.h"

namespace MemWar
{

/*! Wind special: Remove all active challenges when used. */
class Wind: public Special
{
   public:
      /*! Constructor */
      Wind();
      /*! Destructor */
      ~Wind();

      /*! Update the wind special state */
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

};

}

#endif

