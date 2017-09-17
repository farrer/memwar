#ifndef _memwar_quantum_h
#define _memwar_quantum_h

#include "special.h"

namespace MemWar
{

/*! Wind special: Remove all active challenges when used. */
class Quantum: public Special
{
   public:
      /*! Constructor */
      Quantum();
      /*! Destructor */
      ~Quantum();

      /*! Update the quantum special state */
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
      /*! Apply the quantum effect */
      void apply();

};

}

#endif

