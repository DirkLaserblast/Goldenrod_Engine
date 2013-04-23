#version 150

uniform bool lBlock;
//uniform sampler2D tex0;

in vec3 L;
in vec3 N;
in vec3 V;
in vec3 H;
in vec4 frag_color;
//in vec2 texCoord;

out vec4 end_color;

const float s = 64.0;
const vec4 L_s = vec4(1.0);
const vec4 L_d = vec4(1.0);
const vec4 K_s = vec4(1.0);

void main()
{
    //vec4 c = vec4(texture(tex0, texCoord));
    //vec4 c = frag_color;
    
    //float phi = acos(dot(N,H));
    //float LdotN = dot(L,N);

    //vec4 diff = c * L_d * LdotN;
    //vec4 spec = K_s * L_s * pow(cos(phi), s);
    //end_color = clamp(diff + max(spec,0.0), 0.0, 1.0);

	end_color = frag_color;
}
