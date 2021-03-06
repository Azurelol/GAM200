/*****************************************************************************/
/*!
@file   ReboundBinding.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ReboundBinding.h"

// Components
#include "ReboundComponentsInclude.h"
// Events
#include "ReboundEvents.h"
// Engine
#include "../../Core/Engine/Engine.h" // @todo ew
// Used to add component properties
#include "..\..\Core\Systems\Reflection\ZilchInterfaceUtilities.h"

namespace DCEngine {

  ZilchDefineStaticLibrary(Rebound) {

    ZilchInitializeType(Components::PlayerController);
    ZilchInitializeType(Components::BallController);
    ZilchInitializeType(Components::EnemyController);
    ZilchInitializeType(Components::CameraController);
    ZilchInitializeType(Components::ChargeBar);    
    ZilchInitializeType(Components::Fade);
    ZilchInitializeType(Components::HazardArea);
    ZilchInitializeType(Components::MoveToLocation);
    ZilchInitializeType(Components::MusicManager);
    ZilchInitializeType(Components::PauseManager);
    ZilchInitializeType(Components::LevelManager);
    ZilchInitializeType(Components::MainMenuManager);
    ZilchInitializeType(Components::ErraticDoor);
    ZilchInitializeType(Components::TutorialTextLogic);
    ZilchInitializeType(Components::LockField);
    ZilchInitializeType(Components::Grunt);
    ZilchInitializeType(Components::HealthController);
    ZilchInitializeType(Components::Sentinel);
    ZilchInitializeType(Components::Lancer);
    ZilchInitializeType(Components::Shield);
    ZilchInitializeType(Components::LancerShield);

    // Setup extension properties for Entity
    auto interface = Systems::ZilchInterface::Get();
    auto& boundTypes = builder.BoundTypes.values();
    while (!boundTypes.empty()) {
      interface.SetupTypeProperty(boundTypes.front(), ZilchTypeId(Component), ZilchTypeId(Entity), boundTypes.front(),
        &builder, Systems::GetNativeComponent);
      boundTypes.popFront();
    }
  }

  // Add the Rebound library
  void ReboundComponentsAddToLibrary() {
    Daisy->getSystem<Systems::Reflection>()->Handler()->AddLibrary(Rebound::GetLibrary());
    Daisy->getSystem<Systems::Reflection>()->Handler()->Build();
  }

  // Add the components to the engine's component factory map
  void ReboundComponentsAddToFactory() {
    auto factory = Daisy->getSystem<Systems::Factory>();
    factory->AddComponentFactory(Components::PlayerController ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::PlayerController >>());
    factory->AddComponentFactory(Components::BallController   ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::BallController   >>());
    factory->AddComponentFactory(Components::EnemyController  ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::EnemyController  >>());
    factory->AddComponentFactory(Components::CameraController ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::CameraController >>());
    factory->AddComponentFactory(Components::MusicManager     ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MusicManager     >>());
    factory->AddComponentFactory(Components::PauseManager     ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::PauseManager     >>());
    factory->AddComponentFactory(Components::ChargeBar        ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::ChargeBar        >>());
    factory->AddComponentFactory(Components::HazardArea       ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::HazardArea       >>());
    factory->AddComponentFactory(Components::Fade             ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Fade             >>());
    factory->AddComponentFactory(Components::LevelManager     ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::LevelManager     >>());
    factory->AddComponentFactory(Components::MainMenuManager  ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MainMenuManager  >>());
    factory->AddComponentFactory(Components::MoveToLocation   ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MoveToLocation   >>());
    factory->AddComponentFactory(Components::ErraticDoor      ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::ErraticDoor      >>());
    factory->AddComponentFactory(Components::TutorialTextLogic::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::TutorialTextLogic>>());
    factory->AddComponentFactory(Components::LockField        ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::LockField        >>());
    factory->AddComponentFactory(Components::Grunt            ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Grunt            >>());
    factory->AddComponentFactory(Components::HealthController ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::HealthController >>());
    factory->AddComponentFactory(Components::Sentinel         ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Sentinel         >>());
    factory->AddComponentFactory(Components::Lancer           ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Lancer           >>());
    factory->AddComponentFactory(Components::Shield           ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Shield           >>());
    factory->AddComponentFactory(Components::LancerShield     ::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::LancerShield     >>());
  }



}