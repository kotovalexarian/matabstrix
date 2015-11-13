#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "resource.hpp"
#include "gl.hpp"

#include <string>

struct Texture:
  Resource
{
  RESOURCE(Texture)

  void use() const;

private:
  GLuint _id;
};

#endif // _TEXTURE_HPP_
