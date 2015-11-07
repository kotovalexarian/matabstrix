#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "gl.hpp"
#include "shader.hpp"

class Program
{
public:
  Program();
  void attach_shader(const Shader &shader);
  void bind_attrib_location(GLuint index, const GLchar *name);
  void link();
  void use() const;
  GLuint get_uniform_location(const GLchar *name) const;

private:
  GLuint _id;
};

#endif // _PROGRAM_HPP_
