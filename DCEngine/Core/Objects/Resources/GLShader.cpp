/******************************************************************************/
/*!
\file   GLShader.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/13/2015
\brief  An encapsulated object that reads shaders from disk, compiles, links them,
checks for errors and other useful functions.
*/
/******************************************************************************/
#include "GLShader.h"

#include <fstream>
#include <sstream>

namespace DCEngine {

  GLShader::GLShader(std::string vertexPath, std::string fragmentPath) : Resource("Shader") {
    trace << "GLShader::GLShader() \n";
    // Loads the shaders
    Load(vertexPath, fragmentPath);
    // Compiles the shader
    Compile();
  }

  /**************************************************************************/
  /*!
  \brief   Loads the shaders from their source files, then stores them.
  \param   The path of the vertex shader.
  \param   The path of the fragment shader.
  */
  /**************************************************************************/
  void GLShader::Load(std::string vertexPath, std::string fragmentPath) {
    if (TRACE_ON)
      trace << "GLShader::Load - Vertex: " << vertexPath << " , Fragment: " << fragmentPath << "\n";
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    // Ensures ifstream objects can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::badbit);

    try {
      // Open the files ( YES I KNOW THIS WAY IS DUMB PLZ HELP )
      std::string shaderLocation("Tutorials/OpenGL/");
      vertexShaderFile.open(shaderLocation + vertexPath);
      fragmentShaderFile.open(shaderLocation + fragmentPath);
      // Use C++ filestreams to read the content from the file
      std::stringstream vertexShaderStream, fragmentShaderStream;
      vertexShaderStream << vertexShaderFile.rdbuf();
      fragmentShaderStream << fragmentShaderFile.rdbuf();
      // Close the files
      vertexShaderFile.close();
      fragmentShaderFile.close();
      // Store the shader code within the object   
      vertexCode = vertexShaderStream.str();
      fragmentCode = fragmentShaderStream.str();    
    }
    catch (std::ifstream::failure e) {
      if (TRACE_ON)
        trace << "GLShader::LoadShaders failed! \n";
    }    
  }

  /**************************************************************************/
  /*!
  \brief   Creates the shaders from their sources, then links them.
  */
  /**************************************************************************/
  void GLShader::Compile() {
    if (TRACE_ON)
      trace << "GLShader::Compile \n";
    GLuint vertex, fragment;   
    
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vShaderCode = vertexCode.c_str();
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    AssertShaderCompilation(vertex, "Vertex Shader");
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fShaderCode = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    AssertShaderCompilation(fragment, "Fragment Shader");
    // Link shader program
    this->_shaderProgram = glCreateProgram();
    glAttachShader(this->_shaderProgram, vertex);
    glAttachShader(this->_shaderProgram, fragment);
    glLinkProgram(this->_shaderProgram);
    AssertShaderProgramLinking(this->_shaderProgram);
    // Delete the shaders as they're now linked into the program and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

  }

  /**************************************************************************/
  /*!
  \brief   Activates the shader program.
  */
  /**************************************************************************/
  void GLShader::Use() {
    if (TRACE_ON && TRACE_UPDATE)
      trace << "GLShader::Use \n";

    glUseProgram(this->_shaderProgram);
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the shader was compiled successfully.
  \param   The shader.
  \param   The name of the shader.
  */
  /**************************************************************************/
  void GLShader::AssertShaderCompilation(GLuint shader, std::string shaderName) {
    GLint success;       // Define an integer to indicate success
    GLchar infoLog[512]; // Container for the error messages (if any)

                         // Check if compilation was successful.
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success) {
      trace << "GLTutorial::AssertShaderCompilation - " << shaderName << " compiled! \n";
    }
    else {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      trace << "GLTutorial::AssertShaderCompilation - " << shaderName << " failed to compile! \n"
        << infoLog << "\n";
    }
  }

  /**************************************************************************/
  /*!
  \brief  Checks if the shader program linked successfully.
  \param   The shader program.
  */
  /**************************************************************************/
  void GLShader::AssertShaderProgramLinking(GLuint shaderProgram) {
    GLint success;       // Define an integer to indicate success
    GLchar infoLog[512]; // Container for the error messages (if any)

                         // Check if compilation was successful.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (success) {
      trace << "GLTutorial::AssertShaderCompilation - Shader program linked! \n";
    }
    else {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      trace << "GLTutorial::AssertShaderCompilation - Shader program failed to link! \n"
        << infoLog << "\n";
    }
  }


}