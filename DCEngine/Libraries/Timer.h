/******************************************************************************/
/*!
\file   Timer.h
\author Allan Deutsch, Christian Sagel (Glorified code-parrot)
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief
The timer handles ??

Functions include:
- ScopeTimer
Description here.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#pragma once

#include <chrono> // http://www.cplusplus.com/reference/chrono/
/* More references;
  http://www.informit.com/articles/article.aspx?p=1881386&seqNum=2 */

namespace DCEngine {

  namespace Time {

    // A framework that relates a time point to real physical time
    using clock = std::chrono::high_resolution_clock;
    // A reference to a specific point in time, like one's birthday, etc
    //  high_resolution_clock is the clock with the shortest tick period
    using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

    using ms = std::chrono::milliseconds;
    using us = std::chrono::microseconds;
    // Durations measure time spans, like one minute, two hours, ten ms
    using floatSeconds = std::chrono::duration<float>;

  };

  class ScopeTimer {
  public:

    /**************************************************************************/
    /*!
    \brief
    The constructor..
    \param output
    A pointer to a float.
    */
    /**************************************************************************/
    ScopeTimer(float* output) : _Start(Time::clock::now()), _RetVal(output) {}

    /**************************************************************************/
    /*!
    \brief
    The destructor. When the timer leaves the scope, gets the end time. It then
    converts the duration to a float where 1.0f == 1 second.
    It then stores the result in a pointer given at initialization.
    */
    /**************************************************************************/
    ~ScopeTimer() {

      _End = Time::clock::now();
      Time::floatSeconds duration = _End - _Start;
      *_RetVal = duration.count();

    }

  private:
    Time::timePoint _Start;
    Time::timePoint _End;
    float* _RetVal;
  };


};