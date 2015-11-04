/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  The graphics system is the bridge between the rendering calls from the 
        entity components and the underlying low-level OpenGL renderer. It
        receives rendering requests and draws on the window.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"
#include "../../Components/GraphicsSpace.h"

namespace DCEngine {  
  class Engine;
  
  class Camera;
  class Sprite;
  class SpriteText;
  class DebugDrawObject;

  namespace Systems {
    class WindowSFML;

    class Graphics : public System {
      friend class Engine;
      friend class WindowSFML;
    public:


      void Register(GraphicsSpace& graphicsSpace);
      
      void DrawSprite(Sprite& sprite, Camera& camera, float dt);
      void DrawSpriteText(SpriteText& st, Camera& cam);
      void DrawModel(GameObject& gameObj);
      void DrawDebug(DebugDrawObject& debugDraw);  

      /* Debug Drawing functions*/
      void DrawCircle(Vec3& pos, Real& radius, Vec4& color, Camera& cam);
      void DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color, Camera& cam);
      void DrawLineSegment(Vec3& startPos, Vec3& endPos, Real& length, Vec4& color, Camera& cam);

    private:

      void StartFrame();
      void EndFrame();
      void BackupState();
      void RestoreState();

      std::unique_ptr<GraphicsGL> GraphicsHandler;
      const int screenwidth_ = 1024;
      const int screenheight_ = 768;
      Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
      Mat4 ProjMatrix;
      Mat4 ViewMatrix;
      Mat4 ViewProjMatrix;
      std::vector<GraphicsSpace*> graphicsSpaces_; //!< Container of graphics spaces accessing this system

      Graphics();
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

	  //2D draw list
	  int TotalObjNumG = 0;
	  int TotalObjTranspNumG = 0;
	  
	  std::vector<Sprite*>  NonTextureObjNontransp;
	  std::vector<Sprite*>  TransparentObj;
	  void SendCountToGL(int TotalObjNumG, int TotalObjTransNumG);
    };


  }

}
