/******************************************************************************/
/*!
@file   Editor.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/24/2015
@brief  The editor allows the real-time editing of gameobjects in levels ,
then doing serialization of the levels.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor.
    */
    /**************************************************************************/
    Editor::Editor(EditorConfig settings) : System(std::string("EditorSystem"), EnumeratedSystem::Editor), 
                                                          Settings(settings)
    {      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the Editor system.
    */
    /**************************************************************************/
    void Editor::Initialize()
    {
      //auto& a = Settings;

      if (TRACE_INITIALIZE)
        DCTrace << "Editor::Initialize \n";
      // Store a reference to the Reflection System
      ReflectionSystem = Daisy->getSystem<Reflection>();
      // Subscribe to events
      Subscribe();
      // Set the default space for the editor to work on
      CurrentSpace = Daisy->getGameSession()->getDefaultSpace();
    }

    /**************************************************************************/
    /*!
    \brief  Subscribe to events.
    */
    /**************************************************************************/
    void Editor::Subscribe()
    {
      Daisy->Connect<Events::MouseDown>(Daisy->getMouse(), &Editor::OnMouseDownEvent, this);
      Daisy->Connect<Events::MouseUp>(Daisy->getMouse(), &Editor::OnMouseUpEvent, this);
      Daisy->Connect<Events::MouseUpdate>(Daisy->getMouse(), &Editor::OnMouseUpdateEvent, this);
      Daisy->Connect<Events::KeyDown>(Daisy->getKeyboard(), &Editor::OnKeyDownEvent, this);
      Daisy->Connect<Events::EditorEnabled>(&Editor::OnEditorEnabledEvent, this);
    }

    /**************************************************************************/
    /*!
    \brief  Event received by Input.
    */
    /**************************************************************************/
    void Editor::OnEditorEnabledEvent(Events::EditorEnabled * event)
    {
      DCTrace << "Editor::OnEditorEnabledEvent \n";
      ToggleEditor();
    }


    /**************************************************************************/
    /*!
    \brief  Updates the Editor system.
    */
    /**************************************************************************/
    void Editor::Update(float dt)
    {
      // Only enable the editor while the GUI is enabled
      if (!GUI_ENABLED)
        return;
     
      if (TRACE_UPDATE)
        DCTrace << "Editor::Update \n";
      ShowSelection();
      DisplayEditor();
      UseTool();
      
    }

    /**************************************************************************/
    /*!
    @brief  Sets the current status of the editor.
    */
    /**************************************************************************/
    void Editor::setEnabled(bool set)
    {
      Settings.EditorEnabled = set;
      DCTrace << "Editor::setEnabled : " << std::boolalpha << Settings.EditorEnabled << "\n";
    }


    /**************************************************************************/
    /*!
    @brief  Returns the current state of the editor.
    */
    /**************************************************************************/
    bool Editor::IsEnabled()
    {
      return Settings.EditorEnabled;
    }

    /**************************************************************************/
    /*!
    \brief  Toggled the Editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor()
    {
      DCTrace << "*!* Editor::ToggleEditor \n";
      // Resize the viewport to accomodate the editor
      //ApplyEditorWindowLayout();
      
      // Toggle on the editor
      setEnabled(!Settings.EditorEnabled);
      ToggleEditor(Settings.EditorEnabled);      
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor(bool toggle)
    {
      // Editor ON
      if (toggle) {
        setEnabled(true);
        // Pause the engine (Physics, Input, Events)
        DispatchSystemEvents::EnginePause();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EnginePaused' event \n";
        // Quit the Game
        DispatchGameEvents::GameEnded();
        // Reload the level
        ReloadLevel();
        // Toggle the widgets
        WidgetLibraryEnabled = true;
        WidgetObjectsEnabled = true;
      }
      // Editor OFF
      else {
        setEnabled(false);
        // Unpause the engine (Physics, Input, Events)
        DispatchSystemEvents::EngineResume();
        // Send the game start event
        DispatchGameEvents::GameStarted();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EngineResume' event \n";
        // Set the editor camera
        SetEditorCamera(false);
        Deselect();
        // Ask the space to reload the level
        CurrentSpace->ReloadLevel();
      }
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the ImGui Test Window on and off.
    @todo   Switch to using a stack of active windows rather than this hackery.
    */
    /**************************************************************************/
    void Editor::ToggleTest()
    {
      ShowTestWindow = !ShowTestWindow;
      DCTrace << "Editor::ToggleTest : " << ShowTestWindow << "\n";
    }

    /**************************************************************************/
    /*!
    @brief  Displays every created Editor window.
    */
    /**************************************************************************/
    void Editor::DisplayEditor()
    {
      if (!Settings.EditorEnabled)
        return;

      // Display all known editor windows
      DisplayMainMenuBar();
      WidgetLevel();
      WindowObjects();
      WidgetLibrary();
      WindowProperties();
      WidgetDiagnostics();
      WindowSaveLevel();
      WindowLoadLevel();    
      WindowConsole();
      WindowCreateFromArchetype();
      WindowTools();
      WindowAddResource();
      WindowCollisionTableEditor();
      WindowCommands();
    }

    /**************************************************************************/
    /*!
    \brief  Allows the user a variety of settings regarding levels, such as
            reloading the current level, loading a new one, or saving the
            current one.
    */
    /**************************************************************************/
    void Editor::WidgetLevel()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Allows the user to add a resource to the current project's library
            of assets.
    */
    /**************************************************************************/
    void Editor::WidgetResourceAdd()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Terminates the editor system.
    */
    /**************************************************************************/
    void Editor::Terminate()
    {
      DCTrace << "Editor::Terminate \n";
    }



    



  }
}