/******************************************************************************/
/*!
@file   Collider.cpp
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Collider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {
  namespace Components {

    unsigned Collider::Created = 0;
    unsigned Collider::Destroyed = 0;
    unsigned Collider::Active = 0;

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Collider, "Collider", DCEngineCore, builder, type) {
      //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
      //DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Collider);
      DCE_BINDING_DEFINE_ATTRIBUTE(Hidden);
      // Constructor / Destructor
      ZilchBindConstructor(builder, type, Collider, "owner, colliderClass", Entity&, std::string);
      ZilchBindDestructor(builder, type, Collider);
      // Properties




      // Properties

    }
    #endif

    /**************************************************************************/
    /*!
    @brief Collider constructor.
    @param owner A reference to the Entity that owns this component.
    @param graphicalComponent The name of the derived collider component.
    */
    /**************************************************************************/
    Collider::Collider(Entity & owner, std::string colliderClass) : Component(colliderClass, owner)
    {
      Created++;
      Active++;
    }

    /**************************************************************************/
    /*!
    @brief Collider destructor.
    */
    /**************************************************************************/
    Collider::~Collider()
    {
      Destroyed++;
      Active--;
    }

    /**************************************************************************/
    /*!
    @brief Collider initializer.
    */
    /**************************************************************************/
    void Collider::Initialize()
    {
    }

    float Collider::getRestitution(void)
    {
      auto PhyMat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(PhysicsMaterial);

      return PhyMat->getRestitution();
    }

    float Collider::getFriction(void)
    {
      auto PhyMat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(PhysicsMaterial);

      return PhyMat->getFriction();
    }

    float Collider::GetInvMass(void)
    {
      return 1.0f / GetMass();
    }

    float Collider::GetMass(void)
    {
      auto PhyMat = Daisy->getSystem<Systems::Content>()->getPhysicsMaterial(PhysicsMaterial);

      Vec3 volume = dynamic_cast<GameObject*>(this->Owner())->getComponent<Components::Transform>()->Scale;

      return (PhyMat->getDensity() * volume.x * volume.y);
    }

    const PhysicsMaterialHandle & Collider::getPhysicsMaterial()
    {
      return PhysicsMaterial;
    }
    void Collider::setPhysicsMaterial(PhysicsMaterialHandle const & material)
    {
      PhysicsMaterial = material;
    }


  }
}