
#ifndef _dnteam_memwar_main_h
#define _dnteam_memwar_main_h

#include <goblin/baseapp.h>
#include <kobold/platform.h>

#include "mblock.h"
#include "initial.h"
#include "level.h"
#include "options.h"
#include "pause.h"
#include "score.h"
#include "world.h"
#include "worldselector.h"

#include "../special/special.h"
#include "../special/selector.h"

namespace MemWar
{

/*! The main xut class*/
class MemWarMain : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      MemWarMain();
      /*! Destructor */
      ~MemWarMain();

   protected:

      const Ogre::String getApplicationUnixName() const { return "memwar";};
      const Orientation getDefaultOrientation() const { return PORTRAIT; };
      const int getMinWidthToUseDoubleSizedGui() const { return 500; };
      const int getDefaultWindowWidth() const { return 320; };
      const int getDefaultWindowHeight() const { return 640; };
      bool shouldQuit() { return shouldExit; };
#if KOBOLD_PLATFORM == KOBOLD_PLATFORM_IOS ||\
    KOBOLD_PLATFORM == KOBOLD_PLATFORM_ANDROID
      const Ogre::String getBaseDataDir() const { return "data"; };
#else
      const Ogre::String getBaseDataDir() const { return "../data"; };
#endif

      bool doCycleInit(int callCounter, bool& shouldAbort);
      void getDataDirectories(Ogre::String** dataDirectories,
                  Ogre::String** dataGroups, int& total);
      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();
      void doBeforeRender();
      void doAfterRender();


      /*! Set field angles, based on hardware-orientation
       * \param vX -> orientation on X
       * \param vY -> orientation on Y */
      void setFieldAngles(Ogre::Real vX, Ogre::Real vY);

      /*! Verify Game Input Events */
      void gameIO();

      /*! Do the random initial seed for the game */
      void randomSeed();

      /*! Pause all timers */
      void pauseTimers();
      /*! Resume all timers */
      void resumeTimers();
      
      /*! Load - and start to play - current world selected level */
      void loadWorldSelectedLevel();

      Ogre::RaySceneQuery* ogreRaySceneQuery;/**< To ray cast */

      MemWar::World* world;                  /**< Current defined world */
      MemWar::Level* gameLevel;              /**< Current game level */
      MemWar::SpecialSelector* spSelector;   /**< The specials selector */
      MemWar::Pause* pause;                  /**< The pause controller */
      
      MemWar::Initial* initialScreen;        /**< Initial Screen */
      MemWar::WorldSelector* worldSelector;  /**< Selector of worlds */
      
      Ogre::Overlay* guiOverlay;             /**< Overlay for the level GUI */
  
      Goblin::Model3d* field; /**< Model to be used as game table. */
      bool shouldExit; /**< Exit controller variable. */


};

}

#endif


