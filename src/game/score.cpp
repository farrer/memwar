
#include "score.h"
#include "level.h"
#include <stdio.h>

#define HIDDEN_POS   -200

namespace MemWar
{
   
/***********************************************************************
 *                                 init                                *
 ***********************************************************************/
void Score::init(Ogre::Overlay* ogreOverlay)
{
   Score::ogreOverlay = ogreOverlay;
   
   /* Load the back images from disk */
   int halfWidth = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 512 : 256;
   backImage = new Goblin::Image(ogreOverlay, "scores/score.png", "gui");
   backImage->setPosition((Goblin::ScreenInfo::getHalfWindowWidth() - 
            halfWidth), HIDDEN_POS);
   backImage->hide();

   lockedBackImage = new Goblin::Image(ogreOverlay, "scores/locked.png", "gui");
   lockedBackImage->setPosition((Goblin::ScreenInfo::getHalfWindowWidth() - 
            halfWidth), HIDDEN_POS);
   lockedBackImage->hide();

   /* Load each button */
   replayButton = new Goblin::Ibutton(ogreOverlay, "initial/replay.png",
         "gui", "Play Again", "infoFontOut", 16);
   replayButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   replayButton->hide();

   playButton = new Goblin::Ibutton(ogreOverlay, "initial/next.png",
         "gui", "Play", "infoFontOut", 16);
   playButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   playButton->hide();

   cancelButton = new Goblin::Ibutton(ogreOverlay, "initial/exit.png",
         "gui", "Cancel", "infoFontOut", 16);
   cancelButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   cancelButton->hide();

   nextButton = new Goblin::Ibutton(ogreOverlay, "initial/advance.png",
         "gui", "Next Level", "infoFontOut", 16);
   nextButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   nextButton->hide();

   listButton = new Goblin::Ibutton(ogreOverlay, "initial/list.png",
         "gui", "Selector", "infoFontOut", 16);
   listButton->setPosition(HIDDEN_POS, HIDDEN_POS);
   listButton->hide();

   /* Create each textbox */
   textTitle = new Goblin::TextBox(HIDDEN_POS, HIDDEN_POS, 300, 64, "",
          "scoreTitleText", ogreOverlay, "infoFontOut", 32);
   textTitle->setAlignment(Ogre::TextAreaOverlayElement::Center);
   textTitle->hide();

   textTime = new Goblin::TextBox(HIDDEN_POS, HIDDEN_POS, 64, 32, "",
            "scoreTextTime", ogreOverlay, "scoreFont", 24);
   textTime->setAlignment(Ogre::TextAreaOverlayElement::Center);
   textTime->hide();
   textMoves = new Goblin::TextBox(HIDDEN_POS, HIDDEN_POS, 64, 32, "",
            "scoreTextMoves", ogreOverlay, "scoreFont", 24);
   textMoves->setAlignment(Ogre::TextAreaOverlayElement::Center);
   textMoves->hide();

   /* The number of obtained stars is yet unknown: they will be loaded
    * latter when showing the score screen. */
   starsLevel = NULL;
   starsMoves = NULL;
   starsTime = NULL;
   
   /* Reset Score things */
   reset(NULL, NULL);
   displayed = false;
}
   
/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void Score::finish()
{
   if(backImage)
   {
      delete backImage;
      backImage = NULL;
   }
   if(lockedBackImage)
   {
      delete lockedBackImage;
      lockedBackImage = NULL;
   }
   if(replayButton)
   {
      delete replayButton;
      replayButton = NULL;
   }
   if(playButton)
   {
      delete playButton;
      playButton = NULL;
   }
   if(cancelButton)
   {
      delete cancelButton;
   }
   if(nextButton)
   {
      delete nextButton;
      nextButton = NULL;
   }
   if(listButton)
   {
      delete listButton;
      listButton = NULL;
   }
   if(textTitle != NULL)
   {
      delete textTitle;
      textTitle = NULL;
   }
   if(textTime != NULL)
   {
      delete textTime;
      textTime = NULL;
   }
   if(starsTime != NULL)
   {
      delete starsTime;
      starsTime = NULL;
   }
   if(textMoves != NULL)
   {
      delete textMoves;
      textMoves = NULL;
   }
   if(starsMoves != NULL)
   {
      delete starsMoves;
      starsMoves = NULL;
   }
   if(starsLevel != NULL)
   {
      delete starsLevel;
      starsLevel = NULL;
   }
}

/***********************************************************************
 *                          getStarsFileName                           *
 ***********************************************************************/ 
Ogre::String Score::getStarsFileName(int stars)
{
   switch(stars)
   {
      case 1:
      default:
         return "initial/stars1.png";
      case 2:
         return "initial/stars2.png";
      case 3:
         return "initial/stars3.png";
   }
}
 
/***********************************************************************
 *                               display                               *
 ***********************************************************************/   
void Score::display()
{
   WorldLevel* wLevel = curLevel->getWorldLevel();

   /* Calculate obtained stars */
   int totalTimeStars = 1;
   if(totalTime <= curLevel->getTimeThreeStars())
   {
      totalTimeStars = 3;
   }
   else if(totalTime <= curLevel->getTimeTwoStars())
   {
      totalTimeStars = 2;
   }
   int totalMoveStars = 1;
   if(totalFlips <= curLevel->getMovesThreeStars())
   {
      totalMoveStars = 3;
   }
   else if(totalFlips <= curLevel->getMovesTwoStars())
   {
      totalMoveStars = 2;
   }
   int stars = (totalTimeStars + totalMoveStars) / 2;

   /* Must check if beat some record */
   bool timeRecord = false;
   if(wLevel->getScoreTime() > totalTime)
   {
      timeRecord = true;
      wLevel->setScoreTime(totalTime);
   }

   bool movesRecord = false;
   if(wLevel->getScoreMoves() > totalFlips)
   {
      movesRecord = true;
      wLevel->setScoreMoves(totalFlips);
   }

   bool starsRecord = false;
   if(wLevel->getScoreStars() < stars)
   {
      starsRecord = true;
      wLevel->setScoreStars(stars);
   }

   /* Record beaten, must save the records and try to unlock
    * some world levels */
   if((timeRecord) || (movesRecord) || (starsRecord))
   {
      curWorld->tryToUnlock();
      curWorld->saveScores();
   }

   doDisplay(true, false, wLevel->getName(), totalTimeStars, timeRecord, 
             totalMoveStars, movesRecord, stars);
}

/***********************************************************************
 *                               display                               *
 ***********************************************************************/
void Score::display(Ogre::String levelTitle, int totalTime, int totalMoves, 
                    int totalStars)
{
   Score::totalTime = totalTime;
   Score::totalFlips = totalMoves;
   doDisplay(false, false, levelTitle, 0, false, 0, false, totalStars);
}

/***********************************************************************
 *                               display                               *
 ***********************************************************************/
void Score::displayAsLocked(Ogre::String levelLockedInfo)
{
   doDisplay(false, true, levelLockedInfo, 0, false, 0, false, 0);
}

/***********************************************************************
 *                               display                               *
 ***********************************************************************/
void Score::doDisplay(bool afterLevel, bool locked, Ogre::String levelInfo, 
                      int totalTimeStars, bool timeRecord, 
                      int totalMoveStars, bool movesRecord, int totalStars)
{
   int sizeMod = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   int pX = Goblin::ScreenInfo::getHalfWindowWidth() - (256 * sizeMod);
   int pY = Goblin::ScreenInfo::getHalfWindowHeight() - (128 * sizeMod);
   if(!locked)
   {
      backImage->setTargetPosition(pX, pY);
      backImage->show();
   }
   else 
   {
      lockedBackImage->setTargetPosition(pX, pY);
      lockedBackImage->show();
   }

   if(afterLevel)
   {
      replayButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
               (150 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
               (140 * sizeMod));
      replayButton->show();
      listButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
               (32 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
               (140 * sizeMod));
      listButton->show();
   }
   else 
   {
      cancelButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
               (150 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
               (140 * sizeMod));
      cancelButton->show();
      if(!locked)
      {
         playButton->setTargetPosition(
               Goblin::ScreenInfo::getHalfWindowWidth() +
               (86 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
               (140 * sizeMod));
         playButton->show();
      }
   }

   if( (afterLevel) && 
       (curWorld->getNextLevel(curLevel->getWorldLevel()) != NULL) )
   {
      /* Only show next button if next level exists and is unlocked. */
      nextButton->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() +
               (86 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
               (140 * sizeMod));
      nextButton->show();
   }

   if(!locked)
   {
      /* Calculate and load image representing the time stars. */
      if(starsTime != NULL)
      {
         delete starsTime;
         starsTime = NULL;
      }
      if(afterLevel)
      {
         starsTime = new Goblin::Image(ogreOverlay,
               getStarsFileName(totalTimeStars), "gui");
         starsTime->setPosition(HIDDEN_POS, HIDDEN_POS);
         starsTime->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                  (75 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() -
                  (60 * sizeMod));
      }

      /* Calculate and load image representing the moves stars. */
      if(starsMoves != NULL)
      {
         delete starsMoves;
         starsMoves = NULL;
      }
      if(afterLevel)
      {
         starsMoves = new Goblin::Image(ogreOverlay,
               getStarsFileName(totalMoveStars), "gui");
         starsMoves->setPosition(HIDDEN_POS, HIDDEN_POS);
         starsMoves->setTargetPosition(
               Goblin::ScreenInfo::getHalfWindowWidth() -
               (60 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() -
               (8 * sizeMod));
      }

      /* Calculate and load image representing level stars. */
      if(starsLevel != NULL)
      {
         delete starsLevel;
      }
      starsLevel = new Goblin::Image(ogreOverlay, getStarsFileName(totalStars),
            "gui");
      starsLevel->setPosition(HIDDEN_POS, HIDDEN_POS);
      starsLevel->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
                  (60 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
                  (42 * sizeMod));

      textTitle->setFont("infoFontOut", 32);
      if(afterLevel)
      {
         textTitle->setColor(0.74f, 0.77f, 0.0f, 1.0f);
         textTitle->setText("Level Done!");
      }
      else 
      {
         textTitle->setColor(1.0f, 1.0f, 1.0f, 1.0f);
         textTitle->setText(levelInfo);
      }
      textTitle->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth(),
                  Goblin::ScreenInfo::getHalfWindowHeight() - (110 * sizeMod));
      textTitle->show();

      /* Set and turn text visible */
      textTime->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() +
                  (50 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() -
                  (46 * sizeMod));
      int minutes = totalTime / 60;
      int seconds = totalTime % 60;
      textTime->setText(Ogre::StringConverter::toString(minutes, 2, '0') + 
            ":" + Ogre::StringConverter::toString(seconds, 2, '0') + 
            ((timeRecord) ? "R" : ""));
      textTime->show();

      textMoves->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() +
                  (50 * sizeMod), Goblin::ScreenInfo::getHalfWindowHeight() +
                  (8 * sizeMod));
      textMoves->setText(Ogre::StringConverter::toString(totalFlips) +
            ((movesRecord) ? "R" : ""));
      textMoves->show();
   } 
   else 
   {
      /* Just show locked information */
      textTitle->setFont("infoFontOut", 16);
      textTitle->setColor(1.0f, 1.0f, 1.0f, 1.0f);
      textTitle->setText(levelInfo);
      textTitle->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth(),
                                   Goblin::ScreenInfo::getHalfWindowHeight());
      textTitle->show();
   }

