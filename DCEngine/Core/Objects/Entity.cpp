/*/******************************************************************************/
/*!
@file   Entity.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/18/2015
@brief  The base object composition class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Entity.h"
#include "Entities\EntityProperties.h"

// Headers
#include "Entities\Space.h"
#include "Entities\GameSession.h"
#include "../Engine/Engine.h" 

ZilchDefineExternalType(DCEngine::EntityType, "EntityType", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::EntityType::GameObject, "GameObject");
  ZilchBindEnumValue(builder, type, DCEngine::EntityType::Space, "Space");
  ZilchBindEnumValue(builder, type, DCEngine::EntityType::GameSession, "GameSession");
}

namespace DCEngine {
  
  /*!************************************************************************\
  @brief  Entity Definition
  \**************************************************************************/
  ZilchDefineType(Entity, "Entity", DCEngineCore, builder, type) {
    DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Entity, "name", std::string);
    ZilchBindDestructor(builder, type, Entity);
    // Properties
    DCE_BINDING_DEFINE_ATTRIBUTE(Hidden); DCE_BINDING_DEFINE_ATTRIBUTE(Skip);
    DCE_BINDING_DEFINE_PROPERTY(Entity, ModifiedFromArchetype);
    ZilchBindProperty(builder, type, &Entity::getArchetype, &Entity::setArchetype, "Archetype");
    DCE_BINDING_DEFINE_PROPERTY(Entity, Protected);
    DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyModifiedFromArchetype, attributeHidden);
    DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyModifiedFromArchetype, attributeSkip);
    // Methods
    Zilch::ParameterArray dispatchParams;
    dispatchParams.push_back(ZilchTypeId(Zilch::String));
    dispatchParams.push_back(ZilchTypeId(Event));
    ZilchBindMethod(builder, type, &Entity::Dispatch, (void(Entity::*)(std::string, Event*)), "Dispatch", "eventID, event");    
    ZilchBindMethod(builder, type, &Entity::AddComponentByName, ZilchNoOverload, "AddComponentByName", "name, initialize");
    ZilchBindProperty(builder, type, &Entity::Parent, ZilchNoSetter, "Parent", ZilchNoNames);
    ZilchBindMethod(builder, type, &Entity::FindChildByName, ZilchNoOverload, "FindChildByName", "name");
    ZilchBindProperty(builder, type, &Entity::ChildrenByRange, ZilchNoSetter, "Children");
    ZilchBindMethod(builder, type, &Entity::AttachTo, ZilchNoOverload, "AttachTo", "parent");    
    ZilchBindMethod(builder, type, &Entity::AttachToRelative, ZilchNoOverload, "AttachToRelative", "parent");
    ZilchBindMethod(builder, type, &Entity::Detach, ZilchNoOverload, "Detach", ZilchNoNames);
    ZilchBindMethod(builder, type, &Entity::DetachRelative, ZilchNoOverload, "DetachRelative", ZilchNoNames);
    ZilchBindMethod(builder, type, &Entity::Destroy, ZilchNoOverload, "Destroy", ZilchNoNames);
    DCE_BINDING_DEFINE_PROPERTY_NOSETTER(Entity, Actions);

    // Engine-component properties
    //DCE_BINDING_ENTITY_COMPONENT_AS_PROPERTY(Transform);
    //ZilchBindProperty(builder, type, &Entity::getComponent<Components::Transform>, ZilchNoSetter, "Transform");
  }

  /**************************************************************************/
  /*!
  \brief Returns the entity pointer to this object.. if it's one.
  \param object A pointer to the object.
  \return A valid entity pointer if the object was an entity.
  */
  /**************************************************************************/
  EntityPtr Entity::IsA(ObjectPtr object) 
  {
    return dynamic_cast<EntityPtr>(object);
  }

  /**************************************************************************/
  /*!
  \brief  Entity constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) : Object(name), IsInitialized(false), 
                                     ModifiedFromArchetype(false), Protected(false),
                                     ArchetypeName(""), Actions(*this), ParentRef(nullptr) {
  }

  /**************************************************************************/
  /*!
  \brief  Entity destructor.
  */
  /**************************************************************************/
  Entity::~Entity()
  {
    if (DCE_TRACE_GAMEOBJECT_DESTRUCTOR)
      DCTrace << Name() << "::~Entity - Destructor called! \n";

    // 1. Remove all components from the entity
    RemoveAllComponents();

    // 2. Inform all observers of this entity's death
    InformObserversOfDeath();
  }

  /**************************************************************************/
  /*!
  \brief  Initialize all of the entity's components.
  */
  /**************************************************************************/
  void Entity::Initialize() {
    // If the entity has already been initialized, do naught
    if (IsInitialized) {
      DCTrace << ObjectName << "::Initialize - Failed! Already initialized!\n";
      return;
    }
    
    //DCTrace << "Initializing " << Name() << "\n";

    // Initialize factory-created components
    for (auto &component : ComponentsContainer)
      component.get()->Initialize();
    // Initialize Zilch-created components
    for (auto& component : ComponentHandlesContainer) {
      reinterpret_cast<Component*>(component.Dereference())->Initialize();
    }
    // Flag this entity as already being initialized
    IsInitialized = true;
    if (DCE_TRACE_COMPONENT_INITIALIZE)
      DCTrace << ObjectName << "::Initialize \n";
  }

  /**************************************************************************/
  /*!
  \brief Terminates the Entity.
  */
  /**************************************************************************/
  void Entity::Terminate()
  {

  }

  /**************************************************************************/
  /*!
  @brief Serializes an entity
  @param builder A reference to the JSON builder.
  @note  This will serialize the entity's properties, then its components.
  */
  /**************************************************************************/
  void Entity::Serialize(Zilch::JsonBuilder & builder)
  {
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    // Serialize Object-specific properties
    Object::Serialize(builder);
    // Serialize Entity-specific properties
    SerializeByType(builder, interface->GetState(), ZilchTypeId(Entity), this);
    // Serialize all of its components
    builder.Key("Components");
    //DCTrace << "After Components: \n" << builder.ToString().c_str() << "\n";
    builder.Begin(Zilch::JsonType::Object);
    // Factory-created components
    for (auto& component : ComponentsContainer) {
      component->Serialize(builder);
    }
    // Zilch-created components
    for (auto& component : ComponentHandlesContainer) {
      reinterpret_cast<Component*>(component.Dereference())->Serialize(builder);
    }
    builder.End();

  }

  /**************************************************************************/
  /*!
  @brief Deserializes an entity
  @param properties A pointer to the object containing the properties.
  @note  This will deserialize the entity's properties, then its components.
  */
  /**************************************************************************/
  void Entity::Deserialize(Zilch::JsonValue * properties)
  {
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    DeserializeByType(properties, interface->GetState(), ZilchTypeId(Entity), this);
  }

  /**************************************************************************/
  /*!
  @brief  Reconstructs an entity by rebuilding its components.
  */
  /**************************************************************************/
  void Entity::Rebuild()
  {
    Daisy->getSystem<Systems::Factory>()->MarkForRebuild(this);
  }

  /**************************************************************************/
  /*!
  @brief  Adds a component onto the entity by name.
  @param  name The name of the component.
  @bool   Whether to initialize the component.
  @return A pointer to the component.
  @todo   Do this dynamically rather than using string->class mapping.
  */
  /**************************************************************************/
  ComponentPtr Entity::AddComponentByName(const std::string & name, bool initialize)
  {
    // If the entity already has the component, do nothing.
    if (auto a = this->HasComponent(name)) {
      DCTrace << ObjectName << "::AddComponentByName - " << name << " is already present!\n";
      return nullptr;
    }

    #if (DCE_FACTORY_CONSTRUCT_COMPONENTS)
    auto Factory = Daisy->getSystem<Systems::Factory>();
    auto component = Factory->CreateComponentByName(name, *this);

    // If the component could not be constructed... 
    if (component == nullptr) {
      if (DCE_TRACE_COMPONENT_ADD)
        DCTrace << ObjectName << "::AddComponentByName - " << name << " could not be added!\n";
      return nullptr;
    }

    ComponentsContainer.emplace_back(std::move(component));
    if (DCE_TRACE_COMPONENT_ADD)
      DCTrace << ObjectName << "::AddComponentByName - " << name << " has been added!\n";

    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();

    // Returns a pointer to the just-added component
    return ComponentsContainer.back().get();
    
    #else // Zilch-created components
    // Construct the component through Zilch, getting a handle
    auto componentHandle = Daisy->getSystem<Systems::Factory>()->CreateComponentByNameFromZilch(name, *this);
    if (componentHandle.IsNull())
      return nullptr;

    // Add the component's handle to the container
    ComponentHandlesContainer.push_back(componentHandle);
    // Get a pointer to it
    auto componentPtr = Component::Dereference(componentHandle);
    // Save the handle to the component
    componentPtr->mHandle = componentHandle;
    // Initialize the component if need be
    if (initialize)
      componentPtr->Initialize();
    return componentPtr;
    #endif


  }

  /**************************************************************************/
  /*!
  @brief  Adds a component onto the entity by Zilch::BoundType. This is a
  pointer data about the bound component.
  @param  boundType A pointer to the component's BoundType.
  */
  /**************************************************************************/
  bool Entity::AddComponentByType(Zilch::BoundType * boundType, bool initialize)
  {
    auto Factory = Daisy->getSystem<Systems::Factory>();
    ComponentsContainer.emplace_back(std::move(Factory->CreateComponentByType(boundType, *this)));
    if (DCE_TRACE_COMPONENT_ADD)
      DCTrace << "Entity::AddComponentByType - " << std::string(boundType->Name.c_str()) << "\n";
    // Initialize the component if need be
    if (initialize)
      ComponentsContainer.back().get()->Initialize();
    return true;
  }

  /**************************************************************************/
  /*!
  @brief  Checks whether the entity has the component already.
  @param  name The name of the component.
  @return Whether it has the component attached to it already.
  */
  /**************************************************************************/
  bool Entity::HasComponent(const std::string & name)
  {
    for (auto& component : ComponentsContainer) {
      if (component->getObjectName() == name) {
        return true;
      }
    }
    for (auto& component : ComponentHandlesContainer) {
      if (reinterpret_cast<Component*>(component.Dereference())->getObjectName() == name) {
        return true;
      }
    }


    // DCTrace << ObjectName << "::HasComponent - '" << name
    //   << "' is not present. \n";
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Removes all components from the entity.
  */
  /**************************************************************************/
  void Entity::RemoveAllComponents()
  {
    // C++- Factory-made components
    for (auto& component : ComponentsContainer) {
      component->Terminate();
    }
    ComponentsContainer.clear();

    // Zilch-made components
    for (auto& componentHandle : ComponentHandlesContainer) {
      Component::Dereference(componentHandle)->Terminate();
      componentHandle.Delete();
    }
    ComponentHandlesContainer.clear();
  }

  /**************************************************************************/
  /*!
  @brief  Removes the specified component from the entity.
  @param  component The name of the component
  @todo   Check for dependencies before removing the component.
  */
  /**************************************************************************/
  void Entity::RemoveComponentByName(std::string  componentName)
  {
    // Factory-owned components
    for (auto& component : ComponentsContainer) {
      if (component->getObjectName() == componentName) {
        DCTrace << "Entity::RemoveComponentByName - Removing " << componentName << "\n";
        // Check for dependencies

        // Call it's terminate method
        component->Terminate();
        // Remove it
        std::swap(component, ComponentsContainer.back());
        ComponentsContainer.pop_back();
        return;
      }
    }
    // Zilch-owned components
    for (auto& componentHandle : ComponentHandlesContainer) {
      auto component = reinterpret_cast<Component*>(componentHandle.Dereference());
      if (component->getObjectName() == componentName) {
        DCTrace << "Entity::RemoveComponentByName - Removing " << componentName << "\n";
        // Check for dependencies

        // Call it's terminate method
        component->Terminate();
        // Ask Zilch to delete the component 
        componentHandle.Delete();
        // Remove it
        std::swap(componentHandle, ComponentHandlesContainer.back());
        ComponentHandlesContainer.pop_back();
        return;
      }
    }

  }
  
  /**************************************************************************/
  /*!
  @brief  Returns a container of pointers to the components this entity owns.
  @return A container of component pointers.
  @todo   Perhaps just pass the container of handles since this means
  this container is being constructed every frame.
  */
  /**************************************************************************/
  ComponentVec Entity::AllComponents()
  {
    // Fill the container of pointers to the components that this entity owns
    ComponentVec components;
    for (auto& component : ComponentsContainer) {
      components.push_back(component.get());
    }
    for (auto componentHandle : ComponentHandlesContainer) {
      components.push_back(reinterpret_cast<Component*>(componentHandle.Dereference()));
    }
    return components;
  }

  ComponentHandleVec& Entity::AllComponentsByHandle()
  {
    return ComponentHandlesContainer;
  }

  /**************************************************************************/
  /*!
  @brief Moves a component to the back of the entity's components container.
  @param component A reference to the component.
  */
  /**************************************************************************/
  void Entity::SwapToBack(ComponentPtr component)
  {
    auto handle = GetComponentHandle(component);
    auto handleIter = std::find(ComponentHandlesContainer.begin(), ComponentHandlesContainer.end(), handle);
    // Shift every element on the way to the back
    std::rotate(handleIter, handleIter + 1, ComponentHandlesContainer.end());
  }

  /**************************************************************************/
  /*!
  \brief Dispatches an event to the object.
  \param eventID The name of the event.
  \param eventObj The event object.
  */
  /**************************************************************************/
  void Entity::Dispatch(std::string eventID, Event * event)
  {
    DCTrace << Name() << "::Entity::Dispatch: Dispatching '" << eventID << "'! \n";

    // For every delegate in the registry
    for (auto& deleg : ObserverRegistryByString) {
      if (eventID == deleg.first) {
        // For every delegate in the list for this specific event
        for (auto& deleg : deleg.second) {
          deleg->Call(event);
        }
      }
    }
  }

  /**************************************************************************/
  /*!
  \brief  Dispatches an event to the object.
  \param eventObj The event object.
  */
  /**************************************************************************/
  void Entity::Dispatch(Event * eventObj)
  {
    // For every delegate in the registry
    for (auto& event : ObserverRegistryByString) {
      if (eventObj->Name == event.first) {
        // For every delegate in the list for this specific event
        for (auto& deleg : event.second) {
          deleg->Call(eventObj);
        }
      }
    }
  }

  /**************************************************************************/
  /*!
  @brief  Constructs a structure containing current information of all
          active evets.
  @return A structure containing data of all events.
  */
  /**************************************************************************/
  EventDelegatesInfo Entity::PeekEvents()
  {
    auto data = EventDelegatesInfo();
    // Identified by RTTI
    for (auto& event : ObserverRegistryByString) {
      EventDelegatesInfo::EventDelegateInfo info;
      info.Name = event.first;
      for (auto& deleg : event.second) {
        auto componentName = deleg->GetObserver()->Name();
        auto ownerName = Component::IsA(deleg->GetObserver())->Owner()->Name();
        info.Observers.push_back(std::string(ownerName + "::" + componentName));
      }
      data.Events.push_back(info);
    }
    // Identified by string
    for (auto& event : ObserverRegistry) {
      EventDelegatesInfo::EventDelegateInfo info;
      info.Name = event.first.name();
      for (auto& deleg : event.second) {
        auto componentName = deleg->GetObserver()->Name();
        auto ownerName = Component::IsA(deleg->GetObserver())->Owner()->Name();
        info.Observers.push_back(std::string(ownerName + "::" + componentName));
      }
      data.Events.push_back(info);
    }
    return data;
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the parent.
  @return A pointer to the parent of this GameObject.
  */
  /**************************************************************************/
  EntityPtr Entity::Parent()
  {
    return ParentRef;
  }

  /**************************************************************************/
  /*!
  @brief   Grab a reference to a specific child of the GameObject.
  @param   name The name of the child.
  @return  A reference to the child.
  */
  /**************************************************************************/
  EntityPtr Entity::FindChildByName(std::string name)
  {
    for (auto child : ChildrenContainer) {
      if (child->Name() == name)
        return child;
    }
    return nullptr;
  }

  /**************************************************************************/
  /*!
  @brief   Returns a range object containing all of the entity's children.
  @param   The name of the children.
  @return  A range.
  */
  /**************************************************************************/
  EntityVec Entity::FindAllChildrenByName(std::string name)
  {
    EntityVec childrenByName;
    for (auto child : ChildrenContainer) {
      if (child->Name() == name)
        childrenByName.push_back(child);
    }
    return childrenByName;
  }

  /**************************************************************************/
  /*!
  @brief   Returns a range object containing all of the entity's children.
  @return  A range of the entity's children.
  */
  /**************************************************************************/
  EntityVec& Entity::Children()
  {
    return ChildrenContainer;
    
    
  }

  EntityRange Entity::ChildrenByRange()
  {
    return EntityRange(ChildrenContainer.begin(), ChildrenContainer.end());
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the specified component.
  @param name The name of the component/
  @return A pointer to the component, casted down to its base class.
  */
  /**************************************************************************/
  ComponentPtr Entity::getComponent(const std::string & name)
  {
    for (auto& component : ComponentsContainer) {
      if (component->getObjectName() == name) {
        return component.get();
      }
    }
    for (auto& componentHandle : ComponentHandlesContainer) {
      auto component = reinterpret_cast<Component*>(componentHandle.Dereference());
      if (component->getObjectName() == name) {
        return component;
      }
    }
    return nullptr;
  }

  /**************************************************************************/
  /*!
  @brief Returns a pointer to the specified component.
  @param type The boundtype of the component.
  @return A pointer to the component, casted down to its base class.
  */
  /**************************************************************************/
  ComponentPtr Entity::getComponent(Zilch::BoundType * type)
  {
    for (auto& componentHandle : ComponentHandlesContainer) {
      if (Zilch::TypeBinding::IsA(componentHandle.Type, type)) {
        auto component = reinterpret_cast<Component*>(componentHandle.Dereference());
        return component;
      }
    }
    return nullptr;
  }

  /**************************************************************************/
  /*!
  @brief Returns a handle to the component given a pointer to it.
  @param component A reference to the component.
  @return A handle to the component.
  */
  /**************************************************************************/
  ComponentHandle Entity::GetComponentHandle(ComponentPtr component)
  {
    // Find the handle to this component since we internally we have a
    // container of handles to it since Zilch is managing them.
    ComponentHandle componentHandle;
    for (auto& handle : ComponentHandlesContainer) {
      // Once the component has been found
      if (reinterpret_cast<Component*>(handle.Dereference())->Name() == component->Name()) {
        componentHandle = handle;
        return componentHandle;
      }
    }
    return componentHandle;
  }

  /**************************************************************************/
  /*!
  @brief  Informs all active observers of this entity's death. This allows
  them to remove this entity from their list of active delegate holders.
  Otherwise, CRASH!
  */
  /**************************************************************************/
  void Entity::InformObserversOfDeath()
  {
    // For every event in the map of events this entity is receiving...
    for (auto& event : ObserverRegistry) {
      // For every delegate in the list of delegates for each event... 
      for (auto it = event.second.begin(); it != event.second.end(); ++it) {

        // Component: Inform each observer this object is being destroyed
        auto observer = (*it)->GetObserver();
        if (!observer)
          continue;

        if (auto component = dynamic_cast<ComponentPtr>(observer)) {
          // Look in its container of active delegates for the pointer to this
          // entity, then remove it.
          for (auto& publisher : component->ActiveDelegateHolders) {
            if (this == publisher) {
              std::swap(publisher, component->ActiveDelegateHolders.back());
              component->ActiveDelegateHolders.pop_back();
              break;
            }
          }
        }

      }
    }

  }


  void Entity::setArchetype(std::string name)
  {
    ArchetypeName = name;
  }

  std::string Entity::getArchetype() const
  {
    return ArchetypeName;
  }
  

}