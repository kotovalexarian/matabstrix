#ifndef _MTLLIB_HPP_
#define _MTLLIB_HPP_

#include "material.hpp"

#include <string>
#include <map>

class Mtllib
{
public:
  Mtllib(const std::string &name);

  std::map<std::string, Material*> materials;
};

#endif // _MTLLIB_HPP_
