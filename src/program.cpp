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

  std::ifstream file(path + "attrib.txt", std::ios::in);

  std::string line;
  int index = 0;

  while (std::getline(file, line))
    glBindAttribLocation(_id, index++, line.c_str());

  for (int i = 0; i < index; ++i)
    glEnableVertexAttribArray(i);
}

void Program::use() const
{
  glUseProgram(_id);
}

void Program::link()
{
  glLinkProgram(_id);
}

GLuint Program::get_uniform_location(const GLchar *name) const
{
  return glGetUniformLocation(_id, name);
}
