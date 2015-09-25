/******************************************************************************/
/*!
\file   GLCameraTutorial.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  My study of the "Textures" tutorial:
"http://learnopengl.com/#!Getting-started/Textures"
A texture is a 2D (or 1D, 3D) image used to add detail to an object.

It also includes some transformations done with matrixes, with the
GLM library.
"http://learnopengl.com/#!Getting-started/Transformations"

*/
/******************************************************************************/
#pragma once
#include "GLTutorial.h"

// OpenGL Mathematics for doing transformations
#define GLM_FORCE_RADIANS // functions taking degrees is deprecated.
#include "..\..\Dependencies\include\GLM\glm.hpp"
#include "..\..\Dependencies\include\GLM\gtc\matrix_transform.hpp"
#include "..\..\Dependencies\include\GLM\gtc\type_ptr.hpp"
// SFML
#include "..\..\Dependencies\include\SFML\Window.hpp"

#include <vector>

namespace DCEngine {
  namespace Tutorial {

    class GLCameraTutorial : public GLTutorial {

    public:
      GLCameraTutorial(Entity& owner) : GLTutorial(owner) {}
      ~GLCameraTutorial() {};

      void Initialize();
      void OnLogicUpdate(Events::LogicUpdate* event);
      void Update();
      void Terminate();

      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);
      void GenerateMesh();
      void GenerateTexture(const std::string imagePath, GLuint& texture);
      void GenerateProjection();

      void UpdateViewMatrix();
      void ApplyTexture();
      void ApplyProjection();
      void ApplyTransformation(GLuint box);

      void OnKeyDown(Event*);
      void CameraInitialize();
      void CameraUpdate();


    private:
      sf::Window* _window; // A pointer to the sf::Window object for input polling
      const GLfloat screenWidth = 800;
      const GLfloat screenHeight = 600;
      int width, height;
      unsigned char* image;
      GLuint texture1, texture2;

      glm::vec4 vec; //!< Define a vec using GLM's built-in vector class
      glm::mat4 transform; //!< // Define a 'mat4', a 4-by-4 identity matrix by default
      glm::vec3 cameraPos, cameraFront, cameraUp, cameraTarget, cameraDirection;
      GLfloat cameraSpeed = 0.05f;

      //const glm::vec3* cubePositions;
      std::vector<glm::vec3> cubePositions;
      glm::mat4 model, view, projection;
      GLint modelLoc, viewLoc, projectionLoc;

      GLuint VBO, VAO, EBO;
      DCEngine::ShaderPtr shader;

    };

  }
}