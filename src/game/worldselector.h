#ifndef _world_selector_h
#define _world_selector_h

#include "selector.h"

namespace MemWar
{

/*! Retrieves some information about worlds on selection */
class WorldInfo : public Selectable
{
   public:
      /*! Constructor */
      WorldInfo();
      /*! Destructor */
      ~WorldInfo();

      /*! \return filename for button to use for this logo. */
      Ogre::String getButtonImageFileName();
      /*! Set the image to use as button for this world. */
      void setButtonImageFileName(Ogre::String fileName);

      /*! \return filename for scores save / load. */
      Ogre::String getScoresFileName();
      /*! Set the file at user's home for save / load scores */
      void setScoresFileName(Ogre::String fileName);


   protected:
      Ogre::String buttonFile; /**< filename to use as button image */
      Ogre::String scoresFile; /**< filename to save / load scores */
};

/*! The Selector class for Worlds */
class WorldSelector : public Selector
{
   public:
      /*! Constructor */
      WorldSelector(Ogre::Overlay* ogreOverlay);
      /*! Destructor */
      ~WorldSelector();

      /*! \return information about selected world. */
      WorldInfo* getSelectedWorldInfo();

   protected:

      /*! Create a selectable object.
       * \return Selectable created */
      Selectable* createSelectable();

      /*! Delete a selectable object.
       * \param selectable pointer of object to delete */
      void deleteSelectable(Selectable* selectable);
};

}

#endif

