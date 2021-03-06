/******************************************************************************/
/*!
\file   Resolution.cpp
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Resolves the collisions betwen objects
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Physics.h"

#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Components/CircleCollider.h"
#include "../../Objects/Entities/EntitiesInclude.h"
#include "../../Events/CollisionEvents.h"
#include "Collision.h"
#include "Resolution.h"

namespace DCEngine
{
  /**************************************************************************/
  /*!
  @brief send the data to other resolution functions.
  @param dt The length of the last frame.
  @param contactlist an array of the manifolds.
  */
  /**************************************************************************/
  void Resolution::Resolve(float dt, std::vector<Manifold> &contactlist)
  {

    ResolvePositions(dt, contactlist);
    ResolveVelocities(dt, contactlist);

  }

  /**************************************************************************/
  /*!
  @brief resolve the velocity of this contact.
  @param dt The length of the last frame.
  @param c a specific manifold.
  @return void.
  */
  /**************************************************************************/
  void Resolution::ResolveContactVelocity(float dt, Manifold &c)
  {
    auto rigid1 = c.Object1->getComponent<Components::RigidBody>();
    auto rigid2 = c.Object2->getComponent<Components::RigidBody>();

    Components::Collider * Collider1 = c.Object1->getComponent<Components::BoxCollider>();
    Components::Collider * Collider2 = c.Object1->getComponent<Components::BoxCollider>();

    if (Collider1 == NULL)
    {
      Collider1 = c.Object1->getComponent<Components::CircleCollider>();
    }

    if (Collider2 == NULL)
    {
      Collider2 = c.Object2->getComponent<Components::CircleCollider>();
    }


    //Find the velocity of the two object along the contact normal
    float separatingVelocity = c.CalculateSeparatingVelocity();

    if (separatingVelocity >= 0)
    {
      //The objects are no longer moving towards each other
      //or the contact they are stationary
      c.ContactImpulse = 0;
      return;
    }

    //This velocity of this collision has to be resolved.

    //To model inelastic collision the kinetic energy of the bodies 
    //needs to be reduced.
    //This technique uses the combined restitution to determine what percentage
    //of the energy along the collision normal is conserved
    float newSepVelocity = -separatingVelocity;

    const bool AccelerationBuildUp = true;
    //When an object is resting on the ground it is constantly falling
    //due to gravity. This acceleration need be removed or objects will
    //jitter on the ground.

    if (AccelerationBuildUp)
    {
      // Check the velocity build-up due to acceleration only
      glm::vec3 accCausedVelocity;

      if (c.rigid1 == false && c.rigid2 == false)
      {
        accCausedVelocity = Vec3(0, 0, 0);
      }

      if (c.rigid1 != false && c.rigid2 == false)
      {
        accCausedVelocity = rigid1->getAcceleration();
      }

      if (c.rigid1 == false && c.rigid2 != false)
      {
        accCausedVelocity = -rigid2->getAcceleration();
      }

      if (c.rigid1 != false && c.rigid2 != false)
      {
        accCausedVelocity = rigid1->getAcceleration() - rigid2->getAcceleration();
      }


      float accCausedSepVelocity = glm::dot(accCausedVelocity, c.ContactNormal);

      // If we've got a closing velocity due to acceleration build-up,
      // remove it from the new separating velocity
      if (accCausedSepVelocity < 0)
      {
        newSepVelocity += accCausedSepVelocity;

        // Make sure we haven't removed more than was
        // there to remove.
        if (newSepVelocity < 0) newSepVelocity = 0;
      }
    }

    //What is the total change in velocity for the contact?
    float deltaVelocity = /*newSepVelocity*/ 0.0f - separatingVelocity;

    //The delta velocity is applied to each object proportional to inverse
    //mass. So the more massive an object is the less of the change
    //in velocity it will receive.
    float totalInverseMass;

    if (c.rigid1 == false && c.rigid2 == false)
    {
      totalInverseMass = 0;
    }

    if (c.rigid1 != false && c.rigid2 == false)
    {
      totalInverseMass = Collider1->GetInvMass();
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      totalInverseMass = Collider2->GetInvMass();
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      totalInverseMass = Collider1->GetInvMass() + Collider2->GetInvMass();
    }


    // Calculate the impulse to apply
    float impulse = ((1 + c.Restitution) * deltaVelocity) / totalInverseMass;

    c.ContactImpulse = impulse;
		if(impulse > 600)
			DCTrace << impulse << '\n';

    // Find the amount of impulse per unit of inverse mass
    glm::vec3 impulsePerIMass = c.ContactNormal * impulse;

    // Apply impulses: they are applied in the direction of the contact,
    // and in proportion to inverse mass.

    if (c.rigid1 == false && c.rigid2 == false)
    {
      return;
    }

    if (c.rigid1 != false && c.rigid2 == false)
    {
      if (rigid1->DynamicState != DynamicStateType::Static)
      {
        // The other body goes in the opposite direction
        rigid1->setVelocity(rigid1->getVelocity() + impulsePerIMass * Collider1->GetInvMass());
      }
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      if (rigid2->DynamicState != DynamicStateType::Static)
      {
        rigid2->setVelocity(rigid2->getVelocity() - impulsePerIMass * Collider2->GetInvMass());
      }
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      if (rigid1->DynamicState != DynamicStateType::Static)
      {
        rigid1->setVelocity(rigid1->getVelocity() + impulsePerIMass * Collider1->GetInvMass());
      }

      if (rigid2->DynamicState != DynamicStateType::Static)
      {
        // The other body goes in the opposite direction
        rigid2->setVelocity(rigid2->getVelocity() - impulsePerIMass * Collider2->GetInvMass());
      }
    }


    // this is the code that handles friction
     
    if (c.rigid1 != false && c.rigid2 == false)
    {
      if (c.Object1->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
          Vec3 friction(-c.ContactNormal.y, c.ContactNormal.x, 0);

          friction = glm::normalize(friction);

          Real dot = glm::dot(friction, c.Object1->getComponent<Components::RigidBody>()->getVelocity());

          friction *= dot / 12.0f;

          friction *= -c.FrictionCof;

          if (friction.length() < 1.0f)
          {
            friction = glm::normalize(friction);
            friction *= 2.0f;
          }

          if (friction.length() > c.Object1->getComponent<Components::RigidBody>()->getVelocity().length())
          {
            friction = glm::normalize(friction);
            friction *= c.Object1->getComponent<Components::RigidBody>()->getVelocity().length();
          }

          c.Object1->getComponent<Components::RigidBody>()->setVelocity(c.Object1->getComponent<Components::RigidBody>()->getVelocity() + friction);
      }
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      if (c.Object2->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        // The other body goes in the opposite direction
        Vec3 friction(-c.ContactNormal.y, c.ContactNormal.x, 0);

        friction = glm::normalize(friction);

        Real dot = glm::dot(friction, c.Object2->getComponent<Components::RigidBody>()->getVelocity());

        friction *= dot / 12.0f;

        friction *= -c.FrictionCof;

        if (friction.length() < 1.0f)
        {
          friction = glm::normalize(friction);
          friction *= 2.0f;
        }

        if (friction.length() > c.Object2->getComponent<Components::RigidBody>()->getVelocity().length())
        {
          friction = glm::normalize(friction);
          friction *= c.Object2->getComponent<Components::RigidBody>()->getVelocity().length();
        }

        c.Object2->getComponent<Components::RigidBody>()->setVelocity(c.Object2->getComponent<Components::RigidBody>()->getVelocity() + friction);
      }
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      if (c.Object1->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        Vec3 friction(-c.ContactNormal.y, c.ContactNormal.x, 0);

        friction = glm::normalize(friction);

        Real dot = glm::dot(friction, c.Object1->getComponent<Components::RigidBody>()->getVelocity());

        friction *= dot / 12.0f;

        friction *= -c.FrictionCof;

        if (friction.length() < 1.0f)
        {
          friction = glm::normalize(friction);
          friction *= 2.0f;
        }

        if (friction.length() > c.Object1->getComponent<Components::RigidBody>()->getVelocity().length())
        {
          friction = glm::normalize(friction);
          friction *= c.Object1->getComponent<Components::RigidBody>()->getVelocity().length();
        }

        c.Object1->getComponent<Components::RigidBody>()->setVelocity(c.Object1->getComponent<Components::RigidBody>()->getVelocity() + friction);
      }

      if (c.Object2->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        // The other body goes in the opposite direction
        Vec3 friction(-c.ContactNormal.y, c.ContactNormal.x, 0);

        friction = glm::normalize(friction);

        Real dot = glm::dot(friction, c.Object2->getComponent<Components::RigidBody>()->getVelocity());

        friction *= dot / 12.0f;

        friction *= -c.FrictionCof;

        if (friction.length() < 1.0f)
        {
          friction = glm::normalize(friction);
          friction *= 2.0f;
        }

        if (friction.length() > c.Object2->getComponent<Components::RigidBody>()->getVelocity().length())
        {
          friction = glm::normalize(friction);
          friction *= c.Object2->getComponent<Components::RigidBody>()->getVelocity().length();
        }

        c.Object2->getComponent<Components::RigidBody>()->setVelocity(c.Object2->getComponent<Components::RigidBody>()->getVelocity() + friction);
      }
    }

  }

  void Resolution::ResolveVelocities(float dt, std::vector<Manifold> &contactlist)
  {
   // unsigned int iterationsRun = 0;
   // unsigned int maxIterations = static_cast<unsigned int>(contactlist.size());
   // while (iterationsRun < maxIterations)
   // {
      // Find the contact with the largest closing velocity;
    //  float maxVelocity = FLT_MAX;
    //  unsigned int contactIndex = static_cast<unsigned int>(contactlist.size());
      for (unsigned i = 0; i < contactlist.size(); ++i)
      {
        if (!contactlist[i].rigid1 && !contactlist[i].rigid2)
        {
          continue;
        }

        ResolveContactVelocity(dt, contactlist[i]);
       // float sepVel = contactlist[i].CalculateSeparatingVelocity();
       // if (sepVel < 0 && sepVel < maxVelocity)
       // {
       //   maxVelocity = sepVel;
       //   contactIndex = i;
       // }
      }

      // Do we have anything worth resolving?
      /*if (contactIndex == contactlist.size())
      {
        break;
      }*/

      // Resolve this contact velocity
      //ResolveContactVelocity(dt, contactlist[contactIndex]);

      //++iterationsRun;
    //}
  }

  void Resolution::ResolvePenetration(float dt, Manifold& c)
  {
    // The movement of each object is based on their inverse mass, so
    // total that.
    float totalInverseMass;
    Components::Collider * Collider1 = c.Object1->getComponent<Components::BoxCollider>();
    Components::Collider * Collider2 = c.Object1->getComponent<Components::BoxCollider>();

    if (Collider1 == NULL)
    {
      Collider1 = c.Object1->getComponent<Components::CircleCollider>();
    }

    if (Collider2 == NULL)
    {
      Collider2 = c.Object2->getComponent<Components::CircleCollider>();
    }



    if (c.rigid1 == false && c.rigid2 == false)
    {
      totalInverseMass = 0.000000000000000001f;
    }

    if (c.rigid1 != false && c.rigid2 == false)
    {
      totalInverseMass = Collider1->GetInvMass();
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      totalInverseMass = Collider2->GetInvMass();
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      totalInverseMass = Collider1->GetInvMass() + Collider2->GetInvMass();
    }

    // Find the amount of penetration resolution per unit of inverse mass
    glm::vec3 movePerIMass = c.ContactNormal * (c.Penetration / totalInverseMass);

    //If stack stability can be increased by not resolving all the penetrations
    //in one step
    movePerIMass *= 1.0f;

    // Calculate the the movement amounts

    if (c.rigid1 == false && c.rigid2 == false)
    {
      c.v1 = movePerIMass *  0.0f;
      c.v2 = movePerIMass * -0.0f;
    }

    if (c.rigid1 != false && c.rigid2 == false)
    {
      c.v1 = movePerIMass *  Collider1->GetInvMass();
      c.v2 = movePerIMass * -0.0f;
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      c.v1 = movePerIMass *  0.0f;
      c.v2 = movePerIMass * -Collider2->GetInvMass();
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      c.v1 = movePerIMass *  Collider1->GetInvMass();
      c.v2 = movePerIMass * -Collider2->GetInvMass();
    }

    // Apply the penetration resolution

    if (c.rigid1 == false && c.rigid2 == false)
    {
      return;
    }

    if (c.rigid1 != false && c.rigid2 == false)
    {
      if (c.Object1->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object1->getComponent<Components::RigidBody>()->setPosition(c.Object1->getComponent<Components::RigidBody>()->getPosition() + c.v1);
      }
    }

    if (c.rigid1 == false && c.rigid2 != false)
    {
      if (c.Object2->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object2->getComponent<Components::RigidBody>()->setPosition(c.Object2->getComponent<Components::RigidBody>()->getPosition() + c.v2);
      }
    }

    if (c.rigid1 != false && c.rigid2 != false)
    {
      if (c.Object1->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object1->getComponent<Components::RigidBody>()->setPosition(c.Object1->getComponent<Components::RigidBody>()->getPosition() + c.v1);
      }
      else if (c.Object2->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object2->getComponent<Components::RigidBody>()->setPosition(c.Object2->getComponent<Components::RigidBody>()->getPosition() + c.v2);
      }

      if (c.Object2->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object2->getComponent<Components::RigidBody>()->setPosition(c.Object2->getComponent<Components::RigidBody>()->getPosition() + c.v2);
      }
      else if (c.Object1->getComponent<Components::RigidBody>()->DynamicState != DynamicStateType::Static)
      {
        c.Object1->getComponent<Components::RigidBody>()->setPosition(c.Object1->getComponent<Components::RigidBody>()->getPosition() + c.v1);
      }
    }

    c.Penetration = 0.0f;
  }

  void Resolution::ResolvePositions(float dt, std::vector<Manifold> &contactlist)
  {
    //Resolving penetrations is a tricky problem with large stacks. The only real way to get 
    //good results is by considering all contacts at once at solving them together. This method basically 
    //just keeps poking the bodies until the penetrations are below the penetration slop epsilon. 
    //Better solutions involve building contact graphs, shock propagation and constraint based
    //solvers.
    /*unsigned int iterationsRun = 0;
    unsigned int maxIterations = static_cast<unsigned int>(contactlist.size());
    const float positionEpsilon = 0.000000001f;

    while (iterationsRun < maxIterations)
    {
      // Find biggest penetration greater than
      //the correction epsilon
      float maxPenetration = positionEpsilon;
      unsigned contactIndex = static_cast<unsigned int>(contactlist.size());
      for (unsigned j = 0; j < contactlist.size(); ++j)
      {
        if (contactlist[j].Penetration > maxPenetration)
        {
          maxPenetration = contactlist[j].Penetration;
          contactIndex = j;
        }
      }

      if (contactIndex >= contactlist.size())
      {
        break;
      }

      //Resolve the penetration
      ResolvePenetration(dt, contactlist[contactIndex]);

      ++iterationsRun;
    }*/

    for (auto contact : contactlist)
    {
      ResolvePenetration(dt, contact);
    }
  }

}
