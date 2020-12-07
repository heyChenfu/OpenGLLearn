#include <iostream>
#include <glad\glad.h>

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string sError = "";
        char buffer[10];
        sprintf_s(buffer, "%d", errorCode);
        sError = buffer;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  sError = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 sError = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             sError = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:                 sError = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: sError = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << sError << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 