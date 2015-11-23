/******************************************************************************/
/*!
@file   WindowsSFML.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/12/2015
@brief  Handling the window created by SFML functions.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include <Windows.h>

#include "WindowSFML.h"
#include "Window.h"
#include "..\..\Engine\Engine.h"
#include <sstream>


namespace DCEngine {
  namespace Systems {


	  float localCounter = 0;
	  int frameCounter = 0;
    /**************************************************************************/
    /*!
    \brief  Constructor for the WindowSFML class.
    */
    /**************************************************************************/
    WindowSFML::WindowSFML(Window& windowInterface) : WindowInterface(windowInterface) {
    }

    WindowSFML::~WindowSFML() {
    }

    sf::Vector2i WindowSFML::getWindowDimensions()
    {
      auto windowSize = WindowContext->getSize();
      return sf::Vector2i(windowSize.x, windowSize.y);
    }

    /**************************************************************************/
    /*!
    @brief  Switches back and forth between FullScreen and Windowed mode.
    @param  The mode of the window. FullScreen or Windowed.
    */
    /**************************************************************************/
    void WindowSFML::setFullScreen()
    {      
      if (Mode != WindowMode::Fullscreen) {
        setWindow(WindowMode::Fullscreen);
        Mode = WindowMode::Fullscreen;
      }        
      else {
        setWindow(WindowMode::Default);
        Mode = WindowMode::Default;
      }      
    }

    /**************************************************************************/
    /*!
    @brief  Sets the window context.
    @param  The mode of the window. FullScreen or Windowed.
    @todo   Don't directly call the Graphics system by friending it. Instead
            send an event to replace it.
			2) Line 92, 93 reload VAO after creating a new window(fullscreen->window, window->fullscreen) 
    */
    /**************************************************************************/
    void WindowSFML::setWindow(WindowMode style)
    {
      // Create the window
      //if (WindowContext != nullptr)
      //  WindowContext->close();

      // Save the current OpenGL state
      Daisy->getSystem<Graphics>()->BackupState();

      // This is stupid, but I can't pass in the sf::Style enum as a param :(
      switch (style) {
      case WindowMode::Default:
		  WindowInterface.Width = widthRecord;
		  WindowInterface.Height = heightRecord;
        WindowContext->create(sf::VideoMode(WindowInterface.Width, WindowInterface.Height),
                              WindowInterface.Caption, sf::Style::Default, ContextSettings);
        break;
      case WindowMode::Fullscreen :
		  widthRecord = WindowInterface.Width;
		  heightRecord = WindowInterface.Height;
		  WindowInterface.Width = sf::VideoMode::getDesktopMode().width;
		  WindowInterface.Height = sf::VideoMode::getDesktopMode().height;
        WindowContext->create(sf::VideoMode(WindowInterface.Width, WindowInterface.Height),
                              WindowInterface.Caption, sf::Style::Fullscreen, ContextSettings);
        break;
      }
      // After this call, the application will run at the same frequency as the monitor's refresh rate
      //WindowContext->setVerticalSyncEnabled(true);
      // In some situations you want the application to run at a given framerate. (!) Do not mix with setVSync
      WindowContext->setFramerateLimit(WindowInterface.Framerate);

      // Restore the previous OpenGL state
      Daisy->getSystem<Graphics>()->RestoreState();
	  Daisy->getSystem<GUI>()->ReloadVAO();

      // Reload textures
      //Daisy->getSystem<Content>()->LoadTextures();

      // The window context needs to give the input system a reference to this pointer
    }

    /**************************************************************************/
    /*!
    \brief  Initializes SFML, configuring the window before creating it.
    */
    /**************************************************************************/
    void WindowSFML::Initialize() {    
      
      // Stores the settings for the underlying SFML window context
      ContextSettings.depthBits = _depthBits;
      ContextSettings.stencilBits = _stencilBits;
      ContextSettings.antialiasingLevel = _antiAliasingLevel;
      ContextSettings.majorVersion = _majorVersion;
      ContextSettings.minorVersion = _minorVersion;

      WindowContext.reset(new sf::Window(sf::VideoMode(WindowInterface.Width, WindowInterface.Height),
                          WindowInterface.Caption, sf::Style::Default, ContextSettings));
      WindowContext->setFramerateLimit(WindowInterface.Framerate);

      // Configures the window context, then creates it
      //setWindow(WindowMode::Default);

      // Assert that the window was created
      if (WindowContext == nullptr) {
        DCTrace << "Failed to create SFML window \n";
        // Terminate??
      }
    }

    /**************************************************************************/
    /*!
    \brief  Updates the window.
    */
    /**************************************************************************/
    void WindowSFML::Update(float dt) {

      //auto currentTime = Clock.restart().asSeconds();
      //float fps = 1.f / (currentTime - LastTime);
      //LastTime = currentTime;
		localCounter += dt;
		++frameCounter;
		if (localCounter > 0.5)
		{
			std::stringstream ss;
			ss << WindowInterface.Caption << "              [fps=" << int(frameCounter / localCounter) << "]";
			WindowContext->setTitle(ss.str());
			windowsTitle = ss.str();
			localCounter = 0;
			frameCounter = 0;
		}

      //DCTrace << "WindowSFML::Update - FPS: " << dt << "\n";

      // Checks at the start of loop iteration if SFML has been instructed
      // to close, and if so tell the engine to stop running.
		if (EventObj.type == sf::Event::Closed)
		{
			Terminate();
		}
    }

    /**************************************************************************/
    /*!
    \brief  Starts the current frame.
    */
    /**************************************************************************/
    void WindowSFML::StartFrame() {
      
    }

    /**************************************************************************/
    /*!
    \brief Displays on the screen what has been rendered to the window so far,
           typically called after all OpenGL rendering calls have been done
           for the current frame.     
    */
    /**************************************************************************/
    void WindowSFML::EndFrame() {    
      WindowContext->display();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void WindowSFML::Terminate() {
      WindowContext->close();
      Daisy->Stop();
    }
  
  }


  


}
