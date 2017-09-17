#include "worldselector.h"

#include <kobold/defparser.h>
#include <goblin/screeninfo.h>

using namespace MemWar;

#define WORLDS_FILE "worlds.mwr"
#define TOKEN_TOTAL "total"
#define TOKEN_WORLD "world"
#define TOKEN_FILE "file"
#define TOKEN_SCORES "scores"

#define EXTENSION_WORLD        ".mwr"
#define EXTENSION_LOGO         ".png"
#define EXTENSION_LOGO_DOUBLE  "_2x.png"

/***************************************************************************
 *                             Constructor                                 *
 ***************************************************************************/
WorldInfo::WorldInfo() : Selectable()
{
   this->buttonFile = "";
   this->scoresFile = "";
}

/***************************************************************************
 *                              Destructor                                 *
 ***************************************************************************/
WorldInfo::~WorldInfo()
{
}

/***************************************************************************
 *                       getButtonImageFileName                            *
 ***************************************************************************/
Ogre::String WorldInfo::getButtonImageFileName()
{
   return this->buttonFile;
}

/***************************************************************************
 *                       setButtonImageFileName                            *
 ***************************************************************************/
void WorldInfo::setButtonImageFileName(Ogre::String fileName)
{
   this->buttonFile = fileName;
}

/***************************************************************************
 *                          getScoresFileName                              *
 ***************************************************************************/
Ogre::String WorldInfo::getScoresFileName()
{
   return this->scoresFile;
}

/***************************************************************************
 *                          setScoresFileName                              *
 ***************************************************************************/
void WorldInfo::setScoresFileName(Ogre::String fileName)
{
   this->scoresFile = fileName;
}


/***************************************************************************
 *                             Constructor                                 *
 ***************************************************************************/
WorldSelector::WorldSelector(Ogre::Overlay* ogreOverlay) : Selector(ogreOverlay)
{
   Kobold::DefParser def;
   Ogre::String key, value;

   if(!def.load(WORLDS_FILE))
   {
      return;
   }

   WorldInfo* curWorld = NULL;

   while(def.getNextTuple(key, value))
   {
      if(key == TOKEN_TOTAL)
      {
         /* Start the read of worlds */
         beginCreation(Ogre::StringConverter::parseInt(value));
      }
      else if(key == TOKEN_WORLD)
      {
         /* Create the new world selectable */
         curWorld = (WorldInfo*) addNextSelectable(value);
      }
      else if(key == TOKEN_FILE)
      {
         /* Define world defintion filename */
         curWorld->setFileName(value + EXTENSION_WORLD);
         
         /* Define logo file to use. */
         Ogre::String logoFile;
         if(Goblin::ScreenInfo::shouldUseDoubleSizedGui())
         {
            logoFile = value + EXTENSION_LOGO_DOUBLE;
         }
         else
         {
            logoFile = value + EXTENSION_LOGO;
         }
         curWorld->setButtonImageFileName(logoFile);
         curWorld->createButton(logoFile, "levels", ogreOverlay);
      }
      else if(key == TOKEN_SCORES)
      {
         /* Define file of user's scores */
         curWorld->setScoresFileName(value);
      }
   }

   endCreation();
}

/***************************************************************************
 *                              Destructor                                 *
 ***************************************************************************/
WorldSelector::~WorldSelector()
{
   clearSelectables();
}

/***************************************************************************
 *                             createSelectable                            *
 ***************************************************************************/
Selectable* WorldSelector::createSelectable()
{
   return (Selectable*) new WorldInfo();
}

/***************************************************************************
 *                             createSelectable                            *
 ***************************************************************************/
void WorldSelector::deleteSelectable(Selectable* selectable)
{
   WorldInfo* wi = (WorldInfo*) selectable;
   delete wi;
}

/***************************************************************************
 *                           getSelectedWorldInfo                          *
 ***************************************************************************/
WorldInfo* WorldSelector::getSelectedWorldInfo()
{
   return (WorldInfo*) getSelected();
}

