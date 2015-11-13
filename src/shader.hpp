#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "gl.hpp"

struct Shader
{
  Shader(GLenum type, const char *filename);
  inline GLuint id() const { return _id; };

private:
  GLuint _id;
};

#endif // _SHADER_HPP_