   displayed = true;
   treatReturn = RETURN_OTHER;

}
   
/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
int Score::treat(int mouseX, int mouseY, bool leftButtonPressed)
{
   backImage->update();
   lockedBackImage->update();
   if(starsTime != NULL)
   {
      starsTime->update();
   }
   if(starsMoves != NULL)
   {
      starsMoves->update();
   }
   if(starsLevel != NULL)
   {
      starsLevel->update();
   }


   textTitle->update();
   textTime->update();
   textMoves->update();

   if(replayButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
         IBUTTON_EVENT_PRESSED)
   {
      treatReturn = RETURN_REPLAY_LEVEL;
      sendToHidePosition();
   }
   if(playButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
         IBUTTON_EVENT_PRESSED)
   {
      treatReturn = RETURN_PLAY_LEVEL;
      sendToHidePosition();
   }
   if(cancelButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
         IBUTTON_EVENT_PRESSED)
   {
      treatReturn = RETURN_CANCEL;
      sendToHidePosition();
   }
   if(listButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
         IBUTTON_EVENT_PRESSED)
   {
      treatReturn = RETURN_SHOW_LIST;
      sendToHidePosition();
   }
   if(nextButton->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
         IBUTTON_EVENT_PRESSED)
   {
      treatReturn = RETURN_NEXT_LEVEL;
      sendToHidePosition();
   }

   if( (treatReturn != RETURN_OTHER) && (nextButton->getPosX() == HIDDEN_POS) )
   {
      int ret = treatReturn;
      treatReturn = RETURN_OTHER;
      hide();
      displayed = false;
      return ret;
   }

   return RETURN_OTHER;
}
   
