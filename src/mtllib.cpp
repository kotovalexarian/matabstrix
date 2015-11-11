#include "mtllib.hpp"

#include <string>
#include <fstream>

const std::string Mtllib::filename(const std::string &name)
{
  return "/data/materials/" + name;
}

Mtllib::Mtllib(const std::string &name)
{
  std::ifstream file(filename(name), std::ios::in);

  Material *material = nullptr;

  std::string line;
  while (std::getline(file, line))
  {
    if (!material && line.substr(0, 7) != "newmtl ")
      continue;

    if (line.substr(0, 7) == "newmtl ")
    {
      material = new Material();
      materials[line.substr(7)] = material;
    }
    else
    if (line.substr(0, 7) == "map_Kd ")
    {
      material->texture = Store().load<Texture>(line.substr(7));
    }
  }
}
