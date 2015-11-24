/******************************************************************************/
/*!
\file   GraphicsEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/6/2015
\brief  Events for the graphics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {
  namespace Events {

    class FullscreenEnabledEvent : public Event {
    };

    class ResizeViewportEvent : public Event {
    public:
      Vec2 viewportScale;
    };

  }
}