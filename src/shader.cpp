#include "shader.hpp"

#include <cstdlib>
#include <cstdio>

Shader::Shader(const GLenum type, const std::string &filename)
{
  FILE *file = fopen(filename.c_str(), "r");

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
