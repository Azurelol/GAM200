/******************************************************************************/
/*!
@file   CameraController.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for 
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

#define CameraControllerTraceOn 0

namespace DCEngine {
  namespace Components {
    class Transform;
    class Sprite;
    class CameraController : public Component {

    public:
      Transform* TransformRef;
      Sprite* SpriteRef;
      GameObject* PlayerRef;
      Real InterpolationSpeed = 0.06f; //percent of the way to interpolate per frame from 0-1
      String TargetName = "Player";
      Boolean DoScreenShake = false;

      DCE_DEFINE_PROPERTY(String, TargetName);
      DCE_DEFINE_PROPERTY(Real, InterpolationSpeed);


      CameraController(Entity& owner) : Component(std::string("CameraController"), owner) {}
      void Initialize();
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void CameraController::OnLogicUpdateEvent(Events::LogicUpdate * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(CameraController, Component);
#endif

    private:
      void PrintTranslation();
    };
  }

}