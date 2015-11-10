#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "object.hpp"

#include <vector>

#include <glm/glm.hpp>

class Scene
{
public:
  inline Scene &operator <<(Object *object);

  inline void draw(const glm::mat4 &mvp) const;

private:
  std::vector<Object*> _objects;
};

Scene &Scene::operator <<(Object *object)
{
  _objects.push_back(object);

  return *this;
}

void Scene::draw(const glm::mat4 &mvp) const
{
  for (auto object : _objects)
    object->draw(mvp);
}

#endif // _SCENE_HPP_
