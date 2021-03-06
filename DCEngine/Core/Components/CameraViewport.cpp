/******************************************************************************/
/*!
\file   CameraViewport.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "CameraViewport.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(CameraViewport, "CameraViewport", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(CameraViewport);
      DCE_BINDING_DEFINE_PROPERTY(CameraViewport, Active);
      DCE_BINDING_DEFINE_PROPERTY(CameraViewport, Blocking);
      DCE_BINDING_DEFINE_PROPERTY(CameraViewport, Layer);
      DCE_BINDING_DEFINE_PROPERTY(CameraViewport, Background);
      DCE_BINDING_DEFINE_PROPERTY(CameraViewport, ViewportResolution);
    }
#endif


    /**************************************************************************/
    /*!
    @brief  CameraViewport constructor.
    */
    /**************************************************************************/
    CameraViewport::CameraViewport(Entity & owner) : Component(std::string("CameraViewport"), owner),
      CameraObj(nullptr), DefaultCameraObj(nullptr)
    {
    }


    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void CameraViewport::Initialize() {

      // Temporary: Look for the default camera object by searching
      // through the space's gameobjects and looking for one with the
      // camera component
      FindDefaultCamera();
    }

    /**************************************************************************/
    /*!
    @brief  Converts the Windows screenPos coordinates to OpenGL coordinates.
    @param  screenPoint The mouse position on the screenPos, as pixels with (0,0)
    @return A 2D vector containing the coordinates in world space.
    */
    /**************************************************************************/
    Vec2 CameraViewport::ScreenToViewport(Vec2 screenPoint)
    {
      // We need to grab the current viewport. The information we need is the
      // starting X and Y position of our GL viewport along with the width and
      // height.

      if (!CameraObj)
        return Vec2();

      auto screenCenter = Vec3(*CameraObj->ScreenWidth / 2, *CameraObj->ScreenHeight / 2, 0);
      auto vecCenter = Vec3(screenPoint, 0) - screenCenter;
      auto angle = (float)(CameraObj->FieldOfView) / 2;
      auto PI = 3.1415926535897f;

      auto distBetweenCameraAndPlane = CameraObj->TransformComponent->getTranslation().z;
      float height = tan(angle / 180 * PI) * distBetweenCameraAndPlane;
      height *= 2;
      float width = height * *CameraObj->ScreenWidth / *CameraObj->ScreenHeight;
      float widthRatio = vecCenter.x / *CameraObj->ScreenWidth;
      float heightRatio = vecCenter.y / *CameraObj->ScreenHeight;
      vecCenter.x = widthRatio * width + CameraObj->TransformComponent->Translation.x;
      vecCenter.y = CameraObj->TransformComponent->Translation.y - heightRatio * height;

      return Vec2(vecCenter.x, vecCenter.y);
    }

    /**************************************************************************/
    /*!
    @brief  Sets the viewport's default camera.
    @param  camera A pointer to the new camera to be used.
    /**************************************************************************/
    void CameraViewport::setCamera(Camera * camera)
    {
      // Set the previous camera to false
      if (CameraObj)
        CameraObj->Active = false;
      // Set the current camera
      CameraObj = camera;
      // If the camera pointer was invalid... 
      if (camera == nullptr) {
        DCTrace << "CameraViewport::setCamera - No active camera has been set. \n";
        return;
      }
      // Set it as active
      CameraObj->Active = true;
      DCTrace << "CameraViewport::setCamera - Setting " << camera->Owner()->Name() << " as the active camera \n";

    }

    /**************************************************************************/
    /*!
    @brief  Finds this space's default camera.
    @todo   Currently looking for the first Camera in the space.
    /**************************************************************************/
    Camera* CameraViewport::FindDefaultCamera() {
      // Look for a GameObject with the 'Camera' component in the space
      for (auto gameObj : *SpaceRef->AllObjects()) {

        // Skip the editor camera..
        if (gameObj->Name() == "EditorCamera")
          continue;

        auto camera = gameObj->getComponent<Camera>();
        // Do not set the EditorCamera as the space's default camera.
        if (camera != nullptr && camera->Owner()->Name() != std::string("EditorCamera")) {
          DCTrace << "CameraViewport::FindDefaultCamera - Setting " << camera->Owner()->Name() << " as the default camera \n";
          setCamera(camera);
          return camera;
          //DefaultCameraObj = camera;        
          //return DefaultCameraObj;
        }
      }
      DCTrace << "CameraViewport::FindDefaultCamera - No camera was found on the space \n";
      return nullptr;
      //CameraObj = SpaceRef->FindObjectByName("Camera")->getComponent<Camera>();
    }
  }
}