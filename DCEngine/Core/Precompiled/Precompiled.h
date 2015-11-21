/******************************************************************************/
/*!
@file   Precompiled.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/1/2015
@brief  The Precompiled header for Daisy Chain Engine.
@note   The use of this file requires that it be included in every translational
        unit (.cpp) in the project.
*/
/******************************************************************************/
#pragma once

/**************************************************************************/
/*!
      STANDARD LIBRARY / STANDARD TEMPLATE LIBRARY
*/
/**************************************************************************/
#include <memory>
#include <unordered_map>
#include <string>

/**************************************************************************/
/*!
      MULTIMEDIA LIBRARIES
*/
/**************************************************************************/
// SFML

#include <SFML\System\Vector2.hpp>
// GLEW

// FMOD
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

/**************************************************************************/
/*!
    BINDING
*/
/**************************************************************************/
#include <ZILCH\Zilch.hpp>

/**************************************************************************/
/*!
      ENGINE FILES
*/
/**************************************************************************/
#include "../Engine/Types.h"
#include "../Engine/Macros.h"
//#include <Windows.h>