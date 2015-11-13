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

void Executable::get_uniforms(unsigned count, const GLchar *const names[])
{
  uniforms.resize(count);

  for (unsigned index = 0; index < count; ++index)
    uniforms[index] = glGetUniformLocation(id, names[index]);
}
