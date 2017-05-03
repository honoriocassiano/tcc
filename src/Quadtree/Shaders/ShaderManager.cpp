#include "ShaderManager.h"
#include <map>
#include <stdio.h>

using std::map;

bool    ShaderManager::initialized    = false;
GLuint  ShaderManager::programs[NONE] = { 0 };

void ShaderManager::ReportErrors(const GLuint handle, const bool isProgram)
{
  GLchar messageString[4096];
  GLsizei messageLength = 0;
  GLint status = GL_TRUE;
  if (isProgram)
  {
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    if (!status)
      printf("Shader program (%u) linking failed\n", handle);
    glGetProgramInfoLog(handle, sizeof(messageString), &messageLength, messageString);
  }
  else
  {
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if (!status)
      printf("Shader (%u) compilation failed\n", handle);
    glGetShaderInfoLog(handle, sizeof(messageString), &messageLength, messageString);
  }
  if (messageLength > 0)
    printf(messageString);
}

void ShaderManager::Initialize()
{
  if (initialized)
    return;
  initialized = true;

  string shaderFiles[NONE][2];
  map<string, string> shaderSources;
  #define SHADER_FILES(p, v, f) shaderFiles[p][0] = v; shaderFiles[p][1] = f;
  #include "GLSL/ShaderSources.txt"
  bool done = false;
  while (!done)
  {
    done = true;
    for (map<string, string>::iterator it = shaderSources.begin(); it != shaderSources.end(); it++)
    {
      const size_t pos0 = it->second.find("#pragma include ");
      if (pos0 == string::npos)
        continue;
      done = false;
      const size_t pos1 = pos0 + 16;
      const size_t pos2 = it->second.find_first_of(" \n\t", pos1);
      const string includedFilename = it->second.substr(pos1, pos2 - pos1);
      it->second.erase(pos0, pos2 - pos0);
      it->second.insert(pos0, shaderSources[includedFilename]);
    }
  }
  for (int i = 0; i < NONE; i++)
  {
    programs[i] = glCreateProgram();
    const GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    const GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char *vertStrings[1] = { shaderSources[shaderFiles[i][0]].c_str() };
    const char *fragStrings[1] = { shaderSources[shaderFiles[i][1]].c_str() };
    glShaderSource(vert, 1, vertStrings, 0);
    glShaderSource(frag, 1, fragStrings, 0);
    glCompileShader(vert);
    glCompileShader(frag);
    ReportErrors(vert);
    ReportErrors(frag);
    glAttachShader(programs[i], vert);
    glAttachShader(programs[i], frag);
    glLinkProgram(programs[i]);
    ReportErrors(programs[i], true);
  }
}

void ShaderManager::Deinitialize()
{
  for (int i = 0; i < NONE; i++)
  {
    GLsizei count;
    GLuint shaders[2];
    glGetAttachedShaders(programs[i], 2, &count, shaders);
    for (int j = 0; j < count; j++)
      glDeleteShader(shaders[j]);
    glDeleteProgram(programs[i]);
    programs[i] = 0;
  }
  initialized = false;
}

GLuint ShaderManager::GetShaderProgramHandle(const unsigned int program)
{
  return program < NONE ? programs[program] : 0;
}

void ShaderManager::UseShaderProgram(const unsigned int program)
{
  glUseProgram(GetShaderProgramHandle(program));
}
