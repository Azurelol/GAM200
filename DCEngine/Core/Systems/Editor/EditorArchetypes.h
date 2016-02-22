/******************************************************************************/
/*!
@file   EditorArchetypes.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Resources/Archetype.h"
#include "../System.h"

namespace DCEngine {

  class GameObject;
  class Space;

  namespace Systems {

    class Editor;
    class EditorArchetypes : public Module {
    public:
      EditorArchetypes(Editor& editor);
      ~EditorArchetypes();
      void Select(ArchetypeHandle archetype);
      void Preview();      
      void UploadArchetype(ArchetypeHandle archetype);      
      void RevertToArchetype(EntityPtr entity);

    private:
      void Deselect();
      void ConstructSpace();
      void UpdateArchetypeInstances(ArchetypeHandle archetypeName);

      void OnEngineInitializedEvent(Events::EngineInitialized* event);
      void OnEditorSelectObjectEvent(Events::EditorSelectObject* event);
      void OnEditorDeselectObjectEvent(Events::EditorDeselectObject* event);

      Editor& EditorRef;
      GameObject* CurrentArchetype;      
      Space* ArchetypeSpace;


    };

  }
}