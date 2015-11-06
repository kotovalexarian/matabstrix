attribute vec4 position;
attribute vec2 tex_coord;

varying vec2 f_tex_coord;

uniform mat4 mvp;

void main(void) {
  gl_Position = mvp * position;
  f_tex_coord = tex_coord;
}
