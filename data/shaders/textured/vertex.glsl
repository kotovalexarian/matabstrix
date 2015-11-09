attribute vec4 position;
attribute vec2 tex_coord;
attribute vec3 normal;

varying vec2 f_tex_coord;
varying float light_weight;

uniform mat4 mvp;
uniform mat3 local_modelview;

void main(void) {
  gl_Position = mvp * position;
  f_tex_coord = tex_coord;
  vec3 transformed_normal = local_modelview * normal;
  light_weight = max(dot(transformed_normal, vec3(0.0, 1.0, 0.0)), 0.0);
}
