$input a_position, a_normal, a_texcoord0
$output v_texcoord0, v_worldPos, v_normal

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
	v_texcoord0 = vec4(a_texcoord0, pos.xy / pos.w);
	v_worldPos = P;
	v_normal = N;

	gl_Position = pos;
}