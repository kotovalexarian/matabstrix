#ifndef _MTLLIB_HPP_
#define _MTLLIB_HPP_

#include "resource.hpp"
#include "material.hpp"

#include <string>
#include <map>

struct Mtllib:
  Resource
{
  RESOURCE(Mtllib)

  const Material *operator[](const std::string &name) const;

private:
  std::map<std::string, const Material*> _materials;
};

#endif // _MTLLIB_HPP_
