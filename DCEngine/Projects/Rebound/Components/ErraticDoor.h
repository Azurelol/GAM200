/*****************************************************************************/
/*!
@file   ErraticDoor.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
  namespace Components
  {
    class Transform;
    class RigidBody;
    class Sprite;
    class MoveToLocation;
    class ErraticDoor : public Component {

    public:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      MoveToLocation* MTLRef;

      // Properties
      DCE_DEFINE_PROPERTY(Real, Timer);
      DCE_DEFINE_PROPERTY(Real, RandomDelay);

      // Methods
      ErraticDoor(Entity& owner) : Component(std::string("ErraticDoor"), owner) {}
      void Initialize();
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(ErraticDoor, Component);
#endif

    private:
      // Member variables
      Real Timer = 0;
      Real RandomDelay;
    };
  }

}