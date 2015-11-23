/******************************************************************************/
/*!
@file   Properties.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's properties widget.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"
#include "../../Systems/Reflection/ZilchInterface.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetProperties()
    {
      if (!WidgetPropertiesEnabled)
        return;
      
      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Properties", &WidgetPropertiesEnabled);

      // If there's an object selected, display its properties.
      if (SelectedObject != nullptr) {
        // 1. Display the object's name
        char name[32]; strcpy(name, SelectedObject->Name().c_str());
        // If the user has given input, change the name
        if (ImGui::InputText("Name", name, IM_ARRAYSIZE(name))) {
          SelectedObject->setName(name);
        }

        // 2. Display the object's archetype
        char archetypeName[32]; 
        strcpy(archetypeName, SelectedObject->getArchetype().c_str());
        // If the user has given input, change the archetype
        if (ImGui::InputText("Archetype", archetypeName, IM_ARRAYSIZE(archetypeName))) {
          SelectedObject->setArchetype(archetypeName);
        }

        // 3. Display its components
        ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
        for (auto &component : *SelectedObject->AllComponents()) {
          if (ImGui::TreeNode(component->Name().c_str())) {
            ImGui::SetWindowFocus();
            // 3. If the user clicks on a tree-node, display the commponent's properties
            //    through reflection
            DisplayProperties(component.get());
            ImGui::TreePop();
          }                  
        }

        // 4. Allow the user to add new components
        AddComponent();

      }

      ImGui::End();
    }

    

    /**************************************************************************/
    /*!
    @brief  Displays the component's properties, allowing them to be
            seen and edited.
    @param  component A pointer to the Component.
    @note   Properties are listed from the order they were defined.
    @todo   Refactor the property-setting so I DONT COPY PASTE THE SAME
            3 LINES.
    */
    /**************************************************************************/
    void Editor::DisplayProperties(ComponentPtr component) {
      // 1. Get the component's BoundType, which has a wealth of reflected data
      auto componentBoundType = component->ZilchGetDerivedType();
      if (componentBoundType == nullptr)
        return;
      // 2. Get a list of all properties on the object
      for (auto property : componentBoundType->AllProperties) {
        
        // If the bound field/property does not have the Property Attribute, do
        // nothing.
        //if (!property->HasAttribute("Property"))
        //  continue;              

        //auto reflection = Daisy->getSystem<Reflection>()->Handler();
        
        // Create an exception report object
        Zilch::ExceptionReport report;
        // Grab the current property
        Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
        getCall.SetHandleVirtual(Zilch::Call::This, component);        
        getCall.Invoke(report);

        // Property: Boolean
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean))) {
          auto boolean = getCall.Get<Zilch::Boolean>(Zilch::Call::Return);
          // If the user modifies it
          if (ImGui::Checkbox(property->Name.c_str(), &boolean)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, boolean);
            setCall.Invoke(report);
          }
        }

        // Property: String
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String))) {
          auto string = getCall.Get<Zilch::String>(Zilch::Call::Return);
          char buf[32];
          strcpy(buf, string.c_str());
          // If the user has given input, set the property
          if (ImGui::InputText(property->Name.c_str(), buf, IM_ARRAYSIZE(buf)), ImGuiInputTextFlags_EnterReturnsTrue) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::String(buf));
            setCall.Invoke(report);
          }
        }

        // Property: Integer
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
          auto integer = getCall.Get<Zilch::Integer>(Zilch::Call::Return);
          // If the user has given input, set the property
          if (ImGui::InputInt(property->Name.c_str(), &integer)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, integer);
            setCall.Invoke(report);
          }
        }

        // Property: Integer2
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer2))) {
          auto integer2 = getCall.Get<Zilch::Integer2>(Zilch::Call::Return);
          int int2[2] = { integer2.x, integer2.y };
          // If the user has given input, set the property
          if (ImGui::InputInt2(property->Name.c_str(), int2)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer2(int2[0], int2[1]));
            setCall.Invoke(report);
          }
        }

        // Property: Integer3
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer3))) {
          auto integer3 = getCall.Get<Zilch::Integer3>(Zilch::Call::Return);
          int int3[3] = { integer3.x, integer3.y, integer3.z };
          // If the user has given input, set the property
          if (ImGui::InputInt3(property->Name.c_str(), int3)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer3(int3[0], int3[1], int3[2]));
            setCall.Invoke(report);
          }
        }

        // Property: Integer4
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
          auto integer4 = getCall.Get<Zilch::Integer4>(Zilch::Call::Return);
          int int4[4] = { integer4.x, integer4.y, integer4.z, integer4.w};
          // If the user has given input, set the property
          if (ImGui::InputInt4(property->Name.c_str(), int4)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Integer4(int4[0], int4[1], int4[2], int4[3]));
            setCall.Invoke(report);
          }
        }

        // Property: Real (float)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
          auto real = getCall.Get<Zilch::Real>(Zilch::Call::Return);
          // If the user has given input, set the property
          if (ImGui::InputFloat(property->Name.c_str(), &real, 0.01f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, real);
            setCall.Invoke(report);
          }
        }

        // Property: Real2 (Vec2)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2))) {
          auto vec2 = getCall.Get<Zilch::Real2>(Zilch::Call::Return);
          float vec2f[2] = { vec2.x, vec2.y };
          ImGui::Text(property->Name.c_str());
          // If the user has given input, set the property
          if (ImGui::InputFloat2("", vec2f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real3(vec2f));
            setCall.Invoke(report);
          }
        }

        // Property: Real3 (Vec3)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3))) {
          auto vec3 = getCall.Get<Zilch::Real3>(Zilch::Call::Return);
          float vec3f[3] = { vec3.x, vec3.y, vec3.z };
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputFloat3("", vec3f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real3(vec3f));
            setCall.Invoke(report);
          }
        }        

        // Property: Real4 (Vec4)
        if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4))) {
          auto vec4 = getCall.Get<Zilch::Real4>(Zilch::Call::Return);
          float vec4f[4] = { vec4.x, vec4.y, vec4.z, vec4.w};
          // If the user has given input, set the property
          ImGui::Text(property->Name.c_str());
          if (ImGui::InputFloat4("", vec4f)) {
            Zilch::Call setCall(property->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
            setCall.SetHandleVirtual(Zilch::Call::This, component);
            setCall.Set(0, Zilch::Real4(vec4f[0], vec4f[1], vec4f[2], vec4f[3]));
            setCall.Invoke(report);
          }
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to add a component to the entity.
    @todo   Refactor the property-setting so I DONT COPY PASTE THE SAME
    3 LINES.
    */
    /**************************************************************************/
    void Editor::AddComponent()
    {
      // Grab a container of all bound components.. 
      auto components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
      //DCTrace << "The following components have been bound to Zilch: \n";
      if (ImGui::TreeNode("Add Component")) {
        for (auto component : components) {
          auto name = std::string(component->Name.c_str());
          if (ImGui::Selectable(name.c_str())) {
            DCTrace << "Editor::AddComponent - Adding " << name << " to " << SelectedObject->Name() << "\n";
            // Add the component on the entity and initialize it
            //SelectedObject->AddComponentByType(component);
            SelectedObject->AddComponentByName(name, true);
            //SelectedObject->AddComponentByName(name, false);
            //auto rtti = std::type_index(typeid(component->));
            //SelectedObject->AddComponent<rtti>();
          }          
        }
        ImGui::TreePop();
      }



    }


  }
}