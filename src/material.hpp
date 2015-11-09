#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "texture.hpp"

#include <string>

class Material
{
public:
  Material(const std::string &name);
  void use() const;

private:
  Texture *_texture;
};

#endif // _MATERIAL_HPP_
