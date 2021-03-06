/*****************************************************************************/
/*!
@file      Actions.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      3/24/2016
@brief     ActionList interface class.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Action.h"
#include "ActionProperty.h"

namespace DCEngine {

  /*===================*
  *     Interface   *
  *===================*/
  /**************************************************************************/
  /*!
  @class The ActionsClass is the interface class that the client will be using
  for constructing and interacting with actions.
  */
  /**************************************************************************/
  class Actions : public Zilch::IZilchObject {
  public:
    ZilchDeclareBaseType(Actions, Zilch::TypeCopyMode::ReferenceType);
    // Action Sets
    static ActionSetPtr Sequence(ActionSet& owner);
    static ActionSetPtr Group(ActionSet& owner);
    // Delay
    static void Delay(ActionSetPtr setRef, Real duration);
    // Call
    template <typename Class, typename... Args> static void Call(ActionSetPtr sets,
      void(Class::*func)(Args...), Class* object, Args...);
    static void Call(Zilch::Call& call, Zilch::ExceptionReport& report);
    // Properties (C++)
    template <typename PropertyType>
    static void PropertyT(ActionSetPtr setRef, PropertyType & propertyRef, PropertyType val,
                          Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Boolean& prty, Boolean val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Integer& prty, Integer val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Real& prty, Real val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Vec2& prty, Vec2 val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Vec3& prty, Vec3 val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Vec4& prty, Vec4 val, Real duration, Ease ease);
    // Properties (Zilch)
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Real val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Boolean val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Integer val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, String val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec2 val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec3 val, Real duration, Ease ease);
    static void Property(ActionSetPtr setRef, Zilch::Handle propertyHandle, Vec4 val, Real duration, Ease ease);

  private:

  };

} 
#include "Actions.hpp"
