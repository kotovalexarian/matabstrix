#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "gl.hpp"
#include "resource.hpp"

#include <string>

struct Shader:
  Resource
{
  RESOURCE(Shader)

  inline GLuint id() const { return _id; };

private:
  GLuint _id;
};

#endif // _SHADER_HPP_
