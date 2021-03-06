/*****************************************************************************/
/*!
@file   MainMenuManager.h
@author Christian Sagel
@par    email: connor.tilley\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
    
  namespace Components
  {
    class MainMenuManager : public Component {

    public:
      // Properties    
      DCE_DEFINE_PROPERTY(LevelHandle, LevelNewGame);
      DCE_DEFINE_PROPERTY(LevelHandle, LevelHelp);
      DCE_DEFINE_PROPERTY(LevelHandle, LevelCredits);
      DCE_DEFINE_PROPERTY(LevelHandle, LevelCrunk);
      DCE_DEFINE_PROPERTY(Real, TransitionTime);

      // Methods
      MainMenuManager(Entity& owner) : Component(std::string("MainMenuManager"), owner) {}
      void Initialize();
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);

      // Main Menu
      void OnNewGameClicked(Events::MouseClickedOn* event);
      void OnHelpClicked(Events::MouseClickedOn* event);
      void OnCreditsClicked(Events::MouseClickedOn* event);
      void OnExitClicked(Events::MouseClickedOn* event);
      void OnFullScreenSwitch(Events::MouseClickedOn* event);
      void OnCrunkClickedEvent(Events::MouseClickedOn* event);

      // Confirm
      void OnConfirmQuitClicked(Events::MouseClickedOn* event);
      void OnCancelQuitClicked(Events::MouseClickedOn* event);


#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(MainMenuManager, Component);
#endif

    private:

      bool DisplayConfirmationActive = false;
      void DisplayConfirmQuit(bool);

      // Properties
      LevelHandle LevelNewGame;
      LevelHandle LevelHelp;
      LevelHandle LevelCredits;
      LevelHandle LevelCrunk;
      Real TransitionTime = 0;

      // Main Menu
      GameObjectPtr ButtonNewGame;
      GameObjectPtr ButtonCrunk;
      GameObjectPtr ButtonHelp;
      GameObjectPtr ButtonCredits;
      GameObjectPtr ButtonExit;
      GameObjectPtr ButtonFullScreen;

      // Confirm
      GameObjectPtr ButtonConfirm;
      GameObjectPtr ButtonCancel;

    };

  }
}