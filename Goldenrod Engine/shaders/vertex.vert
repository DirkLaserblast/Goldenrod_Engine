#version 150

//These variables are constant for all vertices
uniform mat4 M; //modelview matrix
uniform mat4 P; //projection matrix
uniform mat3 M_n; //normal matrix
uniform vec3 L_p; //light position
uniform vec3 E; //view position
uniform float time; //time variable

//input variables from host
in vec3 pos; //vertex position
in vec3 norm; //vertex normal
in vec4 color; //vertex color
//in vec2 tex;

//variables to be passed to the fragment shader
out vec3 L;
out vec3 N;
out vec3 V;
out vec3 H;
out vec4 frag_color;
//out vec2 texCoord;

void main()
{
    //texCoord = tex;

	vec3 lightPosition = vec3(0, 0, 1);

    vec4 posT = M * vec4(pos,1.0);
    
    L = normalize(M * vec4(lightPosition, 1.0)).xyz;
    V = (M * vec4(E,1.0)).xyz;
    
    N = normalize(M_n * norm);
    L = normalize(L - posT.xyz);
    V = normalize(V - posT.xyz);
    H = normalize(L + V);
    
    frag_color = color;
    
    gl_Position = P * posT;
}
