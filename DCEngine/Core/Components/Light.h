/******************************************************************************/
/*!
@file   Light.h
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2015
@brief  Light component for dynamic scene lightning.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  enum class LightType {
    Point,
    Spot,
    Ambient,
    Directional,
    Box
  };

  namespace Components {

    class Light : public Component {
    public:

      /*=============
         Properties
      ==============*/
      // If the light should be rendered
      DCE_DEFINE_PROPERTY(bool, Visible);
      // If the object is culled and not graphically updated when its not visible
      DCE_DEFINE_PROPERTY(bool, VisibilityCulling);
      // If the object receives events when entering/exiting the view of a rendering camera or shadow
      DCE_DEFINE_PROPERTY(bool, VisibilityEvents);
      // If the object is rendered to shadow maps
      DCE_DEFINE_PROPERTY(bool, CastShadows);
			// If the light illuminates directional faces only
			DCE_DEFINE_PROPERTY(bool, Diffuse);
      // The type of the light (Point, Spot, Directional, etc...)
      DCE_DEFINE_PROPERTY(LightType, Type);
      int getTypeAsInt();
      //DCE_DEFINE_PROPERTY(int, Type);
      // The color of the light.
      DCE_DEFINE_PROPERTY(Vec4, Color);
      // The intensity of the light
      DCE_DEFINE_PROPERTY(float, Intensity);
      // The maximum range that the light can reach.
      DCE_DEFINE_PROPERTY(float, Range);
      // Falloff exponent. 1 is linear.
      DCE_DEFINE_PROPERTY(float, Falloff);
      // The angle of the light
      DCE_DEFINE_PROPERTY(Vec3, DirectionVector);
      DCE_DEFINE_PROPERTY(float, InnerAngle);
			DCE_DEFINE_PROPERTY(float, OuterAngle);
			DCE_DEFINE_PROPERTY(float, CullLight);
      
      ZilchDeclareDerivedType(Light, Component);
      Light(Entity& owner);
      ~Light();
      void Initialize();
			void Register();

    private:
			//NOTE: these variables must match the light struct in the sprite shader
      bool Visible; 
      bool VisibilityCulling;
      bool VisibilityEvents; 
      bool CastShadows;
			bool Diffuse;
      LightType Type;
      //int Type;
      Vec4 Color;
      float Intensity;
      float Range;
      float Falloff;
			Vec3 DirectionVector;
      float InnerAngle;
			float OuterAngle;
			float CullLight;
    };


  }
}

ZilchDeclareExternalBaseType(DCEngine::LightType, Zilch::TypeCopyMode::ValueType);