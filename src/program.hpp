#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "gl.hpp"

#include <string>

struct Program
{
  Program(const std::string &name);

  void bind_attrib_location(GLuint index, const GLchar *name);
  void bind_attribs(GLuint count, const GLchar *const names[]);

  void link();

  void use() const;
  GLuint get_uniform_location(const GLchar *name) const;

private:
  static const std::string filename(const std::string &name);

  GLuint _id;
};

#endif // _PROGRAM_HPP_
