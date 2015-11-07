#include "object.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Object::draw(const glm::mat4 &mvp) const
{
  const glm::mat4 transform = glm::translate(mvp, position)
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
    * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(transform));

  _model.draw();
}
