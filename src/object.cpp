#include "object.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Object::draw(const glm::mat4 &mvp) const
{
  const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

  const glm::mat4 rotation =
    glm::rotate(glm::mat4(1.0f), glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));

  const glm::mat4 transform = mvp * translation * rotation;
  glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(transform));

  const glm::mat3 local_modelview = glm::transpose(glm::inverse(glm::mat3(translation * rotation)));
  glUniformMatrix3fv(local_modelview_uniform, 1, GL_FALSE, glm::value_ptr(local_modelview));

  _model.draw();
}
