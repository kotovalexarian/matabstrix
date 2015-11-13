#include "program.hpp"

#include "shader.hpp"

#include <fstream>

const std::string Program::filename(const std::string &name)
{
  return "/data/shaders/" + name;
}

Program::Program(const std::string &name):
  vertex_shader(GL_VERTEX_SHADER, filename(name) + "/vertex.glsl"),
  fragment_shader(GL_FRAGMENT_SHADER, filename(name) + "/fragment.glsl")
{}

const Executable *Program::build(GLuint count, const GLchar *const names[])
{
  Executable *exe = new Executable();

  exe->attach_shader(vertex_shader);
  exe->attach_shader(fragment_shader);

  for (GLuint index = 0; index < count; ++index)
    exe->bind_attrib_location(index, names[index]);

  exe->link();

  return exe;
}
