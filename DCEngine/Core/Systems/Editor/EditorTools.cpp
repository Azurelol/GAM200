/******************************************************************************/
/*!
@file   EditorTools.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Activates the selected tool. Called on update.
    */
    /**************************************************************************/
    void Editor::UseTool()
    {
      switch (ActiveTool) {

      case EditorTool::Select:
        //ShowSelection();
        break;
      case EditorTool::Translate:
        TranslateTool();
        break;
      case EditorTool::Rotate:
        RotateTool();
        break;
      case EditorTool::Scale:
        ScaleTool();
        break;
      default:
        break;

      }
    }

    /**************************************************************************/
    /*!
    @brief  The select tool allows the user to select an object on screen.
    */
    /**************************************************************************/
    void Editor::ShowSelection()
    {      
      if (!SelectedObject)
        return;

      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        // Get the object's position
        auto transform = gameObject->getComponent<Transform>();

        // Draw a selected 'box' around the object
        Vec3 pos = transform->getTranslation();
        Real width = transform->getScale().x * 2;
        Real height = transform->getScale().y * 2;
        Vec4 color(1.0, 0, 0, 1.0);

        CurrentSpace->getComponent<GraphicsSpace>()->DrawRectangle(pos,
          width, height, color);
      }


    }

    /**************************************************************************/
    /*!
    @brief  The translate tool allows the user to move an object on screen.
    */
    /**************************************************************************/
    void Editor::TranslateTool()
    {
      if (!SelectedObject)
        return;

      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {

        // Get the object's transform data
        auto transform = gameObject->getComponent<Transform>();
        Vec3 pos = transform->getTranslation();
        Real radius = 8;
        Real arrowTip = 1;
        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
                                         // X-axis
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(radius, 0, 0), xColor);
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, -arrowTip, 0), xColor);
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, arrowTip, 0), xColor);
        // Y-axis
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, radius, 0), yColor);
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(-arrowTip, -arrowTip, 0), yColor);
        CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(arrowTip, -arrowTip, 0), yColor);
      }
      
      // Create thin box-colliders on every line

    }

    /**************************************************************************/
    /*!
    @brief  The rotate tool allows the user to rotate an object.
    */
    /**************************************************************************/
    void Editor::RotateTool()
    {
      if (!SelectedObject)
        return;

      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        // Get the object's transform data
        auto transform = gameObject->getComponent<Transform>();
        Vec3 pos = transform->getTranslation();
        Real width = transform->getScale().x *2.5;
        Vec4 color(0.0f, 0.0f, 1.0f, 1.0);

        // Draw a selected 'box' around the object
        CurrentSpace->getComponent<GraphicsSpace>()->DrawCircle(pos, width, color);
      }

    }

    /**************************************************************************/
    /*!
    @brief  The scale tool allows the user to scale an object.
    */
    /**************************************************************************/
    void Editor::ScaleTool()
    {
      if (!SelectedObject)
        return;

        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Get the object's transform data
          auto transform = gameObject->getComponent<Transform>();
          Vec3 pos = transform->getTranslation();
          Real width = transform->getScale().x *2.5;
          Real height = transform->getScale().y *2.5;
          Vec4 color(0.0f, 0.0f, 1.0f, 1.0);

          // Draw a selected 'box' around the object
          CurrentSpace->getComponent<GraphicsSpace>()->DrawRectangle(pos,
            width, width, color);
        }
      
    }

    /**************************************************************************/
    /*!
    @brief  Moves an object by translation by the specified direction vector.
    @param  direction A vector.
    */
    /**************************************************************************/
    void Editor::MoveObject(Vec3 direction)
    {
      if (!SelectedObject)
        return;

      if (ActiveTool != EditorTool::Translate)
        return;

      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Transform>();
        Vec3 pos = transform->getTranslation();
        // Translate the object
        gameObject->getComponent<Transform>()->setTranslation(pos + direction);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scales an object by the specified vector.
    @param  scaleChange The scaling value.
    */
    /**************************************************************************/
    void Editor::ScaleObject(Vec3 scaleChange)
    {
      if (!SelectedObject)
        return;

      if (ActiveTool != EditorTool::Scale)
        return;

      if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Transform>();
        Vec3 scale = transform->getScale();
        // Translate the object
        gameObject->getComponent<Transform>()->setScale(scale + scaleChange);
      }

    }


  }
}