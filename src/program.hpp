#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "resource.hpp"
#include "gl.hpp"

#include <string>

class Program:
  public Resource
{
  RESOURCE(Program)

public:
  void use() const;
  GLuint get_uniform_location(const GLchar *name) const;

private:
  GLuint _id;
};

#endif // _PROGRAM_HPP_
