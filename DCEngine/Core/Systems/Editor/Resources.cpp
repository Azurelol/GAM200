/******************************************************************************/
/*!
@file   MenuResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's resource menu.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {

    void Editor::AddResource()
    {

    }



    /**************************************************************************/
    /*!
    @brief  Loads the level, deserializing a level file's binary data and
            constructing gameobjects along with their components.
    */
    /**************************************************************************/
    void Editor::LoadLevel(std::string level)
    {
      DCTrace << "Editor::LoadLevel - Loading " << level << "\n";
      
      // Query the Content system for the Level resource 
      auto levelPtr = Daisy->getSystem<Systems::Content>()->getLevel(level);
      SelectedObject = nullptr;
      // Load the level
      CurrentSpace->LoadLevel(level);
      // Load the editor camera
      SetEditorCamera();

      //std::string levelData = 
    }

    /**************************************************************************/
    /*!
    @brief  Saves the current level, serializing all of the active gameobjects
            along with their components in the space.
    @todo   The level path is currently hardcoded. Change that.
    */
    /**************************************************************************/
    void Editor::SaveLevel(std::string level)
    {
      DCTrace << "Editor::SaveLevel - Saving " << level << "\n";
      
      // Get the current project's path
      std::string LevelPath("Projects/Rebound/Resources/Levels/");
      auto levelResource = CurrentSpace->SaveLevel(LevelPath + level + std::string(".lvl"));
      //Daisy->getSystem<Systems::Content>()->AddLevel(levelResource->Name(), levelResource);

    }

    /**************************************************************************/
    /*!
    @brief Reloads the currently loaded level.
    */
    /**************************************************************************/
    void Editor::ReloadLevel()
    {
      // Reload the current level
      CurrentSpace->ReloadLevel();
      // Reload the editor camera
      SetEditorCamera();
    }

    void Editor::LoadDollhouse()
    {
      SelectedObject = nullptr;
      // !!! TESTING: Level loading
      LevelPtr dollhouse = LevelPtr(new DollHouse(*CurrentSpace, CurrentSpace->getGameSession()));
      CurrentSpace->LoadSampleLevel(dollhouse);      
      // Load the editor camera
      SetEditorCamera();
    }

  }
}