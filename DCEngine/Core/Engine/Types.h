#pragma once

#include <typeinfo>
#include <algorithm>
#include <GLM\glm.hpp>
// GLEW
#define GLEW_STATIC
#include <GLEW\glew.h>

//#include <BOOST\any.hpp>

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
  };

  // Built-in types
  using Real2 = glm::vec2;
  using Real3 = glm::vec3;
  using Real4 = glm::vec4;

}
