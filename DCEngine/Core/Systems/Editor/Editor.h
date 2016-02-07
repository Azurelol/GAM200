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
#pragma once
#include "../System.h"

// Utilities
#include "EditorUtilities.h"
// Modules
#include "EditorModulesInclude.h"
// Tools
#include "EditorToolsInclude.h"
// Engine
#include "../../Objects/Object.h"
#include "../../Objects/ObjectsInclude.h"
#include "../../Objects/Entities/EntitiesInclude.h"


namespace DCEngine {

  // Forward declarations
  class Engine;
  class Space;

  namespace Systems {

    class Editor : public System {
      friend class Engine;

    public:

      SelectionData Selection;
      ObjectPtr SelectedObject();      
      ObjectContainer& AllSelectedObjects();
      void Add(CommandPtr command);
      bool IsEnabled();
      void ToggleEditor();
      void ToggleEditor(bool);

    private:

      enum class EditorTools {
        None,
        Translate,
        Rotate,
        Scale,
        Tilemap,
      };

      ObjectContainer SelectedObjects;
      // Tools
      //TransformToolPtr TransformTool;
      EditorToolPtr ActiveTool;
      void SwitchTool(EditorTools tool);
      // Tools
      void DrawSelection();
      void UseTool(GameObjectPtr gameObject, Vec2& position);
      void ReleaseTool();
      bool IsToolRegion(GameObjectPtr);
      void DragObject(Vec2&);
      void RotateObject(Vec2&);
      void ScaleObject(Vec2&);
      void ReleaseObject();

      // Actions
      void MoveObject(Vec3);
      void ScaleObject(Vec3);

      /////////////////
      //  Settings 
      ////////////////
      EditorConfig Settings;
      TransformToolData TransformData;
      void setEnabled(bool);
      std::string RecentProject;
      bool WidgetMenuBarEnabled = false;
      bool WidgetLevelEnabled = false;
      bool WindowAddResourceEnabled = false;
      bool WidgetObjectsEnabled = false;
      bool WindowPropertiesEnabled = false;
      bool WidgetLibraryEnabled = false;
      bool WindowDiagnosticsEnabled = false;
      bool WindowSaveLevelEnabled = false;
      bool WindowLoadLevelEnabled = false;
      bool WindowConsoleEnabled = false;
      bool WindowToolsEnabled = false;
      bool WindowCreateFromArchetypeEnabled = false;
      bool WindowCollisionTableEditorEnabled = false;
      bool WindowSpriteLayerOrderEditorEnabled = false;
      bool WindowCommandsEnabled = false;
      SystemPtr ReflectionSystem;
      Space* CurrentSpace;
      GameObjectPtr EditorCamera = nullptr;
      //GameObjectPtr TransformTool = nullptr;
      Vec2 ViewportResize = Vec2(0.75, 0.75);

      /////////////////
      //  Methods 
      ////////////////
      void DisplayEditor();
      // Windows
      void DisplayMainMenuBar();
      void WidgetLevel();
      void WidgetResourceAdd();
      // Objects
      void WindowObjects();
      void ObjectsListSwapPosition(GameObjectPtr, Direction);
      // Tools
      void WindowTools();
      // Properties
      void WindowProperties();
      bool DisplayProperties(ObjectPtr);
      void DisplayEntityProperties();
      void DisplayResourceProperties();
      bool AddComponent(EntityPtr);
      // Library
      void WindowLibrary();
      //template <typename ResourceMap> void DisplayResourceTree(std::string resourceName, ResourceMap resourceMap, Delegate* function);
      void WindowDiagnostics();
      void WindowSaveLevel();
      void WindowLoadLevel();
      void WindowConsole();
      void WindowCreateFromArchetype();
      // Archetypes
      void SaveArchetype(std::string&);
      void RevertToArchetype();
      // Project
      void NewProject();
      void ArchiveProject();
      void OpenProject();
      void OpenRecentProject();
      void LoadProject(std::string path);
      void SaveProject();
      void SaveCurrentLevel();
      void PlayGame();
      void StopGame();
      void ExportGame();
      void Exit();
      // Select
      void SelectObjectFromSpace(GameObject*);
      GameObject* FindObjectFromSpace(Vec2 pos);
      GameObjectPtr IsSelectableGameObject(ObjectPtr);
      void SelectObject(GameObject* obj);
      void Select(ObjectPtr);
      void Deselect();
      void SelectSpace();
      void CenterSelected();
      void SelectMultiple(Vec2&);
      void CalculateSelectionBounding();
      void DrawMultiSelect();
      // Resources
      void WindowAddResource();
      void ResourceCreate(std::string& name, ResourceType type);
      void ResourceAddFromFile(std::string& name, ResourceType type);
      bool CreateLevel(std::string&);
      ResourcePtr CreateBank(std::string& name, std::string& assetPath);
      ResourcePtr CreateCollisionGroup(std::string& name);
      ResourcePtr CreateCollisionTable(std::string& name);
      ResourcePtr CreatePhysicsMaterial(std::string& name);
      ResourcePtr CreateSpriteSource(std::string& name, std::string& assetPath);
      ResourcePtr CreateSoundCue(std::string& name, std::string& assetPath);
      ResourcePtr CreateZilchScript(std::string& name);
      ResourcePtr CreateFont(std::string& name, std::string& assetPath);
      ResourcePtr CreateSpriteLayer(std::string& name);
      ResourcePtr CreateSpriteLayerOrder(std::string& name);
      bool SelectEnumeration(Zilch::Property*, ObjectPtr, unsigned int&);
      bool SelectResource(Zilch::Property*, ObjectPtr, unsigned int&);
      template <typename ResourceMap>
      bool SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID);
      void WindowCollisionTableEditor();
      void WindowSpriteLayerOrderEditor();
      ResourcePtr SelectedCollisionTable;
      SpriteLayerOrderPtr SelectedSpriteLayerOrder;
      bool LoadLevel(std::string level);
      bool SaveLevel(std::string level);
      bool ReloadLevel();
      // Object Selection
      void WindowCommands();
      void Undo();
      void Redo();
      void Cut();
      void Copy();
      void Paste();
      void Duplicate();
      void DeleteObject();
      void DeleteResource(ResourcePtr);
      // Window, Input
      void ApplyEditorWindowLayout();
      void SetEditorCamera(bool);
      void Hotkeys(Events::KeyDown* event);
      void UpdateCaption();
      void PanCamera(Vec2&);
      void DrawGrid();
      // Create
      void CreateTransform();
      void CreateSprite();
      void CreateSpriteText();
      void CreateParticleSystem();
      void CreateFromArchetype(std::string&);
      void MoveToViewportCenter(GameObject* gameobject);
      // Processes      
      void LaunchProjectFolder();
      void LaunchDocumentation();
      // CTOR
      Editor(EditorConfig settings);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      // Events
      void OnEditorEnabledEvent(Events::EditorEnabled* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnMouseUpdateEvent(Events::MouseUpdate* event);

    };
    
  }
}

#include "Editor.hpp"
