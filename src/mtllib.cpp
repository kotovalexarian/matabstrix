#include "mtllib.hpp"

#include <string>
#include <fstream>

Mtllib::Mtllib(const std::string &name)
{
  std::ifstream file("/data/materials/" + name, std::ios::in);

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
      material->texture = new Texture(line.substr(7).c_str());
    }
  }
}
