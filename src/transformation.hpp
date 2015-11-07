#ifndef _TRANSFORMATION_HPP_
#define _TRANSFORMATION_HPP_

#include <glm/glm.hpp>

class Transformation
{
public:
  glm::mat4 transformation() const;

  glm::vec3 position;
  glm::vec3 angles;
};

#endif // _TRANSFORMATION_HPP_
