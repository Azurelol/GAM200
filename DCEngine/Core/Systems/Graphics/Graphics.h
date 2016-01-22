/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  The graphics system is the bridge between the rendering calls from the
entity components and the underlying low-level OpenGL renderer. It
receives rendering requests and draws on the window.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"
#include "../../Components/GraphicsSpace.h"
#include "../../Components/SpriteParticleSystem.h"

namespace DCEngine {
	class Engine;

	class Camera;
	class Sprite;
	class SpriteText;
	class DebugDrawObject;

  struct GraphicsConfig {
    int MaxDrawLayers;
  };


	namespace Systems {
		class WindowSFML;

		class Graphics : public System {
			friend class Engine;
			friend class WindowSFML;
			friend class GraphicsGL;
		public:

      // Registration
			void Register(Components::GraphicsSpace& graphicsSpace);
			void Deregister(Components::GraphicsSpace& graphicsSpace);
      // Draw
			void DrawSprite(Components::Sprite& sprite, Components::Camera& camera, float dt);
			void DrawSpriteText(Components::SpriteText& st, Components::Camera& cam);
			void DrawParticles(Components::SpriteParticleSystem& particles, Components::Camera& cam, double dt);
			void DrawModel(GameObject& gameObj);
			void DrawDebug(DebugDrawObject& debugDraw);
			// DebugDraw
			void DrawCircle(Vec3& pos, Real& radius, Vec4& color, Components::Camera& cam);
			void DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color, Components::Camera& cam);
			void DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color, Components::Camera& cam);

		private:

      GraphicsConfig Settings;
			std::unique_ptr<GraphicsGL> GraphicsHandler;
			const int screenwidth_ = 1024;
			const int screenheight_ = 768;
			Vec4 ClearColor = Vec4(0.0f, 0.5f, 1.0f, 1.0f);
			Vec2 ViewportScale = Vec2(1, 1);
			Mat4 ProjMatrix;
			Mat4 ViewMatrix;
			Mat4 ViewProjMatrix;
			std::vector<Components::GraphicsSpace*> graphicsSpaces_;

			// Base methods
			void StartFrame();
			void EndFrame();
			void BackupState();
			void RestoreState();
			// Events
			void OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent* event);
			void OnResizeViewportEvent(Events::ResizeViewportEvent* event);
      // CTOR/ DTOR, Initialize
			Graphics(GraphicsConfig settings);
			void Initialize();
			void Subscribe();
			void Update(float dt);
			void Terminate();

			//2D draw list
			//int TotalObjNumG = 0;
			//int TotalObjTranspNumG = 0;
			std::vector<std::vector<Components::Sprite*>> mDrawList;
			//std::vector<Components::Sprite*>  NonTextureObjNontransp;
			//std::vector<Components::Sprite*>  TextureObjNontransp;
			//std::vector<Components::Sprite*> NonTextureObjtransp;
			//std::vector<Components::Sprite*> TextureObjtransp;
			void SendCountToGL(int TotalObjNumG, int TotalObjTransNumG);
		};


	}

}