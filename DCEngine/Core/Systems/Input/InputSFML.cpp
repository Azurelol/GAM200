#include <Windows.h>
#include "InputSFML.h"

// Engine
#include "..\..\Engine\Engine.h"
// Libraries
#include "..\Window\WindowSFML.h"
#include <SFML\System\Vector2.hpp>
// GUI Handling
#include "../GUI/ImGuiSFML.h"

namespace DCEngine {

  // Need access to the engine to get a pointer to the GLFWwindow object
  extern std::unique_ptr<Engine> Daisy;

  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Constructor for the InputSFML class.
    */
    /**************************************************************************/
    InputSFML::InputSFML() {
    }

    InputSFML::~InputSFML() {
    }

    /**************************************************************************/
    /*!
    \brief  Initialize.
    */
    /**************************************************************************/
    void InputSFML::Initialize() {

     #if(USE_SFML)      
      WindowContext = Daisy->getSystem<Window>()->WindowHandler->WindowContext.get();
     #endif

      // Should key presses repeat?
      WindowContext->setKeyRepeatEnabled(false);

      // Store the reference to the mouse and keyboard interface objects
      KeyboardRef = Daisy->getKeyboard();
      MouseRef = Daisy->getMouse();

    }

    /**************************************************************************/
    /*!
    \brief  Update.
    */
    /**************************************************************************/
    void InputSFML::Update(float dt) {
      PollEvents();    
    }

    /**************************************************************************/
    /*!
    @brief  Polls for all input events..
    @todo   Currently intruded upon by ImGui input code.
    */
    /**************************************************************************/
    void InputSFML::PollEvents() {
      
      // Check all window's events that were triggered since the last iteration
      if (WindowContext->pollEvent(_event) == false)
        return;

      // Poll for events
      switch (_event.type) {
      case sf::Event::KeyPressed:
        //ImGuiPollKeyPressed(_event);
        PollKeyPressed(_event);
        break;
      case sf::Event::KeyReleased:
        //ImGuiPollKeyReleased(_event);
        PollKeyReleased(_event);
        break;
      case sf::Event::MouseButtonPressed:
        Daisy->getSystem<GUI>()->GUIHandler->MousePressed[_event.mouseButton.button] = true;
        PollMouseButtonPressed(_event);
        break;
      case sf::Event::MouseButtonReleased:
        Daisy->getSystem<GUI>()->GUIHandler->MousePressed[_event.mouseButton.button] = false;
        PollMouseButtonReleased(_event);
        break;
      case sf::Event::MouseWheelMoved:
        PollMouseWheelMoved(_event);
        //ImGuiIO& io = ImGui::GetIO();
        //io.MouseWheel += (float)_event.mouseWheel.delta;
        break;
      case sf::Event::TextEntered:
        PollTextEntered(_event);
        break;
		
    // KEYS: alt+tab, ctrl+alt+delete
	  case sf::Event::LostFocus:
		  if (Daisy->getSystem<Window>()->WindowHandler->Mode == WindowMode::Fullscreen)
		  {
			  ShowWindow(WindowContext->getSystemHandle(), SW_MINIMIZE);
		  }
		  break;
	  case sf::Event::GainedFocus:
		  ShowWindow(WindowContext->getSystemHandle(), SW_RESTORE);
		  break;


        // Don't process other events
      default:
      // @todo WHAT??
		  if ((((unsigned short)GetKeyState(VK_CONTROL)) >> 15) && (((unsigned short)GetKeyState(VK_MENU) >> 15)) && (((unsigned short)GetKeyState(VK_MENU) >> 15)))
		  {
			  ShowWindow(WindowContext->getSystemHandle(), SW_MINIMIZE);
		  }
        break;
      }
    }
    
  /**************************************************************************/
  /*!
  \brief  Polls for text events.
  */
  /**************************************************************************/
  void InputSFML::PollTextEntered(sf::Event & event)
  {
    // Update ImGui
    if (event.text.unicode > 0 && event.text.unicode < 0x10000)
      ImGui::GetIO().AddInputCharacter(event.text.unicode);  
  }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void InputSFML::Terminate() {
    }
  }
}
