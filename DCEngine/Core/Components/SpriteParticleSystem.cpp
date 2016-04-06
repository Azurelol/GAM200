/******************************************************************************/
/*!
@file   SpriteParticleSystem.cpp
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2016
@brief  The SpriteParticleSystem component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SpriteParticleSystem.h"
#include "EngineReference.h"
#include "../Debug/DebugGraphics.h"

namespace DCEngine {

  namespace Components
  {
		GLuint SpriteParticleSystem::mTransformInstanceVBO;
		GLuint SpriteParticleSystem::mVAO;
		GLuint SpriteParticleSystem::mColorInstanceVBO;
		ShaderPtr SpriteParticleSystem::mShader;

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(SpriteParticleSystem, "SpriteParticleSystem", DCEngineCore, builder, type) {      
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(SpriteParticleSystem);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Visible);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Tint);

      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(SpriteSource);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Texture);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyTexture, attributeSpriteSource);

      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, VelocityScale);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, LengthScale);
      DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, SystemSize);

			DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Additive);
			DCE_BINDING_DEFINE_PROPERTY(SpriteParticleSystem, Lock);
    }
    #endif

    /*!************************************************************************\
    @brief  SpriteParticleSystem default constructor
    \**************************************************************************/
    SpriteParticleSystem::SpriteParticleSystem(Entity & owner)
			: Graphical("SpriteParticleSystem", owner), mParticleEmissionTimer(0), Visible(true)
    {
			TransformComponent = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
			// Register
			SpaceRef->getComponent<Components::GraphicsSpace>()->RegisterGraphicsComponent(this);
			srand(static_cast<unsigned>(time(NULL)));
    }



    /**************************************************************************/
    /*!
    @brief SpriteParticleSystem destructor
    */
    /**************************************************************************/
		SpriteParticleSystem::~SpriteParticleSystem(void)
		{
			SpaceRef->getComponent<GraphicsSpace>()->RemoveGraphicsComponent(this);
		}

    /*!************************************************************************\
    @brief  Initializes the SpriteParticleSystem.
    \**************************************************************************/
    void SpriteParticleSystem::Initialize()
    {
      // Grab a reference to the space's default camera
      CameraComponent = SpaceRef->getComponent<CameraViewport>()->getCamera();

      // Access to owner is automatically given to components through Owner()! Just need
      // to cast them into the type of entity the owner is.
      auto gameObjOwner = dynamic_cast<GameObject*>(Owner());

      // Access to the graphics system is already given
			mParticleEmitter = Owner()->getComponent<Components::ParticleEmitter>();
			mColorAnimator = Owner()->getComponent<Components::ParticleColorAnimator>();
			mLinearAnimator = Owner()->getComponent<Components::LinearParticleAnimator>();
    }

    /**************************************************************************/
    /*!
    @brief Updates the SpriteParticleSystem.
    @param dt The delta time.
    */
    /**************************************************************************/
    void SpriteParticleSystem::Update(float dt)
    {
      if (mParticleEmitter)
      {
        if (mActiveFlag)
        {
          if (mParticleEmitter->Active == false)
          {
            mEmitCounter = 0;
            mParticleEmissionTimer = 0;
            mActiveFlag = false;
          }
					if (mParticleEmitter->ResetCount)
					{
						mEmitCounter = 0;
						mParticleEmissionTimer = 0;
						mParticleEmitter->ResetCount = false;
					}
          mParticleEmissionTimer -= dt;

          if (mParticleEmitter->EmitRate > 0)
          {
            if (mParticleEmissionTimer <= 0)
            {
              mParticleEmissionTimer += 1.f / float(mParticleEmitter->EmitRate);
              if (mParticleEmitter->EmitCount > 0)
              {
                if (mParticleEmitter->EmitCount > static_cast<int>(mEmitCounter))
                {
                  AddParticle();
                  ++mEmitCounter;
                }
              }
              else
                AddParticle();
            }
          }
        }
        else
        {
          if (mParticleEmitter->Active)
          {
            mActiveFlag = true;
          }
					if (mParticleEmitter->ResetCount)
					{
						mParticleEmitter->ResetCount = false;
					}
        }
        for (auto&& particle : mParticleList)
        {
          particle.Update(dt);
        }
        mParticleList.erase(std::remove_if(mParticleList.begin(), mParticleList.end(),
          [](const Particle& p) { return p.GetLifeleft() <= 0; }), mParticleList.end());
      }
    }

    /**************************************************************************/
    /*!
    @brief Sets the uniforms of the particle system.
    @param shader A pointer to the shader.
    */
    /**************************************************************************/
		void SpriteParticleSystem::SetUniforms(ShaderPtr shader, Camera* camera, Light * light)
		{
			if (!shader)
				shader = mShader;
			shader->Use();
			Components::Transform* transform = TransformComponent;

			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->Translation.x,
				transform->Translation.y,
				transform->Translation.z));
			modelMatrix = glm::scale(modelMatrix,
				glm::vec3(transform->Scale.x, transform->Scale.y, 0.0f));

			std::vector<glm::vec2> offset(GetPositionData());
			std::vector<float> rotation(GetRotationData());
			std::vector<float> scale(GetScaleData());
			
			for (unsigned i = 0; i < GetParticleCount(); ++i)
			{
				glm::mat4 modelMatrix;
				if (Lock)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(
						transform->Translation.x + offset[i].x, transform->Translation.y + offset[i].y,
						transform->Translation.z));
				else
					modelMatrix = glm::translate(modelMatrix, glm::vec3(offset[i].x, offset[i].y,
						transform->Translation.z));
				modelMatrix = glm::rotate(modelMatrix, rotation[i], glm::vec3(0, 0, 1));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scale[i], scale[i], 0.0f));
				transformData.push_back(modelMatrix);
			}

			//texture info
			SpriteSourcePtr src = Daisy->getSystem<Systems::Content>()->getSpriteSrc(Texture);
			mShader->SetFloat("CutMinX", (float)src->MinX / src->PicWidth);
			mShader->SetFloat("CutMaxX", (float)src->MaxX / src->PicWidth);
			mShader->SetFloat("CutMinY", (float)src->MinY / src->PicHeight);
			mShader->SetFloat("CutMaxY", (float)src->MaxY / src->PicHeight);
			glActiveTexture(GL_TEXTURE0); // Used for 3D???
			src->getTexture().Bind();

			mShader->SetVector4f("spriteColor", Tint);

			// Set the projection matrix
			shader->SetMatrix4("projection", camera->GetProjectionMatrix());
			// Set the view matrix 
			shader->SetMatrix4("view", camera->GetViewMatrix());
		}

    /**************************************************************************/
    /*!
    @brief Draws all active particles.
    @param camera Reference to the current camera in the space.
    */
    /**************************************************************************/
		void SpriteParticleSystem::Draw(void)
		{
			//mShader->Use();
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			std::vector<glm::vec2> offset(GetPositionData());
			std::vector<glm::vec4> color(GetColorData());
			if(getAdditive())
				glBlendFunc(GL_ONE, GL_ONE);
			else
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (Visible)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mColorInstanceVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::vec4) * GetParticleCount(), color.data());
				glBindBuffer(GL_ARRAY_BUFFER, mTransformInstanceVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0,
					sizeof(glm::mat4) * GetParticleCount(), transformData.data());
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindVertexArray(mVAO);
				glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4,  static_cast<GLsizei>(offset.size()));
				glBindVertexArray(0);
			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_TRUE);
			transformData.clear();
		}


    /**************************************************************************/
    /*!
    @brief Generates a particle.
    */
    /**************************************************************************/
		void SpriteParticleSystem::AddParticle(void)
		{
			unsigned emitCount = 1 + rand() % (mParticleEmitter->EmitVariance + 1);
			for (unsigned i = 0; i < emitCount; ++i)
			{
				float lifetime = mParticleEmitter->Lifetime + mParticleEmitter->LifetimeVariance * (rand() % 100 - 50) / 100;
				Vec2 velocity = Vec2(mParticleEmitter->StartVelocity.x + mParticleEmitter->RandomVelocity.x * (rand() % 100 - 50) / 50,
					mParticleEmitter->StartVelocity.y + mParticleEmitter->RandomVelocity.y * (rand() % 100 - 50) / 50);
				float size = mParticleEmitter->Size + mParticleEmitter->SizeVariance * (rand() % 100 - 50) / 100;
				float spin = mParticleEmitter->Spin + mParticleEmitter->SpinVariance * (rand() % 100 - 50) / 100;
				Vec2 force = Vec2(0, 0);
				Vec2 position = Vec2(0, 0);
				if (mLinearAnimator)
					force = Vec2(mLinearAnimator->Force.x + mLinearAnimator->RandomForce.x * (rand() % 100 - 50) / 50,
						mLinearAnimator->Force.y + mLinearAnimator->RandomForce.y * (rand() % 100 - 50) / 50);
				if (!Lock)
					position = Vec2(TransformComponent->Translation.x, TransformComponent->Translation.y);
				mParticleList.push_back(Particle(
					lifetime, position, velocity, force, size, spin, Tint, mColorAnimator, mLinearAnimator));
			}
		}

    /**************************************************************************/
    /*!
    @brief Grabs the position data of all active particles in the system.
    @return A container containing the position data.
    */
    /**************************************************************************/
		std::vector<Vec2> SpriteParticleSystem::GetPositionData(void)
		{
			std::vector<Vec2> positions;
			for (auto&& particle : mParticleList)
			{
				positions.push_back(particle.GetPosition());
			}
			return positions;
		}

    /**************************************************************************/
    /*!
    @brief Grabs the scale data of all active particles in the system.
    @return A container containing the scale data.
    */
    /**************************************************************************/
		std::vector<float> SpriteParticleSystem::GetScaleData(void)
		{
			std::vector<float> scale;
			for (auto&& particle : mParticleList)
			{
				scale.push_back(particle.GetScale());
			}
			return scale;
		}

    /**************************************************************************/
    /*!
    @brief Grabs the rotation data of all active particles in the system.
    @return A container containing the rotation data.
    */
    /**************************************************************************/
		std::vector<float> SpriteParticleSystem::GetRotationData(void)
		{
			std::vector<float> rotation;
			for (auto&& particle : mParticleList)
			{
				rotation.push_back(particle.GetRotation());
			}
			return rotation;
		}

    /**************************************************************************/
    /*!
    @brief Grabs the color data of all active particles in the system.
    @return A container containing the color data.
    */
    /**************************************************************************/
		std::vector<Vec4> SpriteParticleSystem::GetColorData(void)
		{
			std::vector<Vec4> color;
			for (auto&& particle : mParticleList)
			{
				color.push_back(particle.GetColor());
			}
			return color;
		}

    /**************************************************************************/
    /*!
    @brief  Returns the number of active particles in the system.
    @return A count of all active particles.
    */
    /**************************************************************************/
		unsigned SpriteParticleSystem::GetParticleCount(void)
		{
			return static_cast<unsigned>(mParticleList.size());
		}
  }
}