/***********************************************************************
 *                             isDisplaying                            *
 ***********************************************************************/
bool Score::isDisplaying()
{
   return displayed;
}
   
/***********************************************************************
 *                               reset                                 *
 ***********************************************************************/
void Score::reset(Level* lvl, World* world)
{
   totalTime = 0;
   totalFlips = 0;
   totalChallengesLost = 0;
   totalSpecials = 0;
   curLevel = lvl;
   curWorld = world;
}
   
/***********************************************************************
 *                            flippedBlock                             *
 ***********************************************************************/
void Score::flippedBlock()
{
   totalFlips++;
}
   
/***********************************************************************
 *                            lostChallenge                            *
 ***********************************************************************/  
void Score::lostChallenge()
{
   totalChallengesLost++;
}
   
/***********************************************************************
 *                             usedSpecial                             *
 ***********************************************************************/
void Score::usedSpecial()
{
   totalSpecials++;
}
   
/***********************************************************************
 *                             setTotalTime                            *
 ***********************************************************************/
void Score::setTotalTime(int seconds)
{
   totalTime = seconds;
}

/***********************************************************************
 *                                  hide                               *
 ***********************************************************************/
void Score::hide()
{
   backImage->hide();
   lockedBackImage->hide();
   replayButton->hide();
   playButton->hide();
   cancelButton->hide();
   listButton->hide();
   nextButton->hide();
   textTitle->hide();
   textTime->hide();
   textMoves->hide();

   if(starsTime != NULL)
   {
      delete starsTime;
      starsTime = NULL;
   }
   if(starsMoves != NULL)
   {
      delete starsMoves;
      starsMoves = NULL;
   }
   if(starsLevel != NULL)
   {
      delete starsLevel;
      starsLevel = NULL;
   }
}

