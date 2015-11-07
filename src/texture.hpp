#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "gl.hpp"

class Texture
{
public:
  Texture(const char *filename);
  void use() const;

private:
  GLuint _id;
};

#endif // _TEXTURE_HPP_
