attribute vec4 position;
attribute vec3 color;

varying vec4 f_color;

uniform mat4 mvp;

void main(void) {
  gl_Position = mvp * position;
  f_color = vec4(color, 1.0);
}
