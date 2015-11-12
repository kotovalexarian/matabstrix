#ifndef _MODEL_RAW_HPP_
#define _MODEL_RAW_HPP_

#include "model.hpp"
#include "../mesh/mesh.hpp"

namespace Models
{
  class Raw:
    public Model,
    protected Mesh
  {
    RESOURCE(Raw)

  public:
    void draw() const;

  private:
    GLuint positions_id;
    GLuint normals_id;

    GLuint id;
  };
};

#endif // _MODEL_RAW_HPP_
