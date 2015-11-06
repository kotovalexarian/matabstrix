precision lowp float;

varying vec2 f_tex_coord;

uniform sampler2D texture;

void main(void) {
  vec2 flipped_tex_coord = vec2(f_tex_coord.x, 1.0 - f_tex_coord.y);
  gl_FragColor = texture2D(texture, flipped_tex_coord);
}
