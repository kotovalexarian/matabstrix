precision lowp float;

varying float light_weight;

void main(void) {
  gl_FragColor = vec4(vec3(0.5, 0.5, 0.5) * light_weight, 1.0);
}
