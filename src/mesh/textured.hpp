#ifndef _MESH_TEXTURED_HPP_
#define _MESH_TEXTURED_HPP_

#include "mesh.hpp"

class TexturedMesh:
  public Mesh
{
public:
  std::vector<glm::vec2> tex_coords;

  void add(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &tex_coord);
};

#endif // _MESH_TEXTURED_HPP_
