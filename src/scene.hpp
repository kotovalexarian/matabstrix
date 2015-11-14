#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "object.hpp"
#include "light/sun.hpp"

#include <vector>

#include <glm/glm.hpp>

struct Scene
{
  inline Scene &operator<<(const Object *object);
  inline Scene &operator<<(const Lights::Sun *sun);

  inline const Lights::Sun *sun() const;

  inline void draw(const glm::mat4 &mvp) const;

private:
  std::vector<const Object*> _objects;
  const Lights::Sun *_sun = nullptr;
};

Scene &Scene::operator<<(const Object *object)
{
  _objects.push_back(object);
  return *this;
}

Scene &Scene::operator<<(const Lights::Sun *sun)
{
  _sun = sun;
  return *this;
}

const Lights::Sun *Scene::sun() const
{
  return _sun;
};

void Scene::draw(const glm::mat4 &mvp) const
{
  for (auto object : _objects)
    object->draw(*this, mvp);
}

#endif // _SCENE_HPP_
