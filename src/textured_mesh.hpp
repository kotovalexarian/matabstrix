#ifndef _TEXTURED_MESH_HPP_
#define _TEXTURED_MESH_HPP_

#include "mesh.hpp"

#include <vector>

#include <glm/glm.hpp>

class TexturedMesh:
  public Mesh
{
public:
  std::vector<glm::vec2> tex_coords;
};

#endif // _TEXTURED_MESH_HPP_
