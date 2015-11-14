#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "../gl.hpp"

#include <vector>

#include <glm/glm.hpp>

struct Mesh
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;

  std::vector<GLushort> elements;

  void add(const glm::vec3 &position, const glm::vec3 &normal);
};

#endif // _MESH_HPP_
