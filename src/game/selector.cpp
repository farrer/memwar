#include "selector.h"
#include <goblin/screeninfo.h>

using namespace MemWar;

#define DEFAULT_BUTTON_PLUS_SPACE_SIZE 72
#define HIDDEN_POS -200


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                             Selectable                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/**************************************************************************
 *                             Constructor                                *
 **************************************************************************/
Selectable::Selectable()
{
   this->button = NULL;
   this->name = "";
   this->fileName = "";
}

/**************************************************************************
 *                              Destructor                                *
 **************************************************************************/
Selectable::~Selectable()
{
   if(this->button != NULL)
   {
      delete this->button;
   }
}

/**************************************************************************
 *                            createButton                                *
 **************************************************************************/
Goblin::Ibutton* Selectable::createButton(const Ogre::String& imageFile, 
      const Ogre::String& groupName, Ogre::Overlay* ogreOverlay)
{
   /* Delete previously created button, if any. */
   if(this->button != NULL)
   {
      delete this->button;
   }
   /* (Re)create it */
   this->button = new Goblin::Ibutton(ogreOverlay, imageFile, 
         groupName, this->name, "infoFontOut", 16);

   /* Reset it to hidden position (and hidden!) */
   this->button->setPosition(HIDDEN_POS, HIDDEN_POS);
   this->button->hide();

   return this->button;
}

/**************************************************************************
 *                               getButton                                *
 **************************************************************************/
Goblin::Ibutton* Selectable::getButton()
{
   return this->button;
}

/**************************************************************************
 *                                getName                                 *
 **************************************************************************/
Ogre::String Selectable::getName()
{
   return this->name;
}

/**************************************************************************
 *                                setName                                 *
 **************************************************************************/
void Selectable::setName(Ogre::String name)
{
   this->name = name;
}

/**************************************************************************
 *                              getFileName                               *
 **************************************************************************/
Ogre::String Selectable::getFileName()
{
   return this->fileName;
}

/**************************************************************************
 *                              setFileName                               *
 **************************************************************************/
void Selectable::setFileName(Ogre::String fileName)
{
   this->fileName = fileName;
}

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                              Selector                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/**************************************************************************
 *                             Constructor                                *
 **************************************************************************/
Selector::Selector(Ogre::Overlay* ogreOverlay)
{
   this->ogreOverlay = ogreOverlay;
   
   this->selectables = NULL;
   this->total = 0;
   this->selected = -1;

   this->displaying = false;
   
   /* Calculate totals */
   int ratio = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;
   this->totalButtonsPerColumn = (Goblin::ScreenInfo::getWindowHeight() 
         - (200 * ratio)) /
      (DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio);
   this->totalButtonsPerLine = (Goblin::ScreenInfo::getWindowWidth() - 40) /
      (DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio);
   this->totalButtonsPerPage = this->totalButtonsPerLine * 
      this->totalButtonsPerColumn;
   this->firstDisplayed = 0;

   this->treatReturn = RETURN_OTHER;

   /* Create buttons */
   this->next = new Goblin::Ibutton(ogreOverlay, "initial/next.png",
         "gui", "Next", "infoFontOut", 16);
   this->next->setPosition(HIDDEN_POS, HIDDEN_POS);
   this->next->hide();
   this->previous = new Goblin::Ibutton(ogreOverlay, "initial/previous.png", 
         "gui", "Previous", "infoFontOut", 16);
   this->previous->setPosition(HIDDEN_POS, HIDDEN_POS);
   this->previous->hide();
   this->back = new Goblin::Ibutton(ogreOverlay, "initial/exit.png",
         "gui", "Cancel", "infoFontOut", 16);
   this->back->setPosition(HIDDEN_POS, HIDDEN_POS);
   this->back->hide();
}

/**************************************************************************
 *                              Destructor                                *
 **************************************************************************/
Selector::~Selector()
{
   if(this->selectables != NULL)
   {
      delete[] this->selectables;
   }
   delete this->next;
   delete this->previous;
   delete this->back;
}

/**************************************************************************
 *                           clearSelectables                             *
 **************************************************************************/
void Selector::clearSelectables()
{
   for(int i=0; i < this->total; i++)
   {
      if(this->selectables[i] != NULL)
      {
         deleteSelectable(selectables[i]);
         this->selectables[i] = NULL;
      }
   }
}

/**************************************************************************
 *                             beginCreation                              *
 **************************************************************************/
void Selector::beginCreation(int totalSelectables)
{
   this->total = totalSelectables;
   if(this->selectables == NULL)
   {
      this->selectables = new Selectable*[this->total];
      for(int i=0; i < this->total; i++)
      {
         this->selectables[i] = NULL;
      }
   }
   else
   {
      clearSelectables();
   }
   this->selected = 0;
}

/**************************************************************************
 *                          addNextSelectable                             *
 **************************************************************************/
Selectable* Selector::addNextSelectable(Ogre::String name)
{
   if(this->selected >= this->total)
   {
      /* Shouldn't add after vector size! */
      return NULL;
   }

   this->selectables[this->selected] = createSelectable(); 
   this->selectables[this->selected]->setName(name); 
   this->selected++;

   return this->selectables[this->selected - 1];
}

/**************************************************************************
 *                          addNextSelectable                             *
 **************************************************************************/
Selectable* Selector::addNextSelectable(const Ogre::String& name, 
      const Ogre::String& imageFile, const Ogre::String& groupName, 
      Ogre::Overlay* ogreOverlay)
{
   Selectable* sel = addNextSelectable(name);

   if(sel != NULL)
   {
      sel->createButton(imageFile, groupName, ogreOverlay);
   }

   return sel;
}

