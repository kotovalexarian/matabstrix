#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "../gl.hpp"

#include <vector>

#include <glm/glm.hpp>

class Mesh
{
public:
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;

  std::vector<GLushort> elements;
};

#endif // _MESH_HPP_
