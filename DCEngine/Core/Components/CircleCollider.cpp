/******************************************************************************/
/*!
@file   CircleCollider.cpp
@author Blaine Reiner@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The CircleCollider component gives a physical representation of a Circle
        in world space so objects can experience collisions with one
        another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"
#include "CircleCollider.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief CircleCollider destructor.
    @todo  This could be done in the base collider class instead.
    */
    /**************************************************************************/
    CircleCollider::~CircleCollider()
    {
      SpaceRef->getComponent<Components::PhysicsSpace>()->RemoveCollider(this);
    }

    /**************************************************************************/
    /*!
    @brief Initializes the CircleCollider component.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    void CircleCollider::Initialize()
    {
      // Store a reference to the Transform Component
      TransformComponent = Owner()->getComponent<Components::Transform>();
      // Subscribe this physics component to the physics space
      SpaceRef->getComponent<Components::PhysicsSpace>()->AddCollider(this);

      // If this component needs to draw the collider with debug draw,
      // subscribe to LogicUpdate events so it can draw
      if (IsDrawingCollider)
        Connect(SpaceRef, Events::LogicUpdate, CircleCollider::OnLogicUpdateEvent);

    }



    /*void CircleCollider::DrawCollider()
    {
      auto debugScale = Vec2(TransformComponent->Scale.x * Size.x,
                              TransformComponent->Scale.y * Size.y);

      SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(TransformComponent->Translation + Offset,
                                                            debugScale.x, //TransformComponent->Scale.x * Size.x,
                                                            debugScale.y, //TransformComponent->Scale.y * Size.y,
                                                            Vec4(1, 0, 0, 1)); // Red
    }*/


    /**************************************************************************/
    /*!
    \brief  CollisionEvents
    */
    /**************************************************************************/
    void CircleCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      // Move the GameObject??
    }

    void CircleCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {

    }

    void CircleCollider::OnLogicUpdateEvent(Events::LogicUpdate* event)
    {
      //DrawCollider();
    }

    /* Getters */

    Vec3 CircleCollider::getOffset(void)
    {
      return this->Offset;
    }

    bool CircleCollider::getGhost(void)
    {
      return this->Ghost;
    }

    bool CircleCollider::getSendsEvents(void)
    {
      return this->SendsEvents;
    }

    float CircleCollider::getRadius(void)
    {
      return Radius;
    }

  }
}