/*  glutils %{Cpp:License:ClassName} - Yann BOUCHER (yann) 24/08/2016
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/
#ifndef GLUTILS_HPP
#define GLUTILS_HPP

#include <string>

#include <SFML/OpenGL.hpp>
#include <SFML/System/Err.hpp>

#ifdef SFML_DEBUG

    // In debug mode, perform a test on every OpenGL call
    // The do-while loop is needed so that glCheck can be used as a single statement in if/else branches
    #define glCheck(expr) do { expr; Rayfun::glCheckError(__FILE__, __LINE__, #expr); } while (false)

#else

    // Else, we don't add any overhead
    #define glCheck(expr) (expr)

#endif

namespace Rayfun
{

inline void glCheckError(const char* file, unsigned int line, const char* expression)
{
    // Get the last error
    GLenum errorCode = glGetError();

    if (errorCode != GL_NO_ERROR)
    {
        std::string fileString = file;
        std::string error = "Unknown error";
        std::string description  = "No description";

        // Decode the error code
        switch (errorCode)
        {
            case GL_INVALID_ENUM:
            {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE:
            {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION:
            {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in the current state.";
                break;
            }

            case GL_STACK_OVERFLOW:
            {
                error = "GL_STACK_OVERFLOW";
                description = "This command would cause a stack overflow.";
                break;
            }

            case GL_STACK_UNDERFLOW:
            {
                error = "GL_STACK_UNDERFLOW";
                description = "This command would cause a stack underflow.";
                break;
            }

            case GL_OUT_OF_MEMORY:
            {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute the command.";
                break;
            }
        }

        // Log the error
        sf::err() << "An internal OpenGL call failed in "
              << fileString.substr(fileString.find_last_of("\\/") + 1) << "(" << line << ")."
              << "\nExpression:\n   " << expression
              << "\nError description:\n   " << error << "\n   " << description << "\n"
              << std::endl;
    }
}
}

#endif // GLUTILS_HPP
