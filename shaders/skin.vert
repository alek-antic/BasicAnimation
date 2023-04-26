// position & normal already in world coordinates
varying vec3 position;
varying vec3 normal;

varying vec3 fragPos;
varying vec3 fragNorm;

uniform mat4 projection;
uniform mat4 view;


void main () {
    gl_Position = projection * view * vec4(position, 1.0);
    fragPos = position;
    fragNorm = normal;
}
