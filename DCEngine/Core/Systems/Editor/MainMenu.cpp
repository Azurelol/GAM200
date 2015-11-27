/******************************************************************************/
/*!
@file   MainMenu.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's main menu bar.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the menu bar for the editor, which contains all available
    widgets. It allows them to be toggled on and off.
    */
    /**************************************************************************/
    void Editor::DisplayMainMenuBar()
    {
      if (ImGui::BeginMainMenuBar()) {

        // Project
        if (ImGui::BeginMenu("Project")) {
          if (ImGui::MenuItem("New Project"))
            NewProject();          
          if (ImGui::MenuItem("Archive Project"))
            ArchiveProject();
          if (ImGui::MenuItem("Open Project"))
            OpenProject();
          if (ImGui::MenuItem("Save Project"))
            SaveProject();
          if (ImGui::MenuItem("Show Project Folder"))
            ShowProjectFolder();
          ImGui::Separator();
          if (ImGui::MenuItem("Play Game"))
            PlayGame();
          if (ImGui::MenuItem("Stop Game"))
            StopGame();
          if (ImGui::MenuItem("Exit"))
            Exit();
          ImGui::EndMenu();
        }

        // Edit
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
          if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Cut", "CTRL+X")) {}
          if (ImGui::MenuItem("Copy", "CTRL+C")) {}
          if (ImGui::MenuItem("Paste", "CTRL+V")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Duplicate", "CTRL+D")) {
            DuplicateObject();
          }
          if (ImGui::MenuItem("Delete", "Del")) {
            DeleteObject();
          }
          ImGui::EndMenu();
        }

        // Create
        if (ImGui::BeginMenu("Create")) {
          if (ImGui::MenuItem("Create Transform")) {
            CreateTransform();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Create Sprite")) {
            CreateSprite();
          }
          if (ImGui::MenuItem("Create SpriteText")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create Particle System")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Create from Archetype"))
            WindowCreateFromArchetypeEnabled = true;
          ImGui::EndMenu();
        }

        // Select
        if (ImGui::BeginMenu("Select")) {
          if (ImGui::MenuItem("Previous Selection")) {}
          if (ImGui::MenuItem("Next Selection")) {}
          ImGui::Separator();
          if (ImGui::MenuItem("Select None")) {};
          if (ImGui::MenuItem("Select All")) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Center Selected")) {};
          if (ImGui::MenuItem("Frame Selected")) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Group Selected")) {};
          ImGui::Separator();
          if (ImGui::MenuItem("Select Space"))
            SelectSpace();
          if (ImGui::MenuItem("Select Game")) {};
          ImGui::EndMenu();
        }


        // Resources
        if (ImGui::BeginMenu("Resources")) {
          if (ImGui::MenuItem("Add")) {
            AddResource();
          }
          ImGui::Separator();
          if (ImGui::MenuItem("Load Level"))
            WindowLoadLevelEnabled = !WindowLoadLevelEnabled;
          if (ImGui::MenuItem("Save Level"))
            WindowSaveLevelEnabled = !WindowSaveLevelEnabled;
          if (ImGui::MenuItem("Reload Level"))
            ReloadLevel();
          ImGui::Separator();
          if (ImGui::MenuItem("Load Dollhouse"))
            LoadDollhouse();
          //ReloadLevel();
          ImGui::EndMenu();
        }

        // Windows
        if (ImGui::BeginMenu("Windows")) {
          if (ImGui::MenuItem("Properties"))
            WindowPropertiesEnabled = !WindowPropertiesEnabled;
          if (ImGui::MenuItem("Library"))
            WidgetLibraryEnabled = !WidgetLibraryEnabled;
          if (ImGui::MenuItem("Objects"))
            WidgetObjectsEnabled = !WidgetObjectsEnabled;
          if (ImGui::MenuItem("Diagnostics"))
            WidgetDiagnosticsEnabled = !WidgetDiagnosticsEnabled;
          if (ImGui::MenuItem("Console"))
            WindowConsoleEnabled = !WindowConsoleEnabled;
          ImGui::EndMenu();
        }

        // Help
        if (ImGui::BeginMenu("Help")) {
          if (ImGui::MenuItem("Documentation")) {}
          if (ImGui::MenuItem("Report Bug", false)) {}  // Disabled item
          if (ImGui::MenuItem("Controls", false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Release Notes")) {}
          if (ImGui::MenuItem("About")) {}

          ImGui::EndMenu();
        }
        
        //ImGui::PushID(1);
        ImGui::BeginChild("Project2", ImVec2(30, 30));
        ImGui::EndChild();

        ImGui::EndMainMenuBar();

      }

    }

  }
}