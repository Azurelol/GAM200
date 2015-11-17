/*****************************************************************************/
/*!
@file   Types.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/02/2015
@brief  Saves the types used in the Daisy Chain engine.
*/
/******************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <cassert>
#include <map>

#include <GLM\glm.hpp>
#include <GLM\matrix.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#define GLEW_STATIC
#include <GLEW\glew.h>

namespace DCEngine {
  
  // Reference: http://codereview.stackexchange.com/questions/48344/yet-another-any-class-implementation-named-some
  
  enum class Keys {
    Escape,
    Space ,
    Return,
    Up,
    Down,
    Left,
    Right,
    W,
    A,
    S,
    D,
    Q,
    E,
    Z,
    X,

    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Add,
    Subtract,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
  };

  enum class MouseButton {
    Left,
    Middle,
    Right
  };

  // Built-in types
  using String = std::string;
  using Boolean = bool;
  using Integer = int;
  using Real = float;
  using Vec2 = glm::vec2;
  using Vec3 = glm::vec3;
  using Vec4 = glm::vec4;
  
  using Mat3 = glm::mat3;
  using Mat4 = glm::mat4;

  // Common Containers
  using StringVec = std::vector<std::string>;

  struct OpenGLStateData {
    GLint lastProgram;
    GLint lastTexture;
    GLint lastArrayBuffer;
    GLint lastElementArrayBuffer;
    GLint lastVertexArray;
    GLint lastBlendSrc;
    GLint lastBlendDst;
    GLint lastBlendEquationRGB;
    GLint lastBlendEquationAlpha;
    GLboolean lastEnableBlend;
    GLboolean lastEnableCullFace;
    GLboolean lastEnableDepthTest;
    GLboolean lastEnableScissorTest;
  };


}
