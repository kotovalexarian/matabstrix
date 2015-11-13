#ifndef _EXECUTABLE_HPP_
#define _EXECUTABLE_HPP_

#include "gl.hpp"
#include "shader.hpp"

struct Executable
{
  void use() const;

  GLuint get_uniform_location(const GLchar *name) const;

private:
  friend struct Program;

  Executable();
  void attach_shader(const Shader &shader);
  void bind_attrib_location(GLuint index, const GLchar *name);
  void link();

  GLuint id;
};

#endif // _EXECUTABLE_HPP_
