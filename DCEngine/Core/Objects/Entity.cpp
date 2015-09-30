/******************************************************************************/
/*!
\file   Entity.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

*/
/******************************************************************************/
#include "Entity.h"

// Headers
#include "..\ComponentsInclude.h" // Entities need to know of componnets
#include "Entities\Space.h"
#include "Entities\GameSession.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  \brief  Constructor.
  */
  /**************************************************************************/
  Entity::Entity(std::string name) : Object("Entity") {
    _name = name;
  }

  bool Entity::CheckMask(mask m) {
    return ((_mask & m) == m);
  }

  /**************************************************************************/
  /*!
  \brief  Adds a component to the entity by pointer.
  \param  A component pointer to the component being added to the entity.
  */
  /**************************************************************************/
  bool Entity::AddComponent(std::shared_ptr<Component> component) {
    // If there is already a component of the same class, reject the operation
    for (auto componentOwned : components_) {
      if (std::type_index(typeid(*componentOwned.get())) == (std::type_index(typeid(*component.get())))) {
        trace << _name << "::AddComponent - Failure! " << component->Name() << " is already present!\n";
        return false;
      }        
    }

    if (TRACE_COMPONENT_ADD)
      trace << _name << "::AddComponent - Added " << component->Name() << "\n";

    // Adds the component to the entity
    components_.push_back(component);
    return true;    
  }

  /**************************************************************************/
  /*!
  \brief  Initialize all of the entity's components.
  */
  /**************************************************************************/
  void Entity::Initialize() {
    trace << _name << "::Initialize \n";
    for (auto component : components_)
      component->Initialize();
  }

  /**************************************************************************/
  /*!
  \brief  Returns a pointer to the given component.
  \param  The name, in a string, of the component.
  \return A pointer to the component.
  */
  /**************************************************************************/
  Component* Entity::getComponentByName(std::string name) {

    for (auto component : components_) {
      if (component->Name() == name)
        return component.get();
    }

    return NULL;

  }


}