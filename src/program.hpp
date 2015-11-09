#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "gl.hpp"

class Program
{
public:
  Program(const char *name);
  void use() const;
  GLuint get_uniform_location(const GLchar *name) const;

private:
  GLuint _id;
};

#endif // _PROGRAM_HPP_
