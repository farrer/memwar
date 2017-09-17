#ifndef _memwar_bomb_h
#define _memwar_bomb_h

#include "challenge.h"

namespace MemWar
{

#define BOMB_MAX_TIME 5000 /**< 5 seconds */

/*! Bomb challenge block: The user will have 5 seconds to do the bomb pair.
 * If fail, all pieces will have its positions changed on the field. */
class Bomb: public Challenge
{
   public:
      /*! Constructor
       * \param l -> pointer to the current level */
      Bomb(Level* l);
      /*! Destructor */
      ~Bomb();

      /*! Update the bomb challenge state */
      void update();
      /*! When selected the first block */
      void firstBlockSelected(Block* b);
      /*! When selected two not matched blocks */
      void pairNotMatched(Pair* p);
      /*! When selected a pair of two blocks */
      void pairMatched(Pair* p);
      /*! When removed from list */
      void unApply();

      /*! Pause the bomb timer */
      void pauseTimer();
      /*! Resume the bomb timer */
      void resumeTimer();

      /*! @return remaining time to explode*/
      int getRemainingTime();

   protected:
      /*! Apply the bomb to the level, after failed */
      void apply();

      Kobold::Timer bombTimer;  /**< The bomb timer */

      int remaining; /**< Remaining time to explode*/

};

}

#endif

