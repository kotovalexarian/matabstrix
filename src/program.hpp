#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "executable.hpp"

#include <string>

struct Program
{
  Program(const std::string &name);

  const Executable *build(
    GLuint attrib_count, const GLchar *const attribs[],
    unsigned uniform_count, const GLchar *const uniforms[]);

private:
  static const std::string filename(const std::string &name);

  const Shader vertex_shader;
  const Shader fragment_shader;
};

#endif // _PROGRAM_HPP_
