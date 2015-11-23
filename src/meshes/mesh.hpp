#ifndef _MESH_HPP_
#define _MESH_HPP_

#include "../gl.hpp"

#include <array>
#include <vector>

#include <glm/glm.hpp>

struct Mesh
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;

  std::vector<GLushort> elements;

  void add(const glm::vec3 &position, const glm::vec3 &normal);

  void add3(const std::array<glm::vec3, 3> &positions, const glm::vec3 &normal);
  void add3_with_auto_normals(const std::array<glm::vec3, 3> &positions, const glm::vec3 &origin = glm::vec3(0.0f, 0.0f, 0.0f));
};

#endif // _MESH_HPP_
