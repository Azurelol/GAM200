#include "MainMenuManager.h"
#include "../../CoreComponents.h"

namespace DCEngine {

#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(MainMenuManager, "MainMenuManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_ATTRIBUTE(Level);
		DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelNewGame)->Attributes.push_back(attributeLevel);
		DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelHelp)->Attributes.push_back(attributeLevel);
		DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelCredits)->Attributes.push_back(attributeLevel);
    DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelCrunk)->Attributes.push_back(attributeLevel);
		DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, TransitionTime);
	}
#endif  


	void MainMenuManager::Initialize()
	{
		// Set the references to the buttons
		ButtonNewGame = SpaceRef->FindObjectByName("NewGameButton");
		ButtonHelp = SpaceRef->FindObjectByName("HelpButton");
    ButtonCrunk = SpaceRef->FindObjectByName("CrunkButton");
    ButtonCredits = SpaceRef->FindObjectByName("CreditsButton");
		ButtonExit = SpaceRef->FindObjectByName("ExitButton");
		ButtonFullScreen = SpaceRef->FindObjectByName("FullScreen");

		// Connect to their events
		if (ButtonNewGame)
			Connect(ButtonNewGame, Events::MouseClickedOn, MainMenuManager::OnNewGameClicked);
		else
			DCTrace << "MainMenuManager::Initialize - Failed to find ButtonNewGame! \n";

		if (ButtonHelp)
			Connect(ButtonHelp, Events::MouseClickedOn, MainMenuManager::OnHelpClicked);
		else
			DCTrace << "MainMenuManager::Initialize - Failed to find ButtonHelp! \n";

		if (ButtonCredits)
			Connect(ButtonCredits, Events::MouseClickedOn, MainMenuManager::OnCreditsClicked);
		else
			DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCredits! \n";

		if (ButtonExit)
			Connect(ButtonExit, Events::MouseClickedOn, MainMenuManager::OnExitClicked);
		else
			DCTrace << "MainMenuManager::Initialize - Failed to find ButtonExit! \n";

    if (ButtonCrunk)
      Connect(ButtonCrunk, Events::MouseClickedOn, MainMenuManager::OnCrunkClickedEvent);
    else
      DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCrunk! \n";


		if (ButtonFullScreen)
			Connect(ButtonFullScreen, Events::MouseClickedOn, MainMenuManager::OnFullScreenSwitch);

		else
			DCTrace << "MainMenuManager::Initialize - Failed to find ButtonFullScreen! \n";

	}

	void MainMenuManager::OnKeyDownEvent(Events::KeyDown * event)
	{
	}

	void MainMenuManager::OnKeyUpEvent(Events::KeyDown * event)
	{
	}

	void MainMenuManager::OnNewGameClicked(Events::MouseClickedOn * event)
	{
		DCTrace << "MainMenuManager::OnNewGameClicked \n";
		// Load the first level
		SpaceRef->LoadLevel(LevelNewGame);
	}

	void MainMenuManager::OnHelpClicked(Events::MouseClickedOn * event)
	{
		DCTrace << "MainMenuManager::OnHelpClicked \n";
	}

	void MainMenuManager::OnCreditsClicked(Events::MouseClickedOn * event)
	{
		DCTrace << "MainMenuManager::OnCreditsClicked \n";
	}

	void MainMenuManager::OnExitClicked(Events::MouseClickedOn * event)
	{
		DCTrace << "MainMenuManager::OnExitClicked \n";
		Systems::DispatchSystemEvents::EngineExit();
	}

	void MainMenuManager::OnFullScreenSwitch(Events::MouseClickedOn * event)
	{
		DCTrace << "MainMenuManager::OnFullScreenSwitch \n";
		auto fsevent = new Events::FullscreenEnabledEvent();
		Daisy->Dispatch<Events::FullscreenEnabledEvent>(fsevent);
		delete fsevent;
	}

  void MainMenuManager::OnCrunkClickedEvent(Events::MouseClickedOn * event)
  {
    SpaceRef->LoadLevel(LevelCrunk);
  }

}