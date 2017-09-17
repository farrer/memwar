
#ifndef _memwar_pair_h
#define _memwar_pair_h

#include "mblock.h"

#include <kobold/timer.h>

namespace MemWar
{

/*! The pair class represents two blocks */
class Pair
{
   public:
      /*! Default constructor */
      Pair();
      /*! Constructor
       * \param b1 -> pointer to first block
       * \param b2 -> pointer to the second block */
      Pair(Block* b1, Block* b2);

      /*! Destructor */
      ~Pair();

      /*! Clear the pointers */
      void clear();

      void setBlock1(Block* b);
      void setBlock2(Block*b);

      Block* getBlock1();
      Block* getBlock2();

   protected:

      Block* block1; /**< First block */
      Block* block2; /**< Second block */
};

}

#endif

