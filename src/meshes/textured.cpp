#include "textured.hpp"

void TexturedMesh::add(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &tex_coord)
{
  Mesh::add(position, normal);

  tex_coords.push_back(tex_coord);
}