/***********************************************************************
 *                          sendToHidePosition                         *
 ***********************************************************************/
void Score::sendToHidePosition()
{
   backImage->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() - 
         HIDDEN_POS, HIDDEN_POS);
   lockedBackImage->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth()
         - HIDDEN_POS, HIDDEN_POS);
   replayButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   playButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   cancelButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   listButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   nextButton->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   textTitle->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   textTime->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   textMoves->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   if(starsTime != NULL)
   {
      starsTime->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   }
   if(starsMoves != NULL)
   {
      starsMoves->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   }
   if(starsLevel != NULL)
   {
      starsLevel->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   }
}
   
/***********************************************************************
 *                          static members                             *
 ***********************************************************************/
int Score::totalTime=0;
int Score::totalFlips=0;
int Score::totalChallengesLost=0;
int Score::totalSpecials=0;
bool Score::displayed=false;
Goblin::Image* Score::backImage=NULL;
Goblin::Image* Score::lockedBackImage=NULL;
Ogre::Overlay* Score::ogreOverlay = NULL; 
Level* Score::curLevel = NULL;
World* Score::curWorld = NULL;
Goblin::Ibutton* Score::replayButton = NULL;
Goblin::Ibutton* Score::playButton = NULL;
Goblin::Ibutton* Score::cancelButton = NULL;
Goblin::Ibutton* Score::nextButton = NULL;
Goblin::Ibutton* Score::listButton = NULL;
Goblin::TextBox* Score::textTime = NULL;
Goblin::TextBox* Score::textTitle = NULL;
Goblin::Image* Score::starsTime = NULL;
Goblin::TextBox* Score::textMoves = NULL;
Goblin::Image* Score::starsMoves = NULL;
Goblin::Image* Score::starsLevel = NULL;
int Score::treatReturn = RETURN_OTHER;


}
