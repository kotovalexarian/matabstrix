#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "gl.hpp"

#include <string>

struct Shader
{
  Shader(const std::string &filename);
  inline GLuint id() const { return _id; };

private:
  static const std::string filename(const std::string &name);

  GLuint _id;
};

#endif // _SHADER_HPP_
