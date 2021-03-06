/******************************************************************************/
/*!
\file   Keyboard.h
\author Connor Tilley, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The keyboard input inteface for the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Keyboard.h"
#include "../../Engine/Engine.h"

namespace DCEngine {

  Keyboard * Keyboard::Access()
  {
    return Daisy->getKeyboard();
  }

  bool Keyboard::KeyIsDown(Keys key)
  {
    switch (key) {

	/*case Keys::Escape:
		if (KeyDown_Escape)
			return true;
		else
			return false;
		break;*/
    case Keys::Space:
      if (KeyDown_Space)
        return true;
      else
        return false;
      break;
    case Keys::LControl:
      if (KeyDown_LControl)
        return true;
      else
        return false;
      break;
    case Keys::RControl:
      if (KeyDown_RControl)
        return true;
      else
        return false;
      break;
    case Keys::LShift:
      if (KeyDown_LShift)
        return true;
      else
        return false;
      break;
    case Keys::RShift:
      if (KeyDown_RShift)
        return true;
      else
        return false;
      break;
    case Keys::Q:
      if (KeyDown_Q)
        return true;
      else
        return false;
      break;
    case Keys::W:
      if (KeyDown_W)
        return true;
      else
        return false;
      break;
    case Keys::E:
      if (KeyDown_E)
        return true;
      else
        return false;
      break;
    case Keys::R:
      if (KeyDown_R)
        return true;
      else
        return false;
      break;
    case Keys::T:
      if (KeyDown_T)
        return true;
      else
        return false;
      break;
    case Keys::Y:
      if (KeyDown_Y)
        return true;
      else
        return false;
      break;
    case Keys::U:
      if (KeyDown_U)
        return true;
      else
        return false;
      break;
    case Keys::I:
      if (KeyDown_I)
        return true;
      else
        return false;
      break;
    case Keys::O:
      if (KeyDown_O)
        return true;
      else
        return false;
      break;
    case Keys::P:
      if (KeyDown_P)
        return true;
      else
        return false;
      break;



    case Keys::A:
      if (KeyDown_A)
        return true;
      else
        return false;
      break;

    case Keys::S:
      if (KeyDown_S)
        return true;
      else
        return false;
      break;

    case Keys::D:
      if (KeyDown_D)
        return true;
      else
        return false;
      break;
    case Keys::F:
      if (KeyDown_F)
        return true;
      else
        return false;
      break;
    case Keys::G:
      if (KeyDown_G)
        return true;
      else
        return false;
      break;
    case Keys::H:
      if (KeyDown_H)
        return true;
      else
        return false;
      break;
    case Keys::J:
      if (KeyDown_J)
        return true;
      else
        return false;
      break;
    case Keys::K:
      if (KeyDown_K)
        return true;
      else
        return false;
      break;
    case Keys::L:
      if (KeyDown_L)
        return true;
      else
        return false;
      break;



    case Keys::Z:
      if (KeyDown_Z)
        return true;
      else
        return false;
      break;

    case Keys::X:
      if (KeyDown_X)
        return true;
      else
        return false;
      break;

    case Keys::C:
      if (KeyDown_C)
        return true;
      else
        return false;
      break;
    case Keys::V:
      if (KeyDown_V)
        return true;
      else
        return false;
      break;
    case Keys::B:
      if (KeyDown_B)
        return true;
      else
        return false;
      break;
    case Keys::N:
      if (KeyDown_N)
        return true;
      else
        return false;
      break;
    case Keys::M:
      if (KeyDown_M)
        return true;
      else
        return false;
      break;




    default:
      break;

    }

    return false;
  }

  bool Keyboard::KeyIsUp(Keys key)
  {
    return !(KeyIsDown(key));
  }

  bool Keyboard::KeyIsTriggered(Keys key)
  {
	  return false;
  }

  bool Keyboard::KeyIsReleased(Keys key)
  {
	  return false;
  }


  void Keyboard::Initialize() {
    //Connect(this, Events::KeyPressed, Keyboard::OnKeyPressedEvent);

  }


}