/******************************************************************************/
/*!
@file   Grunt.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  An AI that patrols on a set route until the player enter's it's area.
        Then it will attack until the player leaves.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Grunt.h"
#include "../../../CoreComponents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Grunt, "Grunt", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Grunt);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, startingHealth);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsInvulnerable);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PatrolDistance);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsPatrolRight);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsAggressive);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpPeriod);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpPeriod);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IdleColor);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PatrolColor);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackColor);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsDebugColorActive);
    }

    // Dependancies
    DCE_COMPONENT_DEFINE_DEPENDENCIES(Grunt, "Transform", "RigidBody", "Sprite");
#endif

    Grunt::~Grunt()
    {
      delete stateMachine;
    }


    void Grunt::Initialize()
    {
      DCTrace << "Grunt Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Grunt::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Grunt::OnCollisionStartedEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();

      stateMachine = new StateMachine<Grunt>(this);
      startingPosition = TransformRef->Translation;
      endPosition = startingPosition;

      defaultColor = SpriteRef->Color;

      if (IsPatrolRight)
      {
        endPosition.x = startingPosition.x + PatrolDistance;
        stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        endPosition.x = startingPosition.x - PatrolDistance;
        stateMachine->ChangeState(PatrolLeft::Instance());
      }

      stateMachine->SetGlobalState(Global::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);
    }

    void Grunt::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
      dt = event->Dt;
    }

    void Grunt::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        ModifyHealth(-1);
      }
    }

    bool Grunt::ModifyHealth(int amount)
    {
      int oldHealth = health;

      if (!IsInvulnerable)
      {
        health += amount;

        if (health > maxHealth)
          health = maxHealth;
        if (health < 0)
          health = 0;
      }

      if (health == 0)
      {
        stateMachine->ChangeState(Die::Instance());
      }

      if (oldHealth == health)
        return false;
      else
        return true;
    }


    // Direction should be 1 (right) or -1 (left). 
    void Grunt::Jump(int direction, float period, float strengthX, float strengthY)
    {
      if (jumpTimer > period)
      {
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(strengthX *  direction, strengthY, 0));
        jumpTimer = 0;
      }

      jumpTimer += dt;
    }

#pragma region Global State
    void Grunt::Global::Enter(Grunt *owner){}

    void Grunt::Global::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Grunt::Global::Exit(Grunt *owner){}

    Grunt::Global* Grunt::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {
      if(owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->IdleColor;

      DCTrace << "Grunt Idle Enter\n";
    }

    void Grunt::Idle::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Grunt::Idle::Exit(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->defaultColor;
    }

    Grunt::Idle* Grunt::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region Right State
    void Grunt::PatrolRight::Enter(Grunt *owner)
    {
      owner->jumpTimer = 0;

      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->PatrolColor;

      owner->SpriteRef->FlipX = true;
      DCTrace << "Grunt PatrolRight Enter\n";
    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);
      
      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }        
    }

    void Grunt::PatrolRight::Exit(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->defaultColor;

      owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolRight* Grunt::PatrolRight::Instance()
    {
      static PatrolRight instance;
      return &instance;
    }
#pragma endregion Right State

#pragma region Left State
    void Grunt::PatrolLeft::Enter(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->PatrolColor;

      owner->SpriteRef->FlipX = false;
      DCTrace << "Grunt PatrolLeft Enter\n";
    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(-1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);

      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
    }

    void Grunt::PatrolLeft::Exit(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->defaultColor;

      owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolLeft* Grunt::PatrolLeft::Instance()
    {
      static PatrolLeft instance;
      return &instance;
    }
#pragma endregion Left State

#pragma region Attack State
    void Grunt::Attack::Enter(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->AttackColor;

      DCTrace << "Grunt Attack Enter\n";
    }

    void Grunt::Attack::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      if (direction.x < 0)
      {
        owner->SpriteRef->FlipX = false;
        owner->Jump(-1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);
      }
      else
      {
        owner->SpriteRef->FlipX = true;
        owner->Jump(1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);
      }

      if (!owner->IsAggressive)
      {
        if (owner->IsPatrolRight)
        {
          if (playerPosition.x < owner->startingPosition.x || playerPosition.x > owner->endPosition.x)
            owner->stateMachine->RevertToPreviousState();
        }
        else
        {
          if (playerPosition.x > owner->startingPosition.x || playerPosition.x < owner->endPosition.x)
            owner->stateMachine->RevertToPreviousState();
        }
      }
    }

    void Grunt::Attack::Exit(Grunt *owner)
    {
      if (owner->IsDebugColorActive)
        owner->SpriteRef->Color = owner->defaultColor;
    }

    Grunt::Attack* Grunt::Attack::Instance()
    {
      static Attack instance;
      return &instance;
    }
#pragma endregion Attack State

#pragma region Die State
    void Grunt::Die::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Grunt::Die::Update(Grunt *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Grunt::Die::Exit(Grunt *owner)
    {
    //create death particle
    auto particle = owner->SpaceRef->CreateObject("EnemyExplosionParticle");
    if (particle)
    {
      particle->getComponent<Components::Transform>()->setTranslation(owner->TransformRef->Translation); 
    }
      // Destroy grunt

      owner->gameObj->Destroy();
    }

    Grunt::Die* Grunt::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}