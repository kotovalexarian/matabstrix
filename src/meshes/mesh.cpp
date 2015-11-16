#include "mesh.hpp"

void Mesh::add(const glm::vec3 &position, const glm::vec3 &normal)
{
  elements.push_back(positions.size());

  positions.push_back(position);
  normals.push_back(normal);
}
