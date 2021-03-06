/*****************************************************************************/
/*!
\file   GraphicsSpace.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  This component receives drawing requests from entities with Sprite,
SpriteText and Model components, and passes the requests to the
engine's underlying graphics system.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Objects/Entities/GameObject.h"
#include "../Objects/DebugDraw.h"
#include "../Systems/Graphics/GraphicsGL.h"

namespace DCEngine {

  namespace Systems {
    class Graphics;
    class GraphicsGL::GraphicsBuffers;
  }
  namespace Components {

    class Camera;
    class CameraViewport;
    
    class Light;

    // Containers
    using SpriteTextContainer = std::vector<SpriteText*>;
    using SpriteContainer = std::vector<Sprite*>;
    using GraphicsComponentContainer = std::vector<Graphical*>;
    using LightComponentContainer = std::vector<Light*>;

    class GraphicsSpace : public Component {
      friend class Systems::Graphics;

    public:

      #if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(GraphicsSpace, Component);
      #endif

      SpriteLayerOrderHandle SpriteLayerOrder;
      bool Active = true; // Prevent visibility, particle system and render updates
                                
      DCE_DEFINE_PROPERTY(bool, Active);
      DCE_DEFINE_PROPERTY(SpriteLayerOrderHandle, SpriteLayerOrder);

      GraphicsSpace(Entity& owner);
      ~GraphicsSpace();
      void Initialize();

      void RegisterGraphicsComponent(Graphical* graphicsComponent);
      void RemoveGraphicsComponent(Graphical* graphicsComponent);
      GraphicsComponentContainer getGraphicsComponents(void);

      void RegisterLightComponent(Light* lightComponent);
      void RemoveLightComponent(Light* lightComponent);
      LightComponentContainer getLightComponents(void);

      //void AddSprite(Sprite& sprite);
      //void RemoveSprite(Sprite& sprite);
      //void AddSpriteText(SpriteText& spriteText);
      //void RemoveSpriteText(SpriteText& spriteText);
      //void AddParticleSystem(SpriteParticleSystem& particleEmitter);
      //void RemoveParticleSystem(SpriteParticleSystem& particleEmitter);

      // Debug Drawing functions 
      void DrawCircle(const Vec3& pos, Real radius, const Vec4& color, bool fill = false) const;
      void DrawRectangle(const Vec3& pos, Real width, Real height, const Vec4& color, bool fill = false) const;
      void DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color) const;

      /* Getters */
      //SpriteContainer getSprites(); // Return the vector of sprites to be drawn
      //SpriteTextContainer getSpriteTextContainer(); // Returns the container of SpriteText to be drawn
      //std::vector<SpriteParticleSystem*> getParticleSystem(); // Returns the emission components


      void SetResolution(unsigned x, unsigned y);
      void SetAntiAliasingLevel(unsigned samples);
      void ToggleFullscreen();
      unsigned GetScreenWidth() const;
      unsigned GetScreenHeight() const;
      void SetNative();


      void OnLogicUpdate(Events::LogicUpdate* updateEvent);
    private:
      Systems::Graphics* GraphicsSystem;
      Systems::GraphicsGL::GraphicsBuffers buffers;
      CameraViewport* CameraViewportComponent;
      GameObjectRawVec models_;  //!< Container of models that need to be rendered
      //SpriteContainer SpriteContainer; //!< Container of sprites that need to be drawn    
      //SpriteTextContainer SpriteTextContainer;  //!< Container of models that need to be rendered
      //std::vector<SpriteParticleSystem*> ParticleSystemContainer;
      GraphicsComponentContainer GraphicsComponents;
      LightComponentContainer LightComponents;
      DebugDrawObjVec DebugDrawObjContainer; //!< Container of debug draw objects to be drawn this frame.
    };

  }
}