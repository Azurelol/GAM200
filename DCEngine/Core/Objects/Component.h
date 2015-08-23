/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The

*/
/******************************************************************************/
#include "Object.h"
#pragma once

#include <memory>
#include <vector>

//#include "..\Engine\Engine.h"

//class Entity; // Forward declaration: EntityPtr

namespace DCEngine {
  
  // Give every component access to the engine object.
  //extern std::unique_ptr<Engine> Daisy;

  // (?) EXPLAIN
  using mask = unsigned;

#pragma region metadataEnums
  
  enum class EnumeratedComponent {
    None = 0,
    Transform = 1,
    Sprite = 2,
    Drawable = 3,

    Capacity,    

  };

  enum class BitfieldComponent {
    Alive = 1,
    Transform = 1 << 1,
    Sprite = 1 << 2,
    Drawable = 1 << 3,

    NoObjects = -1,

  };
  
  class Component : public Object {

  public:    
    Component(EnumeratedComponent type, BitfieldComponent mask);
    virtual ~Component() {} // Derived component types need to be deallocated properly

    virtual void Initialize() = 0; // Every component needs to be initialized.
    virtual void Update() = 0; // Components are updated through events.

    void Connect(); // The component has to register to an event to be updated.
    void Disconnect(); // The component can unsubscribe from listening to an event.

    // Once the component is created, it should never change types
    const EnumeratedComponent _type;
    const BitfieldComponent _mask;

  private:
    Component() = delete; // No default construction
    //EntityPtr _owner; // The owner of this component.

  };

  using ComponentPtr = std::shared_ptr<Component>;
  using ComponentVec = std::vector<ComponentPtr>;
  //using ComponentPtr = std::shared_ptr<Component>;

}