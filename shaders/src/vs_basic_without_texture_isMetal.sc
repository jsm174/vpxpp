$input a_position, a_normal
$output v_worldPos_t1x, v_normal_t1y

#include <bgfx_shader.sh>
#include "helpers.sh"

uniform mat4 matWorldViewProj;
uniform mat4 matWorldView;
uniform mat4 matWorldViewInverseTranspose;
uniform mat4 matView;

#include "material.sh"
 
void main()
{
	const vec3 P = mul(a_position, matWorldView).xyz;
	const vec3 N = normalize(mul(a_normal, matWorldViewInverseTranspose).xyz);
	
	vec4 pos = mul(a_position, matWorldViewProj);

	v_worldPos_t1x.w = pos.x/pos.w;
	v_normal_t1y.w = pos.y/pos.w;

	v_worldPos_t1x.xyz = P;
	v_normal_t1y.xyz = N;

	gl_Position = pos;
}