
#include "memwar.h"

#include <fstream>

#include <OGRE/OgreLogManager.h>
#include <OGRE/OgreLog.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   #include "windows.h"
#endif

#include <kosound/sound.h>
#include <kobold/userinfo.h>
#include <goblin/camera.h>
#include <goblin/screeninfo.h>

using namespace MemWar;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
MemWarMain::MemWarMain()
           :Goblin::BaseApp(Goblin::CameraConfig())
{
   ogreRaySceneQuery = NULL;
   guiOverlay = NULL;
   spSelector = NULL;
   pause = NULL;
 
   field = NULL;
   world = NULL;
   worldSelector = NULL; 
   gameLevel = NULL;
   initialScreen = NULL;

   shouldExit = false;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
MemWarMain::~MemWarMain()
{
   /* Clear Field */
   if(field != NULL)
   {
      delete field;
   }

   if(gameLevel != NULL)
   {
      delete gameLevel;
   }
   if(world != NULL)
   {
      delete world;
   }
   if(worldSelector != NULL)
   {
      delete worldSelector;
   }
   if(pause != NULL)
   {
      delete pause;
   }

   Specials::finish();
   
   if(initialScreen != NULL)
   {
      delete initialScreen;
   }
   
   Score::finish();
   
   if(spSelector != NULL)
   {
      delete spSelector;
   }
   if(guiOverlay != NULL)
   {
      Ogre::OverlayManager::getSingletonPtr()->destroy(guiOverlay);
   }
   if(ogreRaySceneQuery != NULL)
   {
      ogreSceneManager->destroyQuery(ogreRaySceneQuery);
   }
}

/***********************************************************************
 *                              randomSeed                             *
 ***********************************************************************/
void MemWarMain::randomSeed()
{
   std::ifstream fileSeed;
   std::ofstream writeSeed;
   std::string strSeed;
   Ogre::String fileName = Kobold::UserInfo::getUserHome() + "rseed.mem";
   int seed=0;

   /* Load last random seed from file */
   fileSeed.open(fileName.c_str(), std::ios::in | std::ios::binary);

   if(fileSeed)
   {
      getline(fileSeed, strSeed);
      seed = Ogre::StringConverter::parseInt(strSeed);
      fileSeed.close();
   }
   
   seed = ((seed + updateTimer.getMilliseconds()) % 32768);

   /* Write new seed */
   writeSeed.open(fileName.c_str(), std::ios::out | std::ios::binary);
   if(writeSeed)
   {
      writeSeed << seed << std::endl;
      writeSeed.close();
   }

   srand(seed);
}

/***********************************************************************
 *                            getDataDirectories                       *
 ***********************************************************************/
void MemWarMain::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   Ogre::String guiDir = "gui";
   if(Goblin::ScreenInfo::shouldUseDoubleSizedGui())
   {
      guiDir += "2x";
   }
   static Ogre::String dirs[] = {"models", "materials", "levels", guiDir,
      "sndfx", "music", "fonts"};
   static Ogre::String groups[] = {"models", "materials", "levels", "gui",
      "sndfx", "music", "fonts"};

   total = 7;
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &groups[0];
}

/***********************************************************************
 *                                 doInit                              *
 ***********************************************************************/
bool MemWarMain::doCycleInit(int callCounter, bool& shouldAbort)
{
   //Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);

   /* Load current options */
   Options::load();

   Goblin::Camera::disableRotations();
   Goblin::Camera::disableTranslations();
   Goblin::Camera::disableZoomChanges();

   Goblin::Camera::set(0, 3, 0, 0, 89, 16.75f);

   /* Load the field model */
   field = new Goblin::Model3d("field", "field/field.mesh", "models",
         ogreSceneManager, Goblin::Model3d::MODEL_DYNAMIC);
   field->setScaleNow(1.5f, 1.5f, 1.5f);
   field->setPositionNow(0.0f, -1.0f, 4.0f);
   
   /* Create the gui overlay */
   guiOverlay = Ogre::OverlayManager::getSingletonPtr()->create("guiOverlay");
   guiOverlay->setZOrder(648);
   guiOverlay->show();

   /* Create a SceneQuery */
   ogreRaySceneQuery = ogreSceneManager->createRayQuery(Ogre::Ray());

   /* Do the random seed */
   randomSeed();
   
   /* Set ambient light */
   ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));
   
   /* Create the specials */
   Specials::init();
   Specials::hideIcons();
   
   /* Create the main screen */
   initialScreen = new MemWar::Initial();
   initialScreen->open();

   /* Create the world selector. */
   worldSelector = new MemWar::WorldSelector(guiOverlay);

   /* Create the pause controller */
   pause = new MemWar::Pause(guiOverlay);
   
   /* Init the score controller */
   Score::init(guiOverlay);

   /* Add music */
   Options::applySoundOptions();
   Kosound::Sound::loadMusic("factory.ogg");

   return true;
}

/***********************************************************************
 *                           setFieldAngles                            *
 ***********************************************************************/
