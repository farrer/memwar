#ifndef _memwar_challenge_info_h
#define _memwar_challenge_info_h

#include <OGRE/OgreRenderWindow.h>

#include <goblin/image.h>
#include <goblin/textbox.h>

namespace MemWar
{

class Challenge;

/*! The challenge info class display the challenge icon on screen,
 * and, if applicable, the time to it take effect. */
class ChallengeInfo: public Goblin::Image
{
   public:
      /*! Constructor
       * \param ogreOverlay -> ogre overlay used
       * \param x -> x 'left' coordinate
       * \param y -> y 'up' coordinate
       * \param imageFileName -> file name of the challenge image
       * \param useTime -> if will set remaining time or not */
      ChallengeInfo(Ogre::Overlay* ogreOverlay, Ogre::Real x, Ogre::Real y, 
            Ogre::String imageFileName, 
            bool useTime, Challenge* challenge);

      /*! Destructor */
      ~ChallengeInfo();

      /*! Update the challenge info */
      void update();

      /*! If is currentely updating its position */
      bool isUpdatingPosition();

      /*! \return image card of the challenge */
      Goblin::Image* getCard();

   protected:
      Goblin::TextBox* remainingText; /**< text with remaining time */
      Challenge* challenge; /**< Related challenge */
      Goblin::Image* card; /**< Challenge info card*/
};

}

#endif

