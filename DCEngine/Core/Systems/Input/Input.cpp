#include "Input.h"

#include "..\..\Debug\Debug.h" // trace
#include "..\..\Engine\Engine.h" 

// SFML implementation
#if(USE_SFML)
#include "InputSFML.h"
std::unique_ptr<DCEngine::Systems::InputSFML> InputHandler;
// GLFW implementation
#else
#include "InputGLFW.h"
std::unique_ptr<DCEngine::Systems::InputGLFW> InputHandler;
#endif

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;

  namespace Systems {
    Input::Input() : System(std::string("InputSystem"), EnumeratedSystem::Input) {      
      #if(USE_SFML)
      InputHandler.reset(new InputSFML());
      #else
      InputHandler.reset(new InputGLFW());
      #endif
      
    }

    void Input::Initialize() {
      trace << "Input::Initialize \n";
      InputHandler->Initialize();
    }

    void Input::Update(float dt) {
      trace << "Input::Update \n";
      InputHandler->Update(dt);
    }

    void Input::Terminate() {
      trace << "Input::Terminate \n";
      InputHandler->Terminate();
    }

  }


}
