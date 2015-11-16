#include "mtllib.hpp"

#include <string>
#include <fstream>

const std::string Mtllib::filename(const std::string &name)
{
  return "/materials/" + name;
}

Mtllib::Mtllib(const Adapter &adapter, const std::string &name)
{
  std::ifstream file(adapter.filename<Mtllib>(name), std::ios::in);

  Material *material = nullptr;

  std::string line;
  while (std::getline(file, line))
  {
    if (!material && line.substr(0, 7) != "newmtl ")
      continue;

    if (line.substr(0, 7) == "newmtl ")
    {
      material = new Material();
      _materials[line.substr(7)] = material;
    }
    else
    if (line.substr(0, 7) == "map_Kd ")
    {
      material->texture = adapter.load<Texture>(line.substr(7));
    }
  }
}

const Material *Mtllib::operator[](const std::string &name) const
{
  return ((Mtllib*)this)->_materials[name];
}
