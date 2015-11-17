precision lowp float;

varying vec2 f_tex_coord;
varying float light_weight;

uniform sampler2D texture;

void main(void) {
  vec2 flipped_tex_coord = vec2(f_tex_coord.x, 1.0 - f_tex_coord.y);
  vec4 texture_color = texture2D(texture, flipped_tex_coord);
  gl_FragColor = vec4(texture_color.rgb * light_weight, texture_color.a);
}
