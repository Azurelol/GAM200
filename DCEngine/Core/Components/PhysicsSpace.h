/*****************************************************************************/
/*!
\file   PhysicSpaces.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics space hold the settings for all physical interactions within
        a space. It also holds several containers of objects that require
        physics integration and resolution, for collisions, velocity, etc...
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Objects/Entities/GameObject.h"

namespace DCEngine {

  class GameObject;

  class Physics;
  class PhysicsSpace : public Component {
    friend class Physics;
  public:

    bool AllowSleep;
    bool Mode2D;
    bool Deterministic;
    
    PhysicsSpace(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    void AddRigidBody(GameObject* rigidbody);
    void AddCollider(GameObject* collider);
    GameObjectRawVec getRigidbodies();
    GameObjectRawVec getColliders();


  private:

    GameObjectRawVec rigidbodies_;
    GameObjectRawVec colliders_;

  };


}