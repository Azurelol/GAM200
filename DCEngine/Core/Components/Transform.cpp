/******************************************************************************/
/*!
@file   Transform.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/05/2015
@brief  The transform component allows the representation of this object in
world space, allowing it to be drawn, take part in collisions and
force-based movement through the addition of other components.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    ZilchDefineType(Transform, "Transform", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Transform);
      // Properties
      ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
      ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
      ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
    }

    /**************************************************************************/
    /*!
    @brief Initializes the GameObject.
    @note  The WorldRotation's Z component is set at 0 because ???
    */
    /**************************************************************************/
    TransformDataPair Transform::getTransformDataPair()
    {
      TransformDataPair data;
      data.first = this;
      data.second.Translation = Translation;
      data.second.Rotation = Rotation;
      data.second.Scale = Scale;
      return data;
    }

    /**************************************************************************/
    /*!
    @brief Initializes the GameObject.
    @note  The WorldRotation's Z component is set at 0 because ???
    */
    /**************************************************************************/
    void Transform::Initialize() {
      if (DCE_TRACE_COMPONENT_INITIALIZE)
        DCTrace << Owner()->Name() << "::" << Name() << "::Initialize\n";
      WorldRotation.z = 0;
    }

    /**************************************************************************/
    /*!
    @brief Updates the GameObject's translation with respect to its origin.
    @note The origin is relative to its parent. By default the GameObject is
          parented to the Space. When attached to another GameObject, that
          GameObject's translation becomes its origin.
    */
    /**************************************************************************/
    void Transform::UpdateTranslation()
    {

      if (firstloop)
      {
        PrevTranslation = Translation;
        return;
      }

      auto physpace = this->SpaceRef->getComponent<Components::PhysicsSpace>();

      if (Translation.x > physpace->MaxX)
      {
        physpace->MaxX = Translation.x;
      }

      if (Translation.y > physpace->MaxY)
      {
        physpace->MaxY = Translation.y;
      }

      if (Translation.x < physpace->MinX)
      {
        physpace->MinX = Translation.x;
      }

      if (Translation.y < physpace->MinY)
      {
        physpace->MinY = Translation.y;
      }


      if (dynamic_cast<GameObject*>(this->Owner()))
      {
        GameObjectVec &children = (dynamic_cast<GameObject*>(this->Owner())->Children());


        if (Translation != PrevTranslation)
        {
          for (auto child : children)
          {
            child->getComponent<Components::Transform>()->Translation += (Translation - PrevTranslation);
            child->getComponent<Components::Transform>()->UpdateTranslation();
          }
        }

        PrevTranslation = Translation;
      }
    }

    /**************************************************************************/
    /*!
    @brief Updates the GameObject's rotation with respect to its origin.
    */
    /**************************************************************************/
    void Transform::UpdateRotation()
    {
      if (firstloop)
      {
        PrevRotation = Rotation;
        firstloop = false;
        return;
      }

      if (dynamic_cast<GameObject*>(this->Owner()))
      {
        GameObjectVec &children = (dynamic_cast<GameObject*>(this->Owner())->Children());

        if (Rotation != PrevRotation)
        {
          for (auto child : children)
          {
            Vec3 temp = RotatePoint(child->getComponent<Components::Transform>()->Translation, Translation, (Rotation.z - PrevRotation.z));
            child->getComponent<Components::Transform>()->Translation = temp;
            child->getComponent<Components::Transform>()->Rotation.z += (Rotation.z - PrevRotation.z);
            child->getComponent<Components::Transform>()->UpdateTranslation();
            child->getComponent<Components::Transform>()->UpdateRotation();
          }
        }

      }
    }

    /**************************************************************************/
    /*!
    @brief Updates the GameObject's scale with respect to its origin.
    */
    /**************************************************************************/
    void Transform::UpdateScale()
    {

    }

    /**************************************************************************/
    /*                          Properties                                    */
    /**************************************************************************/
    Vec3 Transform::RotatePoint(Vec3 point, Vec3 rotation, float angle)
    {
      point -= rotation;
      Vec3 temp = point;

      point.x = cos(temp.x) + -sin(temp.y);
      point.y = sin(temp.x) + cos(temp.y);

      point += rotation;

      return point;
    }
    /*
    const Vec3 &Transform::getRotation(void)
    {
      DRotation = Rotation * (180.0f / PI);

      return DRotation;
    }
    void Transform::setRotation(Vec3 const & rot)
    {
      Rotation = rot * (PI / 180.0f);
    }*/


  }
}