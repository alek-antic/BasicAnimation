// in
varying vec3 fragPos;
varying vec3 fragNorm;

uniform vec3 matAmbient;
uniform vec3 matDiffuse;

uniform vec3 light1Pos;
uniform vec3 light1Color;

uniform vec3 light2Pos;
uniform vec3 light2Color;

//out
varying vec4 fragColor;

void main () {
    
    vec3 norm = normalize(fragNorm);
    vec3 light1Dir = normalize(light1Pos - fragPos);
    vec3 light2Dir = normalize(light2Pos - fragPos);
    
    float diffuseStrength = 1.0;
    float diff1 = max(dot(norm, light1Dir), 0.0);
    float diff2 = max(dot(norm, light2Dir), 0.0);
    vec3 diffuse = diff1 * light1Color + diff2 * light2Color;
    
    vec3 result = matAmbient + diffuse * matDiffuse;
    
    vec4 fragColor = vec4(result, 1.0);
}
