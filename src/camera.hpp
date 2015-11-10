#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "transformation.hpp"
#include "scene.hpp"

class Camera :
  public Transformation
{
public:
  inline Camera(const Scene &scene) : _scene(scene) {}

  inline void draw() const;
  glm::mat4 transformation() const;

  glm::mat4 projection;

private:
  const Scene &_scene;
};

void Camera::draw() const
{
  _scene.draw(transformation());
};

#endif // _CAMERA_HPP_
