#ifndef _MESH_TEXTURED_HPP_
#define _MESH_TEXTURED_HPP_

#include "mesh.hpp"

class TexturedMesh:
  public Mesh
{
public:
  std::vector<glm::vec2> tex_coords;
};

#endif // _MESH_TEXTURED_HPP_
