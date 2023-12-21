#ifndef ASTEROID_SHADER_H
#define ASTEROID_SHADER_H

#include <vector>

#include "graphics_headers.h"

class AsteroidShader
{
public:
    AsteroidShader();
    ~AsteroidShader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    GLint GetAttribLocation(const char* pAttribName);


private:
    GLuint m_shaderProg;
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
#pragma once
