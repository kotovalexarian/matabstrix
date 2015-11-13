#include "program.hpp"

#include "shader.hpp"

#include <fstream>

const std::string Program::filename(const std::string &name)
{
  return "/data/shaders/" + name;
}

Program::Program(const std::string &name)
{
  const std::string path = filename(name) + '/';

  const Shader vertex_shader = Shader(GL_VERTEX_SHADER, (path + "vertex.glsl").c_str());
  const Shader fragment_shader = Shader(GL_FRAGMENT_SHADER, (path + "fragment.glsl").c_str());

  _id = glCreateProgram();

  glAttachShader(_id, vertex_shader.id());
  glAttachShader(_id, fragment_shader.id());
}

void Program::bind_attrib_location(GLuint index, const GLchar *name)
{
  glBindAttribLocation(_id, index, name);
}

const Program *Program::build(GLuint count, const GLchar *const names[])
{
  for (GLuint index = 0; index < count; ++index)
    bind_attrib_location(index, names[index]);

  link();

  return this;
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
