
#include "challengeinfo.h"
#include "challenge.h"
#include <goblin/screeninfo.h>

namespace MemWar
{

/***********************************************************************
 *                                Constructor                          *
 ***********************************************************************/
ChallengeInfo::ChallengeInfo(Ogre::Overlay* ogreOverlay, 
      Ogre::Real x, Ogre::Real y,
      Ogre::String imageFileName, bool useTime, Challenge* c):
      Goblin::Image(ogreOverlay, CHALLENGES_DIR + imageFileName, "gui")
{
   challenge = c;
   int mod = Goblin::ScreenInfo::shouldUseDoubleSizedGui() ? 2 : 1;
   setPosition(-100, -100);
   /* Define timer font and text (if use a timer) */
   if (useTime) 
   {
      remainingText = new Goblin::TextBox(-88, -74, 24, 24, "0", 
                                          imageFileName + "_txt",
                                          ogreOverlay, "infoFont", 16);
      remainingText->setAlignment(Ogre::TextAreaOverlayElement::Center);
      this->addChild(remainingText, 12 * mod, 26 * mod);
   } 
   else 
   {
      remainingText = NULL;
   }
   setTargetPosition(x, y);

   /* Define challenge card */
   card = new Goblin::Image(ogreOverlay, CHALLENGE_CARDS_DIR + imageFileName, 
         "gui");
   card->setPosition(-300, Goblin::ScreenInfo::getHalfWindowWidth() - 
         (128 * mod));
   card->hide();
}

/***********************************************************************
 *                                 Destructor                          *
 ***********************************************************************/
ChallengeInfo::~ChallengeInfo()
{
   if (remainingText != NULL) 
   {
      delete remainingText;
   }
   if (card != NULL) 
   {
      delete card;
   }
}

/***********************************************************************
 *                                 update                              *
 ***********************************************************************/
void ChallengeInfo::update()
{
   if (remainingText != NULL) 
   {
      if (challenge->getRemainingTime() > 0) 
      {
         remainingText->setText(Ogre::StringConverter::toString(
                  challenge->getRemainingTime()));
      } 
      else 
      {
         remainingText->setText("0");
      }
   }

   if (card != NULL) 
   {
      card->update();
   }
   ((Goblin::Image*) this)->update();
}

/***********************************************************************
 *                         isUpdatingPosition                          *
 ***********************************************************************/
bool ChallengeInfo::isUpdatingPosition()
{
   return needUpdate;
}


/***********************************************************************
 *                                getCard                              *
 ***********************************************************************/
Goblin::Image* ChallengeInfo::getCard()
{
   return card;
}
    
}

