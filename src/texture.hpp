#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "gl.hpp"

#include <string>

class Texture
{
public:
  Texture(const std::string &name);
  void use() const;

private:
  GLuint _id;
};

#endif // _TEXTURE_HPP_
