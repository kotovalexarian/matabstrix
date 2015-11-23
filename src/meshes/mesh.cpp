#include "mesh.hpp"

#include <glm/gtx/vector_angle.hpp>

void Mesh::add(const glm::vec3 &position, const glm::vec3 &normal)
{
  elements.push_back(positions.size());

  positions.push_back(position);
  normals.push_back(normal);
}

void Mesh::add3(const std::array<glm::vec3, 3> &positions, const glm::vec3 &normal)
{
  for (int i = 0; i < 3; ++i)
    add(positions[i], normal);
}

void Mesh::add3_with_auto_normals(const std::array<glm::vec3, 3> &positions, const glm::vec3 &origin)
{
  const glm::vec3 sample = glm::normalize((positions[1] - positions[0]) * (positions[2] - positions[0]));
  // TODO Handle if direction is zero vector (origin is in vertex 0) - use some other vertex
  const glm::vec3 direction = glm::normalize(positions[0] - origin);
  const float angle = glm::angle(direction, sample);
  const glm::vec3 normal = angle <= glm::radians(90.0f) ? sample : -sample;

  add3(positions, normal);
}
