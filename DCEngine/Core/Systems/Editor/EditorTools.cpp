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
    void Editor::DisplayTool()
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

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        // Do nothing if the GameObject does not have a transform.
        if (!gameObject->HasComponent(std::string("Transform")))
          return;

        // Get the object's position
        auto transform = gameObject->getComponent<Components::Transform>();

        // Draw a selected 'box' around the object
        Vec3 pos = transform->getTranslation();
        Real width = transform->getScale().x * 2;
        Real height = transform->getScale().y * 2;
        Vec4 color(1.0, 0, 0, 1.0);

        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos,
          width, height, color);
      }

    }

    /**************************************************************************/
    /*!
    @brief The translate tool allows the user to move an object on screen.
    @param The GameObject to which to apply the tool on.
    */
    /**************************************************************************/
    CommandObjectTransform* TransCommand = NULL;
    void Editor::UseTool(GameObjectPtr gameObject, Vec2& position)
    {
      if (TransCommand != NULL)
      {
        delete TransCommand;
        TransCommand = NULL;
      }

      // If the 'Translate' tool is active...
      if (ActiveTool == EditorTool::Translate) {
        // And a valid GameObject was selected, start dragging it
        if (SelectedObject && gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {
          Settings.Dragging = true;
          auto transform = gameObject->getComponent<Components::Transform>();
          TransCommand = new CommandObjectTransform(transform);
          DCTrace << "Editor::OnMouseDownEvent - Dragging: '" << gameObject->getObjectName() << "'\n";
          return;
        }

        //if we have a valid Object selected still
        else if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
          auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
          //and it has a transform
          if (transform != NULL)
          {
            auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(position);
            auto xPos = transform->getTranslation().x;
            auto yPos = transform->getTranslation().y;
            //data for translate editor tools
            Real radius = 8;
            Real arrowTip = 1;
            //if within horizontal tool, drag along x axis
            if (mousePos.x > xPos && mousePos.x < xPos + radius && mousePos.y < yPos + arrowTip && mousePos.y > yPos - arrowTip)
            {
              Settings.DraggingX = true;
              Settings.DragOffset = mousePos.x - xPos;

              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "Editor::OnMouseDownEvent - DraggingX: '" << SelectedObject->getObjectName() << "'\n";
              return;
            }
            //if within vertical tool, drag along y axis
            if (mousePos.x > xPos - arrowTip && mousePos.x < xPos + arrowTip && mousePos.y < yPos + radius && mousePos.y > yPos)
            {
              Settings.DraggingY = true;
              Settings.DragOffset = mousePos.y - yPos;
              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "Editor::OnMouseDownEvent - DraggingY: '" << SelectedObject->getObjectName() << "'\n";
              return;
            }
          }
        }
      }


      else if (ActiveTool == EditorTool::Rotate)
      {
        if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
          auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
          if (transform != NULL)
          {
            auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(position);
            auto xPos = transform->getTranslation().x;
            auto yPos = transform->getTranslation().y;
            Real radius = transform->getScale().x *2.5;
            Real radiusSquared = radius*radius;
            Real distanceSquared = (mousePos.x - xPos)*(mousePos.x - xPos) + (mousePos.y - yPos)*(mousePos.y - yPos);
            if (distanceSquared - radiusSquared < 1 && distanceSquared - radiusSquared > -1)
            {
              Settings.Rotating = true;
              Settings.OriginMousePos = mousePos;
              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "Editor::OnMouseDownEvent - Rotating: '" << SelectedObject->getObjectName() << "'\n";
              return;
            }
          }
        }
      }


      else if (ActiveTool == EditorTool::Scale)
      {
        if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
          auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
          if (transform != NULL)
          {
            auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(position);
            auto xPos = transform->getTranslation().x;
            auto yPos = transform->getTranslation().y;
            Real width = transform->getScale().x *2.5;
            Real height = transform->getScale().y *2.5;
            Real distanceX = mousePos.x - xPos;
            Real distanceY = mousePos.y - yPos;
            if (distanceY - height / 2 < 0.25 && distanceY - height / 2 > -0.25 && mousePos.x > xPos - width / 2 && mousePos.x < xPos + width / 2)
            {
              Settings.ScalingY = true;
              Settings.OriginScale = transform->getScale();
              Settings.OriginMousePos = mousePos;
              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "Editor::OnMouseDownEvent - ScalingY: '" << SelectedObject->getObjectName() << "'\n";
              return;
            }
            else if (distanceX - width / 2 < 0.25 && distanceX - width / 2 > -0.25 && mousePos.y > yPos - height / 2 && mousePos.y < yPos + height / 2)
            {
              Settings.ScalingX = true;
              Settings.OriginScale = transform->getScale();
              Settings.OriginMousePos = mousePos;
              TransCommand = new CommandObjectTransform(transform);
              DCTrace << "Editor::OnMouseDownEvent - ScalingX: '" << SelectedObject->getObjectName() << "'\n";
              return;
            }
          }
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Drags the object
    @param  The mouse's current position.
    */
    /**************************************************************************/
    void Editor::DragObject(Vec2& pos)
    {
      // Only drag GameObjects in the Space while in translate mode
      if (ActiveTool != EditorTool::Translate)
        return;

      // If the mouse is currently being dragged
      if (Settings.Dragging) {
        //DCTrace << "Dragging! \n ";
        // If the selected object is a GameObject on the space
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
          //auto mvtScale = EditorCamera->getComponent<Components::Camera>()->TransformComponent->getTranslation().z;
          // Move the object
          gameObject->getComponent<Components::Transform>()->setTranslation(Vec3(mousePos.x,
            mousePos.y,
            gameObject->getComponent<Components::Transform>()->getTranslation().z));
        }
      }
      if (Settings.DraggingX) {
        //DCTrace << "Dragging! \n ";
        // If the selected object is a GameObject on the space
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
          //auto mvtScale = EditorCamera->getComponent<Components::Camera>()->TransformComponent->getTranslation().z;
          // Move the object
          gameObject->getComponent<Components::Transform>()->setTranslation(Vec3(mousePos.x - Settings.DragOffset,
            gameObject->getComponent<Components::Transform>()->getTranslation().y,
            gameObject->getComponent<Components::Transform>()->getTranslation().z));
        }
      }
      if (Settings.DraggingY) {
        //DCTrace << "Dragging! \n ";
        // If the selected object is a GameObject on the space
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
          //auto mvtScale = EditorCamera->getComponent<Components::Camera>()->TransformComponent->getTranslation().z;
          // Move the object
          gameObject->getComponent<Components::Transform>()->setTranslation(Vec3(gameObject->getComponent<Components::Transform>()->getTranslation().x,
            mousePos.y - Settings.DragOffset,
            gameObject->getComponent<Components::Transform>()->getTranslation().z));
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Rotates the selected GameObject.
    @param  pos The mouse's current position.
    @param  pos The mouse's current position.
    */
    /**************************************************************************/
    void Editor::RotateObject(Vec2& pos)
    {
      if (ActiveTool != EditorTool::Rotate)
        return;

      if (Settings.Rotating) {
        auto gameObject = dynamic_cast<GameObject*>(SelectedObject);
        auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
        auto transform = gameObject->getComponent<Components::Transform>();
        auto normal = Vec3(transform->getTranslation().y - Settings.OriginMousePos.y, -(transform->getTranslation().x - Settings.OriginMousePos.x), 0);
        auto c = (normal.x * Settings.OriginMousePos.x + normal.y * Settings.OriginMousePos.y);
        auto dist = (normal.x*mousePos.x + normal.y*mousePos.y - c) / sqrt(normal.x*normal.x + normal.y*normal.y);

        normal *= 1 / sqrt(normal.x*normal.x + normal.y*normal.y);

        Vec4 color(1.0, 0.0, 0.0, 1.0); // Red
        Real radius = 1;

        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawCircle(dist*normal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), radius, color);

        transform->setRotation(Vec3(TransCommand->PreviousRotation.x, TransCommand->PreviousRotation.y, TransCommand->PreviousRotation.z + 360 * (static_cast<int>(dist) % 360) / 90));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scales the selected object.
    @param  pos The mouse's current position.
    */
    /**************************************************************************/
    void Editor::ScaleObject(Vec2& pos)
    {
      if (ActiveTool != EditorTool::Scale)
        return;

      if (Settings.ScalingX) {
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
          auto transform = gameObject->getComponent<Components::Transform>();

          gameObject->getComponent<Components::Transform>()->setScale(Vec3(Settings.OriginScale.x + Settings.OriginScale.x*((mousePos.x - Settings.OriginMousePos.x) / abs(Settings.OriginMousePos.x - transform->getTranslation().x)),
            gameObject->getComponent<Components::Transform>()->getScale().y,
            gameObject->getComponent<Components::Transform>()->getScale().z));
        }
      }
      if (Settings.ScalingY) {
        if (auto gameObject = dynamic_cast<GameObject*>(SelectedObject)) {
          // Calculate the current mouse position
          auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(pos);
          auto transform = gameObject->getComponent<Components::Transform>();

          gameObject->getComponent<Components::Transform>()->setScale(Vec3(transform->getScale().x,
            Settings.OriginScale.y + Settings.OriginScale.y*((mousePos.y - Settings.OriginMousePos.y) / abs(Settings.OriginMousePos.y - transform->getTranslation().y)),
            transform->getScale().z));
        }
      }
    }

    /**************************************************************************/
    /*!
    @brief  Releases the selected object at the current dragged position.
    */
    /**************************************************************************/
    void Editor::ReleaseObject()
    {
      if (!SelectedObject)
        return;

      if (Settings.Dragging || Settings.DraggingX || Settings.DraggingY) {

        // Snap the object to the nearest (x,y) snapDistance      
        if (Settings.Snapping) {
          auto& translation = dynamic_cast<GameObjectPtr>(SelectedObject)->getComponent<Components::Transform>()->getTranslation();
          auto snappedPos = Math::Snap(Vec2(translation.x, translation.y));
          dynamic_cast<GameObjectPtr>(SelectedObject)->getComponent<Components::Transform>()->setTranslation(Vec3(snappedPos.x, snappedPos.y, translation.z));

          DCTrace << "Editor::ReleaseObject - Releasing '" << SelectedObject->getObjectName() << "' at: \n"
            << "x: " << snappedPos.x << ", y: " << snappedPos.y << "\n";
        }
      }
    }


    /**************************************************************************/
    /*!
    @brief  Releases the use of the currently-activated tool.
    */
    /**************************************************************************/
    void Editor::ReleaseTool()
    {
      // Stop dragging
      if (Settings.Dragging || Settings.DraggingX || Settings.DraggingY) {
        ReleaseObject();
        Settings.Dragging = false;
        Settings.DraggingX = false;
        Settings.DraggingY = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
      }
      if (Settings.Rotating)
      {
        ReleaseObject();
        Settings.Rotating = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
      }
      if (Settings.ScalingX || Settings.ScalingY)
      {
        ReleaseObject();
        Settings.ScalingX = false;
        Settings.ScalingY = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
      }
    }

    /**************************************************************************/
    /*!
    @brief  Checks if one of editor's tool is being used within that
            object's area. Because different transform tools have
            different areas, we will perform different checks for each.
    */
    /**************************************************************************/
    bool Editor::IsToolRegion(GameObjectPtr)
    {
      // Different to
      return false;
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

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
        
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        Real radius = 8;
        Real arrowTip = 1;
        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
                                         // X-axis
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(radius, 0, 0), xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, -arrowTip, 0), xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, arrowTip, 0), xColor);
        // Y-axis
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, radius, 0), yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(-arrowTip, -arrowTip, 0), yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(arrowTip, -arrowTip, 0), yColor);
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

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        Real radius = transform->getScale().x *2.5;
        Vec4 color(0.0f, 0.0f, 1.0f, 1.0);
        if (Settings.Rotating == true)
        {
          auto normal = Vec3(transform->getTranslation().y - Settings.OriginMousePos.y, -(transform->getTranslation().x - Settings.OriginMousePos.x), 0);
          normal *= 10;
          auto negNormal = -normal;
          Vec4 colorR(1.0, 0.0, 0.0, 1.0); // Red
          Vec4 colorG(0.0,1.0, 0.0, 1.0); // Red
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(-(transform->getTranslation().y - Settings.OriginMousePos.y), transform->getTranslation().x - Settings.OriginMousePos.x, 0) + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorG);
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(negNormal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), normal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorR);
        }

        // Draw a selected 'box' around the object
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawCircle(pos, radius, color);
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

        if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
          // Get the object's transform data
          auto transform = gameObject->getComponent<Components::Transform>();
          Vec3 pos = transform->getTranslation();
          Real width = transform->getScale().x *2.5;
          Real height = transform->getScale().y *2.5;
          Vec4 color(0.0f, 0.0f, 1.0f, 1.0);

          // Draw a selected 'box' around the object
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos,
            width, height, color);
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

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
        
        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        // Translate the object
        auto TransCommand = new CommandObjectTransform(transform);
        transform->setTranslation(pos + direction);
        TransCommand->SaveNew(transform);
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        

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

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 scale = transform->getScale();
        // Translate the object
        auto TransCommand = new CommandObjectTransform(transform);
        transform->setScale(scale + scaleChange);
        TransCommand->SaveNew(transform);
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
      }

    }


  }
}