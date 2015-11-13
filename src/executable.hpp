#ifndef _EXECUTABLE_HPP_
#define _EXECUTABLE_HPP_

#include "gl.hpp"
#include "shader.hpp"

#include <vector>

struct Executable
{
  void use() const;

  inline GLuint uniform(unsigned index) const;

private:
  friend struct Program;

  Executable();
  void attach_shader(const Shader &shader);
  void bind_attrib_location(GLuint index, const GLchar *name);
  void link();

  void get_uniforms(unsigned count, const GLchar *const names[]);

  GLuint id;

  std::vector<GLuint> uniforms;
};

GLuint Executable::uniform(unsigned index) const
{
  return uniforms.at(index);
}

#endif // _EXECUTABLE_HPP_
