#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "transformation.hpp"

class Camera :
  public Transformation
{
public:
  glm::mat4 transformation() const;

  glm::mat4 projection;
};

#endif // _CAMERA_HPP_
