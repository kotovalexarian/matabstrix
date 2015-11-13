#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "gl.hpp"

#include <string>

struct Shader
{
  Shader(GLenum type, const std::string &filename);
  inline GLuint id() const { return _id; };

private:
  GLuint _id;
};

#endif // _SHADER_HPP_
