/*****************************************************************************/
/*!
@file   LevelManager.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "LevelManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(LevelManager, "LevelManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_ATTRIBUTE(Level);
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, NextLevel)->Attributes.push_back(attributeLevel);
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, LoadingTime);
	  DCE_BINDING_DEFINE_PROPERTY(LevelManager, Timer);
	  DCE_BINDING_DEFINE_PROPERTY(LevelManager, TimerStarted);
  }
  #endif

  void LevelManager::Initialize()
  {
	  auto gameObj = dynamic_cast<GameObject*>(Owner());
	  Connect(gameObj, Events::CollisionStarted, LevelManager::OnCollisionStartedEvent);
	  Connect(SpaceRef, Events::LogicUpdate, LevelManager::OnLogicUpdateEvent);
	  //FadeRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Fade>();
  }

  void LevelManager::OnKeyDownEvent(Events::KeyDown * event)
  {

  }

  void LevelManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

  void LevelManager::OnCollisionStartedEvent(Events::CollisionStarted * event)
  {
	  if (event->OtherObject->getComponent<PlayerController>())
	  {
		  TimerStarted = true;
	      SpaceRef->getComponent<SoundSpace>()->PlayCue("Rumble");
		  //FadeRef->setFading(true);
		 // this->SpaceRef->CreateObject()
	  }
  }

  void LevelManager::OnCollisionEndedEvent(Events::CollisionEnded * event)
  {
  }

  void LevelManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
	  //DCTrace << "LevelManager::OnLogicUpdateEvent \n";
	  if (!TimerStarted)
		  return;

	  Timer += event->Dt;
	  if (Timer > LoadingTime)
	  {
		  DCTrace << "LevelManager::OnLogicUpdateEvent - Loading " << NextLevel << "\n";
		  this->SpaceRef->LoadLevel(NextLevel);
	  }
  }

}