void MemWarMain::setFieldAngles(Ogre::Real vX, Ogre::Real vY)
{
   vX -= 0.5;
   vY -= 0.5;
   vY *= -1;
   
   /* Reset the orientation */
   field->setOrientation(vY * 10, 0.0f, vX * 10);
}


/***********************************************************************
 *                                  run                                *
 ***********************************************************************/
void MemWarMain::doBeforeRender()
{
}
void MemWarMain::doAfterRender()
{
         
   if(initialScreen->isRunning())
   {
      /* Treat initial screen input */
      switch(initialScreen->verifyAction(mouseX,mouseY,leftButtonPressed))
      {
         case INITIAL_STATE_NEW:
         {
            worldSelector->show();
         }
         break;
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
         case INITIAL_STATE_QUIT:
         {
            shouldExit = true;
         }
         break;
#endif
      }
            
   }
   else if( (worldSelector->isDisplaying()) && 
         (!Score::isDisplaying()) && 
         ((world == NULL) || (!world->isDisplaying())) )
   {
      int ret = worldSelector->treat(mouseX, mouseY, leftButtonPressed);
      if(ret == WorldSelector::RETURN_CLOSE)
      {
         /* Return to initial screen */
         initialScreen->open();
      }
      else if(ret == WorldSelector::RETURN_SELECTED)
      {
         /* Selected a world, must create it. */
         if(world != NULL)
         {
            delete world;
         }
         WorldInfo* info = worldSelector->getSelectedWorldInfo();
         world = new World(info->getFileName(), 
               info->getScoresFileName(), info->getButtonImageFileName(),
               guiOverlay);
         world->show();
      }
   }
   else if( (world != NULL) && (world->isDisplaying()) &&
         (!Score::isDisplaying()) )
   {
      /* Is displaying selector */
      int worldRet = world->treat(mouseX, mouseY, leftButtonPressed);
      if(worldRet == World::RETURN_CLOSE)
      {
         /* Return to current world selector */ 
         worldSelector->show();
      }
      else if(worldRet == World::RETURN_SELECTED)
      {
         /* Show level information (ie: current high score),
          * or start the level if no information was available.*/
         WorldLevel* worldLevel = world->getSelectedLevel();

         if(worldLevel->isLocked())
         {
            /* Show info about what is needed to unlock the level */
            Score::displayAsLocked(
                  worldLevel->getInfoOnHowToUnlock());
         }
         else if(worldLevel->getScoreTime() == 
               UNDEFINED_WORLD_LEVEL_VALUE)
         {
            /* Level not yet finished, so no information to display */
            loadWorldSelectedLevel();
         }
         else 
         {
            /* Display current score */
            Score::display(worldLevel->getName(),
                  worldLevel->getScoreTime(), 
                  worldLevel->getScoreMoves(), 
                  worldLevel->getScoreStars());
         }
      }
   }
   else
   {   
      /* Select blocks on game */
      if ( (gameLevel != NULL) && 
            (gameLevel->getState() == Level::STATE_PLAYING) &&
            (!pause->isPaused()) && 
            (!spSelector) && (leftButtonPressed) )
      {
         /* Do a ray cast */
         Ogre::Ray mouseRay;
         Goblin::Camera::getCameraToViewportRay(
               mouseX / Ogre::Real(ogreWindow->getWidth()), 
               mouseY / Ogre::Real(ogreWindow->getHeight()),
               &mouseRay);
         ogreRaySceneQuery->setRay(mouseRay);
         ogreRaySceneQuery->setSortByDistance(true);

         /* Execute query */
         Ogre::RaySceneQueryResult &result = ogreRaySceneQuery->execute();
         Ogre::RaySceneQueryResult::iterator itr;

         for( itr = result.begin( ); itr != result.end(); itr++ )
         {
            if(itr->movable)
            {
               gameLevel->select(itr->movable->getParentSceneNode());
            }
         }

      }

      if(gameLevel != NULL)
      {
         /* Do general input */
         gameIO();
      }

      /* Treat score display or level */
      if(Score::isDisplaying())
      {
         /* Update the level, just to do the final flips, etc. */
         if(gameLevel != NULL)
         {
            gameLevel->update(false, -100, -100, false);
         }

         int ret = Score::treat(mouseX, mouseY, leftButtonPressed);
         bool replaying = false;

         if( ( (ret == Score::RETURN_NEXT_LEVEL) || 
                  (ret == Score::RETURN_REPLAY_LEVEL) ) &&
               (gameLevel != NULL) )
         {
            WorldLevel* levelToLoad;
            if( (ret == Score::RETURN_NEXT_LEVEL) &&
                  (world->getNextLevel(
                                       gameLevel->getWorldLevel()) != NULL) )
            {
               /* Call next level */
               levelToLoad = world->getNextLevel(
                     gameLevel->getWorldLevel());
               replaying = false;
            }
            else
            {
               /* Recall current level */
               levelToLoad = gameLevel->getWorldLevel();
               replaying = true;
            }

            /* Delete current loaded level */
            delete(gameLevel);

            /* And load the desired level */
            gameLevel = new Level(world, levelToLoad, ogreSceneManager, 
                  field, guiOverlay, replaying);
            Specials::setLevel(gameLevel);
            pause->reset();
         }
         else if( (ret == Score::RETURN_SHOW_LIST) ||
               (ret == Score::RETURN_CANCEL) )
         {
            world->show();
         } 
         else if(ret == Score::RETURN_PLAY_LEVEL)
         {
            /* Play the level selected at the selector. */
            loadWorldSelectedLevel();
         }
      }
      else if(gameLevel != NULL)
      {
         int retPause = Pause::RETURN_NONE;
         if(gameLevel->getState() != Level::STATE_SHOWING_INFO)
         {
            /* Treat pause controller (and screen if paused) */
            retPause = pause->treat(mouseX, mouseY, leftButtonPressed);
         }

         if(retPause == Pause::RETURN_PAUSE)
         {
            pauseTimers();
         }
         else if(retPause == Pause::RETURN_RESUME)
         {
            resumeTimers();
         }
         else if(retPause == Pause::RETURN_RESTART)
         {
            /* Reload and restart current level */
            WorldLevel* levelToLoad = gameLevel->getWorldLevel();
            delete gameLevel;
            gameLevel = new Level(world, levelToLoad, ogreSceneManager,
                  field, guiOverlay, true);
            Specials::setLevel(gameLevel);
            pause->reset();
         }
         else if(retPause == Pause::RETURN_EXIT)
         {
            /* Return to the main screen */
            initialScreen->open();
            gameLevel->hideUI();
         }
         else if(retPause == Pause::RETURN_NONE)
         {
            /* Update the game */
            gameLevel->update(!spSelector, mouseX, mouseY, 
                  leftButtonPressed);
         }
      }

      /* Check if level finished */
      if( (gameLevel != NULL) &&
            (gameLevel->isCompleted()) && (!Score::isDisplaying()) &&
            (!world->isDisplaying()) )
      {
         /* And hide any level related thing */ 
         pause->hide();
         /* Must display the score screen, saving in it any beaten
          * record. */
         Score::display();
         gameLevel->hideUI();
      }

   }
}

