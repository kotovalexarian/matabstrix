#include "executable.hpp"

Executable::Executable()
{
  id = glCreateProgram();
}

void Executable::attach_shader(const Shader &shader)
{
  glAttachShader(id, shader.id());
}

void Executable::bind_attrib_location(GLuint index, const GLchar *name)
{
  glBindAttribLocation(id, index, name);
}

void Executable::link()
{
  glLinkProgram(id);
}

void Executable::use() const
{
  glUseProgram(id);
}

GLuint Executable::get_uniform_location(const GLchar *name) const
{
  return glGetUniformLocation(id, name);
}
