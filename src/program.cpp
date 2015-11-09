#include "program.hpp"

#include "shader.hpp"

#include <string>
#include <fstream>

Program::Program(const char *const name)
{
  const std::string path = std::string("/data/shaders/") + name + '/';

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

  glLinkProgram(_id);

  for (int i = 0; i < index; ++i)
    glEnableVertexAttribArray(i);
}

void Program::use() const
{
  glUseProgram(_id);
}

GLuint Program::get_uniform_location(const GLchar *name) const
{
  return glGetUniformLocation(_id, name);
}
