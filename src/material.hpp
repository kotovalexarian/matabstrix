#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "texture.hpp"

class Material
{
public:
  inline void use() const;

  const Texture *texture;
};

void Material::use() const
{
  texture->use();
};

#endif // _MATERIAL_HPP_
