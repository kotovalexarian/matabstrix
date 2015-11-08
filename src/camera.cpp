#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::transformation() const
{
  return projection * glm::inverse(Transformation::transformation());
}
