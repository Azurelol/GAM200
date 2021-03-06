/******************************************************************************/
/*!
@file   Graphical.h
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/22/2016
@brief  The Graphical component is the base class from which all graphical
        components derive frm.

@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "Camera.h"
#include "Light.h"

namespace DCEngine {
  namespace Components {

    class Graphical : public Component {

      // Properties
		public:
			
      ZilchDeclareDerivedType(Graphical, Component);
      Graphical(std::string graphicalComponent, Entity& owner);
      virtual ~Graphical();
      virtual void Initialize();
			virtual void Update(float dt) {}
			virtual void SetUniforms(ShaderPtr shader, Camera* camera, Light* light) {}
			virtual void Draw(void) {}
			virtual void SetShader(void) {}
			DCE_DEFINE_PROPERTY(unsigned, DrawLayer);
		protected:
			unsigned DrawLayer;

    };



  }
}