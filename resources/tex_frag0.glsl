#version 330 core

#define MAX_POINT_LIGHTS 30

uniform sampler2D Texture0;

uniform vec3 lightPositions[MAX_POINT_LIGHTS];
uniform vec3 lightColInt[MAX_POINT_LIGHTS];
uniform float numLights;

uniform float flip;

in vec2 vTexCoord;
in vec3 fragNor;
in vec3 fragPos;

out vec4 Outcolor;

void main() {
    vec3 globalLightDir = normalize(vec3(-1, -0.3, -0.5));
    vec3 globalLightCol = vec3(0.31, 0.412, 0.533);

    vec4 texColor0 = texture(Texture0, vTexCoord);
    vec3 normal = flip * normalize(fragNor);
    vec3 tempColor = vec3(0,0,0);
    float a = 0;
    float b = 1;
    float c = 0.1;

    for(int i = 0; i < numLights; i++)
    {
        float lightDist = distance(lightPositions[i], fragPos);
        vec3 L = normalize(lightPositions[i] - fragPos);

        float dc = max(0, dot(normal, L));
        vec3 difcolor = ((dc*texColor0).xyz)*lightColInt[i];
        
        float distAtt = a + (b*lightDist) + c*pow(lightDist, 2);

        tempColor += ((1/distAtt) * difcolor);
    }

    float dc = max(0, dot(normal, globalLightDir));
    vec3 difcolor = ((dc*texColor0).xyz)*globalLightCol;
    tempColor += difcolor;

  	Outcolor = vec4(tempColor, 1.0) + (0.20*texColor0)*vec4(globalLightCol, 1.0);
}