/********************************************************************
 *                    loadWorldSelectedLevel                        *
 ********************************************************************/
void MemWarMain::loadWorldSelectedLevel()
{
   initialScreen->close(true);
   if(gameLevel)
   {
      delete(gameLevel);
   }
   gameLevel = new Level(world, world->getSelectedLevel(), 
         ogreSceneManager, field, guiOverlay, false);
   pause->reset();
   Specials::setLevel(gameLevel);
#ifdef ENABLE_SPECIALS
   Specials::showIcons();
#endif
}

/********************************************************************
 *                            gameIO                                *
 ********************************************************************/
void MemWarMain::gameIO()
{
   /* Verify Special Selector Action */
   if ((spSelector) && (gameLevel) && 
       (gameLevel->getState() == Level::STATE_PLAYING))
   {
#ifdef ENABLE_SPECIALS
      if(spSelector->verifyAction(mouseX, mouseY, leftButtonPressed))
      {
         /* Done with special selector */
         delete spSelector;
         spSelector = NULL;
         resumeTimers();
      }
#endif
   }
   else
   {
      /* Update Field Orientation */
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
      setFieldAngles(mouseX / Ogre::Real(ogreWindow->getWidth()),
            mouseY / Ogre::Real(ogreWindow->getHeight()));
#endif
     
#ifdef ENABLE_SPECIALS 
      /* Verify if selected to activate special
       * FIXME: position hardcoded! */

      if ((gameLevel) && (gameLevel->getState() == Level::STATE_PLAYING))
      {
         if( (leftButtonPressed) &&
               (mouseX >= 180) && (mouseX <= 300) && 
               (mouseY >= 15) && (mouseY <= 35) )
         {
            spSelector = new SpecialSelector();
            pauseTimers();
         }
      }
#endif
   }
}

/***********************************************************************
 *                           doMemoryClean                             *
 ***********************************************************************/
void MemWarMain::doLowMemoryClean()
{
   //TODO
}

/***********************************************************************
 *                        doSendToBackground                           *
 ***********************************************************************/
void MemWarMain::doSendToBackground()
{
   pauseTimers();
}

/***********************************************************************
 *                        doSendToForeground                           *
 ***********************************************************************/
void MemWarMain::doSendToForeground()
{
   resumeTimers();
}

/***********************************************************************
 *                            pauseTimers                              *
 ***********************************************************************/
void MemWarMain::pauseTimers()
{
   gameLevel->pauseTimer();
   Specials::pauseTimer();
}

/***********************************************************************
 *                           resumeTimers                              *
 ***********************************************************************/
void MemWarMain::resumeTimers()
{
   gameLevel->resumeTimer();
   Specials::resumeTimer();
}
   
/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID

/* Note: main function for iphone is located at "main.mm" file */

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
      LPSTR lpCmdLine, int nCmdShow) 
#else
int main (int argc, char *argv[])
#endif
{
   /* Create and run the game */
   MemWar::MemWarMain* game = new MemWar::MemWarMain();
   game->run();
   delete game;

   return(0);
}

#endif
