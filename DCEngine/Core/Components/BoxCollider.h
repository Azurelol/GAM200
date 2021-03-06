/******************************************************************************/
/*!
@file   BoxCollider.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "Collider.h"

namespace DCEngine {
  namespace Components
  {
    class Transform;
    class BoxCollider : public Collider {
    public:

      ZilchDeclareDerivedType(BoxCollider, Collider);
      // Variables
      Vec3 Size = Vec3(1, 1, 1);
      Vec3 Offset = Vec3(0, 0, 0);
      Vec4 Color;
      Boolean Ghost = false;
      Boolean SendsEvents = true;
      Boolean IsDrawingCollider = false;

      // Properties
      DCE_DEFINE_PROPERTY(Vec3, Size);
      DCE_DEFINE_PROPERTY(Vec3, Offset);
      DCE_DEFINE_PROPERTY(Boolean, Ghost);
      DCE_DEFINE_PROPERTY(Boolean, SendsEvents);
      DCE_DEFINE_PROPERTY(Boolean, IsDrawingCollider);


      // These should be private!
      BoxCollider(Entity& owner);
      ~BoxCollider();
      void Initialize();
      Vec3 getColliderScale();

    private:

      Transform* TransformComponent = NULL;
      // The group is a tag used for filter out collisions. The rules

      //CollisionGroupHandle CollisionGroup = String("Default");

      void Register();

      /* Events */
      void OnLogicUpdateEvent(Events::LogicUpdate* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      /* Methods */
      void DrawCollider();

      /* Variables */


    };
  }

}
