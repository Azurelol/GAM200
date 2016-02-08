/******************************************************************************/
/*!
@file   EditorCameraController.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The EditorCameraController...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "EditorCameraController.h"

#include "EngineReference.h"
#include "Camera.h"
#include "Transform.h"

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(EditorCameraController, "EditorCameraController", DCEngineCore, builder, type) {
      // Constructor / Destructor
      //ZilchBindConstructor(builder, type, EditorCameraController, "owner", Entity&);
      //ZilchBindDestructor(builder, type, EditorCameraController);
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(EditorCameraController);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(EditorCameraController, MoveSpeed);
      DCE_BINDING_DEFINE_PROPERTY(EditorCameraController, RotSpeed);
      DCE_BINDING_DEFINE_PROPERTY(EditorCameraController, ZoomSpeed);
    }
    #endif


    /**************************************************************************/
    /*!
    @brief Initializes the EditorCameraController.
    */
    /**************************************************************************/
    void EditorCameraController::Initialize()
    {
      CameraComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Camera>();
      TransformComponent = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>();
      Connect(Daisy->getKeyboard(), Events::KeyDown, EditorCameraController::OnKeyDownEvent);
      Connect(Daisy->getKeyboard(), Events::KeyUp, EditorCameraController::OnKeyUpEvent);
      Connect(Daisy->getMouse(), Events::MouseScroll, EditorCameraController::OnMouseScrollEvent);
    }

    void EditorCameraController::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

        /* Zoom */
      case Keys::Add:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
        {
          CameraComponent->FieldOfView -= 10;
          DCTrace << Owner()->Name() << " - FieldOfView: " << CameraComponent->FieldOfView << "\n";
        }
        else
        {
          CameraComponent->Size += 1;
          DCTrace << Owner()->Name() << " - Size: " << CameraComponent->FieldOfView << "\n";
        }
        break;
      case Keys::Subtract:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
        {
          CameraComponent->FieldOfView += 10;
          DCTrace << Owner()->Name() << " - FieldOfView: " << CameraComponent->FieldOfView << "\n";
        }
        else
        {
          CameraComponent->Size -= 1;
          DCTrace << Owner()->Name() << " - Size: " << CameraComponent->FieldOfView << "\n";
        }
        break;

        /* Movement */
      case Keys::W:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
          TransformComponent->Translation.y += MoveSpeed;
        else
          TransformComponent->Translation.y += MoveSpeed;
        break;
      case Keys::S:
        if (CameraComponent->Projection == ProjectionMode::Perspective)
          TransformComponent->Translation.y -= MoveSpeed;
        else
          TransformComponent->Translation.y -= MoveSpeed;
        break;
      case Keys::A:
        TransformComponent->Translation.x -= MoveSpeed;
        break;
      case Keys::D:
        TransformComponent->Translation.x += MoveSpeed;
        break;
      case Keys::F1:
        TransformComponent->Rotation.z -= RotSpeed;
        break;
      case Keys::F2:
        TransformComponent->Rotation.z += RotSpeed;

      }


    }

    void EditorCameraController::OnKeyUpEvent(Events::KeyUp * event)
    {
    }

    void EditorCameraController::OnMouseScrollEvent(Events::MouseScroll * event)
    {
      // Scroll up
      if (event->Delta > 0)
        TransformComponent->Translation.z -= MoveSpeed / 2;
      // Scroll down
      else if (event->Delta < 0)
        TransformComponent->Translation.z += MoveSpeed / 2;
    }


  }
}