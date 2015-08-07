/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The 

*/
/******************************************************************************/
#include "GraphicsGL.h"

#include "..\..\Debug\Debug.h"

#include "GLTutorial.h" // Tutorial for learning

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    GraphicsGL::GraphicsGL() : System(std::string("GraphicsGLSystem"), EnumeratedSystem::GraphicsGL) {
      
    }
    
    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    void GraphicsGL::Initialize() {
      std::cout << "GraphicsGL::Initialize" << std::endl;
      
      // Tutorial Code
      Tutorial::Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    void GraphicsGL::Update(float dt) {
      std::cout << "GraphicsGL::Update" << std::endl;
      // Tutorial code
      Tutorial::Update();
    }

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {
    }




  }


}
