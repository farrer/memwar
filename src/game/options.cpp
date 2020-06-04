
#include "options.h"
#include <kobold/userinfo.h>
#include <kosound/sound.h>
#include <iostream>
using namespace std;

#define OPTIONS_FILENAME "options.cfg"

#define OPTIONS_TOKEN_MUSIC "music"
#define OPTIONS_TOKEN_SOUND "sound"

#define OPTIONS_VALUE_TRUE   "true"
#define OPTIONS_VALUE_FALSE  "false"

namespace MemWar
{

/***********************************************************************
 *                                 load                                *
 ***********************************************************************/
void Options::load()
{
   Kobold::DefParser def;
   Ogre::String key, value;

   if(!def.load(Kobold::UserInfo::getUserHome() + OPTIONS_FILENAME, false))
   {
      return;
   }
   while(def.getNextTuple(key, value))
   {
      if(key == OPTIONS_TOKEN_SOUND)
      {
         soundEnabled = (value == OPTIONS_VALUE_TRUE);
      }
      else if(key == OPTIONS_TOKEN_MUSIC)
      {
         musicEnabled = (value == OPTIONS_VALUE_TRUE);
      }
   }
}

/***********************************************************************
 *                                 save                                *
 ***********************************************************************/
void Options::save()
{
   ofstream file;

   /* Open the file */
   file.open((Kobold::UserInfo::getUserHome() + OPTIONS_FILENAME).c_str(),
         ios::out | ios::binary);
   if(!file)
   {
      return;
   }

   file << OPTIONS_TOKEN_MUSIC << " = " 
        << (musicEnabled ? OPTIONS_VALUE_TRUE : OPTIONS_VALUE_FALSE) << endl;
   file << OPTIONS_TOKEN_SOUND << " = " 
         << (soundEnabled ? OPTIONS_VALUE_TRUE : OPTIONS_VALUE_FALSE) << endl;

   /* Close it */
   file.close();
}

/***********************************************************************
 *                          setSoundOptions                            *
 ***********************************************************************/     
void Options::setSoundOptions(bool sndfx, bool music)
{
   soundEnabled = sndfx;
   musicEnabled = music;
}

/***********************************************************************
 *                           isMusicEnabled                            *
 ***********************************************************************/
bool Options::isMusicEnabled()
{
   return musicEnabled;
}

/***********************************************************************
 *                           isSoundEnabled                            *
 ***********************************************************************/
bool Options::isSoundEnabled()
{
   return soundEnabled;
}

/***********************************************************************
 *                           applySoundOptions                         *
 ***********************************************************************/
void Options::applySoundOptions()
{
   int soundVolume = (soundEnabled) ? DEFAULT_VOLUME : 0;
   int musicVolume = (musicEnabled) ? DEFAULT_VOLUME : 0;

   Kosound::Sound::changeVolume(musicVolume, soundVolume);
}

/***********************************************************************
 *                              Static Members                         *
 ***********************************************************************/
bool Options::musicEnabled = true;
bool Options::soundEnabled = true;

}
