#include "shader.hpp"

#include <cstdlib>
#include <cstdio>

const std::string Shader::filename(const std::string &name)
{
  return "/shaders/" + name;
}

Shader::Shader(const Adapter &adapter, const std::string &name)
{
  const GLuint type = name.substr(name.size() - 5) == ".vert" ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;

  FILE *file = fopen(adapter.filename<Shader>(name).c_str(), "r");

  fseek(file, 0, SEEK_END);
  const long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *source = (char*)malloc(size + 1);
  fread(source, size, 1, file);
  source[size] = 0;

  fclose(file);

  _id = glCreateShader(type);
  glShaderSource(_id, 1, (const GLchar**)&source, nullptr);
  glCompileShader(_id);

  free(source);
}
