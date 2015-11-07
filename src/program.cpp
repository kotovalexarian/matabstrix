#include "program.hpp"

Program::Program()
{
  _id = glCreateProgram();
}

void Program::attach_shader(const Shader &shader)
{
  glAttachShader(_id, shader.id());
}

void Program::bind_attrib_location(const GLuint index, const GLchar *const name)
{
  glBindAttribLocation(_id, index, name);
}

void Program::link()
{
  glLinkProgram(_id);
}

void Program::use() const
{
  glUseProgram(_id);
}

GLuint Program::get_uniform_location(const GLchar *name) const
{
  return glGetUniformLocation(_id, name);
}
