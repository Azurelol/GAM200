/******************************************************************************/
/*!
@file   LinearParticleAnimator.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The LinearParticleAnimator component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Graphical.h"

#include "../Resources/SpriteSource.h"

namespace DCEngine {
  namespace Components
  {
    class SpriteParticleSystem;
    class LinearParticleAnimator : public Graphical {
    public:
      // Properties
      Vec3 Force;
      Vec3 RandomForce;
      Real Torque;
      Real Growth;
      Real Dampening;
      DCE_DEFINE_PROPERTY(Vec3, Force);
      DCE_DEFINE_PROPERTY(Vec3, RandomForce);
      DCE_DEFINE_PROPERTY(Real, Torque);
      DCE_DEFINE_PROPERTY(Real, Growth);
      DCE_DEFINE_PROPERTY(Real, Dampening);

      ZilchDeclareDerivedType(LinearParticleAnimator, Graphical);
      LinearParticleAnimator(Entity& owner);
			virtual ~LinearParticleAnimator(void);
      void Initialize();

    private:
      SpriteParticleSystem* ParticleSystem;

    };
  }

}
