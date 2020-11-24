#version 330 core

#define MAX_POINT_LIGHTS 30

in vec3 fragNor;
in vec3 fragPos;

uniform vec3 eye;
uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;

uniform vec3 lightPositions[MAX_POINT_LIGHTS];
uniform vec3 lightColInt[MAX_POINT_LIGHTS];
uniform vec3 lightABC[MAX_POINT_LIGHTS];
uniform float numLights;

out vec4 color;

void main()
{
    vec3 globalLightDir = normalize(vec3(-1, -0.3, -0.5));
    vec3 globalLightCol = vec3(0.31, 0.412, 0.533);

    vec3 normal = normalize(fragNor);
    vec3 V = normalize(eye-fragPos);
    vec3 tempColor = vec3(0,0,0);

    for(int i = 0; i < numLights; i++)
    {
        float a = lightABC[i].x;
        float b = lightABC[i].y;
        float c = lightABC[i].z;

        float lightDist = distance(lightPositions[i], fragPos);
        vec3 L = normalize(lightPositions[i] - fragPos);
        vec3 H = normalize(L + V);

        //spec reflectance
        float sc = max(0, dot(H, normal));
        sc = pow(sc, shine);
        vec3 speccolor = (sc*MatSpec)*lightColInt[i];
        
        //diff reflectance
        float dc = max(0, dot(normal, L));
        vec3 difcolor = (dc*MatDif)*lightColInt[i];

        //distance attenuation factor
        float distAtt = a + (b*lightDist) + c*pow(lightDist, 2);

        tempColor += ((1/distAtt) * (difcolor + speccolor));
    }

    float dc = max(0, dot(normal, globalLightDir));
    vec3 difcolor = (dc*MatDif)*globalLightCol;
    tempColor += difcolor;

	color = vec4(tempColor, 1.0) + vec4(MatAmb*globalLightCol, 1.0);
}
