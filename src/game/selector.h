#ifndef _memwar_selectable_selector_h
#define _memwar_selectable_selector_h

#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <goblin/ibutton.h>

namespace MemWar
{

/*! A selectable 'thing' to be used at the 'Selector'. */
class Selectable
{
   public:
      /*! Constructor */
      Selectable();
      /*! Destructor */
      ~Selectable();

      /*! Create (or recreate) the button associated with the selectable. */
      Goblin::Ibutton* createButton(const Ogre::String& imageFile, 
            const Ogre::String& groupName, Ogre::Overlay* ogreOverlay);

      /*! \return button associated with the selectable */
      Goblin::Ibutton* getButton();

      /*! \return the name to display */
      Ogre::String getName();
      /*! Define the selectable's name */
      void setName(Ogre::String name);

      /*! Define the filename associated with the selectable (if any) */
      void setFileName(Ogre::String fileName);
      /*! \return filename associated with the selectable */
      Ogre::String getFileName();

      /*! \return current button image */
      Ogre::String getButtonImage();

   protected:
      Ogre::String name;       /**< Name of the selectable */
      Ogre::String fileName;   /**< Filename of the selectable */
      Goblin::Ibutton* button; /**< Button to select. */
};

/*! A selector is a GUI component with buttons displayed on the
 * screen (paginated if not possible to display them all at the same time),
 * where the user could select one. */
class Selector
{
   public:

      /* Return for the Selector. */
      enum
      {
         RETURN_OTHER,
         RETURN_CLOSE,
         RETURN_SELECTED
      };


      /*! Constructor */
      Selector(Ogre::Overlay* ogreOverlay);
      /*! Destructor
       * \note children destructor must call clearSelectables() */
      virtual ~Selector();

      /*! Init the selector creation.
       * \param totalSelectables number of selectables available.  */
      void beginCreation(int totalSelectables);

      /*! Add next selectable and create its button.
       * \note must be called inner beginCreation-endCreation. */
      Selectable* addNextSelectable(const Ogre::String& name, 
            const Ogre::String& imageFile, const Ogre::String& groupName,
            Ogre::Overlay* ogreOverlay);

      /*! Finish the selector creation. */
      void endCreation();

      /*! Show the selector screen. */
      void show();

      /*! Treat the level selector screen for input
       * \return RETURN_ constant */
      int treat(int mouseX, int mouseY, bool leftButtonPressed);

      /*! Send level selector to its hidden position. */
      void hide();

      /*! Return true when displaying selector screen */
      bool isDisplaying();

      /*! Get the selected 'thing' (if any was selected). */
      Selectable* getSelected();

      /*! Just add the next selectable, without creating its button
       * (which should be done latter).
       * \note must be called inner beginCreation-endCreation. */
      Selectable* addNextSelectable(Ogre::String name);

      /*! \return Selectable by index */
      Selectable* getByIndex(int i);

      /*! \return total selectables in selector.*/
      int getTotal();


   protected:

      /*! Delete all created selectables */
      void clearSelectables();
     
      /*! Create a selectable object.
       * \return Selectable created */ 
      virtual Selectable* createSelectable()=0;

      /*! Delete a selectable object.
       * \param selectable pointer of object to delete */
      virtual void deleteSelectable(Selectable* selectable)=0;

      /*! Really call hide to all buttons. */
      void hideButtons();

      Ogre::Overlay* ogreOverlay; /**< Overlay used at the selector. */

   private:

      Selectable** selectables; /**< Each available selectable */
      int total;  /**< Total number of selectables. */
      int selected; /**< Current selectable selected (or at creation) */

      bool displaying; /**< If is displaying selector screen */
      int totalButtonsPerColumn; /**< How many buttons in a page column */
      int totalButtonsPerLine; /**< How many buttons in a page line */
      int totalButtonsPerPage; /**< How many buttons could be displayed */
      int firstDisplayed; /**< Index of first selectable button displayed */
      
      int treatReturn; /**< Value to return after hide. 
                            Relevant if not OTHER. */
                            
      Goblin::Ibutton* next; /**< Next set of selectables, if in multiple 
                                  pages */
      Goblin::Ibutton* previous; /**< Previous set of selectables. */
      Goblin::Ibutton* back; /**< Back to the caller screen button.*/
};

}

#endif

