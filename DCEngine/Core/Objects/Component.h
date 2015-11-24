/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

// Base class
#include "Object.h"
// Libraries
//#include <JSONCPP\json.h>
// Headers
#include "../Engine/Event.h" //!< Components need to access events.
#include "../EventsInclude.h" //!< A list of events that can be added.
#include "../Systems/Serialization/Serialization.h"

namespace DCEngine {

  // (?) EXPLAIN
  using mask = unsigned;

#pragma region metadataEnums
  
  enum class EntityType;

  enum class EnumeratedComponent {
    None = 0,
    Transform = 1,
    Sprite = 2,
    Drawable = 3,

    TimeSpace = 10,
    GraphicsSpace = 11,
    PhysicsSpace = 12,
    SoundSpace = 13,


    Capacity,    

  };

  enum class BitfieldComponent {
    Alive = 1,
    Transform = 1 << 1,
    Sprite = 1 << 2,
    Drawable = 1 << 3,

    TimeSpace = 1 << 10,
    GraphicsSpace = 1 << 11,
    PhysicsSpace = 1 << 12,
    SoundSpace = 1 << 13,

    NoObjects = -1,

  };
  
  /* Forward-declarations */
  class Entity;
  class GameObject;
  class Space;
  class GameSession;
  namespace Systems {
    class Factory;
  }
  

  class Component : public Object {
	  friend class Entity;
    friend class GameObject; 
    friend class Systems::Factory;
    friend class Engine; // @todo Is this the best way?

  public:   

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Component, Object);
    #endif

    Component(std::string name, Entity& owner);
    // Derived component types need to be deallocated properly
    virtual ~Component();
    virtual void Initialize() = 0; // Every component needs to be initialized.
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);

    //virtual void Destroy() = 0; // Every component needs to provide a method for its destruction.   
    template <typename EntityClass> EntityClass* getOwner();
    Entity* Owner(); // Returns a pointer to the component's owner
    const Space& ThisSpace() const { return  *SpaceRef; }
    const GameSession& ThisGameSession()  const { return *GameSessionRef; }     

    // Static member variables
    static unsigned int ComponentsCreated;
    static unsigned int ComponentsDestroyed;
    const unsigned int ComponentID;
    static std::string ComponentLastCreated;
    static std::string ComponentLastDestroyed;
    static bool DiagnosticsEnabled;
    
  protected:

    Space* SpaceRef;
    GameSession* GameSessionRef;
    
  private:

    EntityType OwnerClass;
    Component() = delete; // No default construction
    void SetReferences();
    static std::vector<Zilch::BoundType*> AllComponents();
    //std::vector<Delegate*> ActiveDelegates;

  };

  using ComponentPtr = Component*;
  using ComponentVec = std::vector<ComponentPtr>;
  using ComponentStrongPtr = std::unique_ptr<Component>;
  using ComponentStrongVec = std::vector<ComponentStrongPtr>;

  template<typename EntityClass>
  inline EntityClass* Component::getOwner() {
    if (ownerType_ == EntityType::GameObject)
      return dynamic_cast<GameObject*>(ObjectOwner);
    else if (ownerType_ == EntityType::Space)
      return dynamic_cast<Space*>(ObjectOwner);
    else if (ownerType_ == EntityType::GameSession) 
      return dynamic_cast<GameSession*>(ObjectOwner);
   return NULL;
  }

} // DCEngine
#endif