/**************************************************************************
 *                            endCreation                                 *
 **************************************************************************/
void Selector::endCreation()
{
   this->selected = -1;
}

/**************************************************************************
 *                                show                                    *
 **************************************************************************/
void Selector::show()
{
   this->displaying = true;
   
   int buttonSize = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 128 : 64;
   int halfButtonSize = buttonSize / 2;
   int ratio = (Goblin::ScreenInfo::shouldUseDoubleSizedGui()) ? 2 : 1;

   /* Set to display navitating buttons */
   if(this->total > this->totalButtonsPerPage)
   {
      /* Total levels is greater than maximun displayed quantity.
       * Must show previous and next buttons to navigate between them. */
      this->next->setTargetPosition(Goblin::ScreenInfo::getWindowWidth() - 16 -
               buttonSize, 
               Goblin::ScreenInfo::getWindowHeight() - buttonSize - 36);
      this->next->show();

      this->previous->setTargetPosition(16,
               Goblin::ScreenInfo::getWindowHeight() - buttonSize - 36);
      this->previous->show();
   }
   this->back->setTargetPosition(Goblin::ScreenInfo::getHalfWindowWidth() -
         halfButtonSize, Goblin::ScreenInfo::getWindowHeight() - buttonSize -
         36); 
   this->back->show();

   int column = 0;
   int line = -1;
   int initialX = (Goblin::ScreenInfo::getWindowWidth() -
         (this->totalButtonsPerLine * DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio) +
         DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio - buttonSize) / 2;
   for(int i = 0; i < this->total; i++)
   {
      bool buttonVisible = (i >= this->firstDisplayed) &&
         (i < this->firstDisplayed + this->totalButtonsPerPage);

      if((buttonVisible) && (i % this->totalButtonsPerLine == 0))
      {
         column = 0;
         line++;
      }

      if(buttonVisible)
      {
         /* Send the button to its visible area. */
         selectables[i]->getButton()->setTargetPosition(initialX + column *
               DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio, 120 * ratio + line *
               DEFAULT_BUTTON_PLUS_SPACE_SIZE * ratio);
         selectables[i]->getButton()->show();
         column++;
      } else {
         /* Send button to the hidden position */
         selectables[i]->getButton()->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
      }
   }
}

/****************************************************************************
 *                               isDisplaying                               *
 ****************************************************************************/
bool Selector::isDisplaying()
{
   return displaying;
}

/****************************************************************************
 *                                 hide                                     *
 ****************************************************************************/
void Selector::hide()
{
   this->next->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   this->previous->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   this->back->setTargetPosition(HIDDEN_POS, HIDDEN_POS);

   for(int i = 0; i < this->total; i++)
   {
      this->selectables[i]->getButton()->setTargetPosition(HIDDEN_POS, HIDDEN_POS);
   }
}

/****************************************************************************
 *                                 hide                                     *
 ****************************************************************************/
void Selector::hideButtons()
{
   this->next->hide();
   this->previous->hide();
   this->back->hide();

   for(int i = 0; i < this->total; i++)
   {
      this->selectables[i]->getButton()->hide();
   }
}

/****************************************************************************
 *                                  treat                                   *
 ****************************************************************************/
int Selector::treat(int mouseX, int mouseY, bool leftButtonPressed)
{
   if(this->displaying)
   {
      if(this->next->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         /* Go to the next page */
         this->firstDisplayed += this->totalButtonsPerPage;
         if(this->firstDisplayed >= this->total)
         {
            /* The next of last is the first page. */
            this->firstDisplayed = 0;
         }
         show();
      }
      if(this->previous->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         /* Go to previous page */
         this->firstDisplayed -= this->totalButtonsPerPage;
         if(this->firstDisplayed < 0)
         {
            /* The previous of first is the last */
            int remain = this->total % this->totalButtonsPerPage;
            if(remain == 0)
            {
               this->firstDisplayed = this->total - this->totalButtonsPerPage;
            }
            else
            {
               this->firstDisplayed = this->total - remain;
            }
         }
         show();
      }

      if(back->verifyEvents(mouseX, mouseY, leftButtonPressed) ==
            IBUTTON_EVENT_PRESSED)
      {
         hide();
         treatReturn = RETURN_CLOSE;
         return RETURN_OTHER;
      }


      for(int i = 0; i < this->total; i++)
      {
         if(selectables[i]->getButton()->verifyEvents(mouseX, mouseY,
                  leftButtonPressed) == IBUTTON_EVENT_PRESSED)
         {
            this->selected = i;
            hide();
            treatReturn = RETURN_SELECTED;
            return RETURN_OTHER;
         }
      }

      /* Verify if achieved hidden position */
      if((back->getPosX() == HIDDEN_POS) && (!back->isUpdating()))
      {
         this->displaying = false;
         hideButtons();
         return treatReturn;
      }
   }
   return RETURN_OTHER;
}

/****************************************************************************
 *                                getByIndex                                *
 ****************************************************************************/
Selectable* Selector::getByIndex(int i)
{
   if((i >= 0) && (i < total))
   {
      return selectables[i];
   }
   return NULL;

}

/****************************************************************************
 *                                 getTotal                                 *
 ****************************************************************************/
int Selector::getTotal()
{
   return total;
}

/****************************************************************************
 *                                getSelected                               *
 ****************************************************************************/
Selectable* Selector::getSelected()
{
   return getByIndex(this->selected);
}

