
#ifndef _memwar_Options_h
#define _memwar_Options_h

#include <OGRE/OgreString.h>

#include <kobold/defparser.h>

namespace MemWar
{

/*! Game Options */
class Options
{
   public:

      /*! Load Options from file */
      static void load();

      /*! Save options to file */
      static void save();

      /*! If sound effects are enabled */
      static bool isSoundEnabled();

      /*! If music are enabled */
      static bool isMusicEnabled();

      /*! Set enabled options related to sound (effects and music).
       * \param sndfx sound effects should be enabled?
       * \param music should be enabled? */
      static void setSoundOptions(bool sndfx, bool music);

      /*! Apply current sound options to the sound controller. */
      static void applySoundOptions();

   private:
      Options(){};

      static bool musicEnabled;
      static bool soundEnabled;
};

}

#endif

