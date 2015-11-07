#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "texture.hpp"

class Material
{
public:
  Material(const char *filename);
  void use() const;

private:
  Texture *_texture;
};

#endif // _MATERIAL_HPP_
