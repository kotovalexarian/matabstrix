#include "program.hpp"

#include "shader.hpp"

#include <fstream>

const std::string Program::filename(const std::string &name)
{
  return "/data/shaders/" + name;
}

Program::Program(const std::string &name):
  vertex_shader(name + ".vert"),
  fragment_shader(name + ".frag")
{}

const Executable *Program::build(
  GLuint attrib_count, const GLchar *const attribs[],
  unsigned uniform_count, const GLchar *const uniforms[])
{
  Executable *exe = new Executable();

  exe->attach_shader(vertex_shader);
  exe->attach_shader(fragment_shader);

  for (GLuint attrib = 0; attrib < attrib_count; ++attrib)
    exe->bind_attrib_location(attrib, attribs[attrib]);

  exe->link();

  exe->get_uniforms(uniform_count, uniforms);

  return exe;
}
