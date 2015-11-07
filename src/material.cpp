#include "material.hpp"

#include <string>
#include <fstream>

Material::Material(const char *const filename)
{
  std::ifstream file(std::string("/data/materials/") + filename, std::ios::in);

  std::string line;
  while (std::getline(file, line))
  {
    if (line.substr(0, 7) == "map_Kd ")
    {
      _texture = new Texture(line.substr(7).c_str());
    }
  }
}

void Material::use() const
{
  _texture->use();
}
