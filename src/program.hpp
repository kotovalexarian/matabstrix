#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "resource.hpp"
#include "shader.hpp"
#include "executable.hpp"

#include <string>

struct Program:
  Resource
{
  RESOURCE(Program)

  const Executable *build(
    GLuint attrib_count, const GLchar *const attribs[],
    unsigned uniform_count, const GLchar *const uniforms[]) const;

private:
  const Shader *vertex_shader;
  const Shader *fragment_shader;
};

#endif // _PROGRAM_HPP_
