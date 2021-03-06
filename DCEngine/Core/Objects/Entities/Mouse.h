/******************************************************************************/
/*!
\file   Mouse.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The mouse input inteface for the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../Entity.h"

namespace DCEngine {

  class Engine;

  namespace Systems {
    class InputSFML;
  }
  
  class Mouse : public Entity {
    friend class Engine;
    friend class Systems::InputSFML;

  public:

    static Mouse* Access();
    ZilchDeclareDerivedType(Mouse, Entity);
    Mouse::Mouse() : Entity(std::string("Mouse")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << " Interface - Constructor \n";
    }

    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);
    DCE_DEFINE_PROPERTY(Vec2, ScreenPosition);

  private:

    // Variables
    Vec2 ScreenPosition;    
    bool MouseDown_Left = false;
    bool MouseDown_Right = false;
    bool MouseDown_Middle = false;
    // Methods
    void Initialize();
    

  };

  using MousePtr = std::unique_ptr<Mouse>;
}