#include "texture.hpp"

#include <string>

#include <SDL_image.h>

const std::string Texture::filename(const std::string &name)
{
  return "/data/textures/" + name;
}

Texture::Texture(const std::string &name)
{
  SDL_Surface *surface = IMG_Load(filename(name).c_str());

  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

  SDL_FreeSurface(surface);
}

void Texture::use() const
{
  glBindTexture(GL_TEXTURE_2D, _id);
}
