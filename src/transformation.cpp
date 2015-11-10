#include "transformation.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Transformation::transformation() const
{
  const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

  const glm::mat4 rotation =
    glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

  return base * translation * rotation;
}
