#ifndef _MTLLIB_HPP_
#define _MTLLIB_HPP_

#include "resource.hpp"
#include "material.hpp"

#include <string>
#include <map>

class Mtllib:
  public Resource
{
  RESOURCE(Mtllib)

public:
  std::map<std::string, const Material*> materials;
};

#endif // _MTLLIB_HPP_
