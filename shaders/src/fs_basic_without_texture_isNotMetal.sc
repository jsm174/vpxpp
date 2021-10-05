$input v_worldPos_t1x, v_normal_t1y 

#define IS_METAL 0

#include <bgfx_shader.sh>
#include "helpers.sh"

uniform vec4 cBase_Alpha;
uniform vec4 cGlossy_ImageLerp;
uniform vec4 cClearcoat_EdgeAlpha;
uniform vec4 Roughness_WrapL_Edge_Thickness;
uniform vec4 fDisableLighting_top_below;

uniform mat4 matWorldViewProj;
uniform mat4 matWorldView;
uniform mat4 matWorldViewInverseTranspose;
uniform mat4 matView;

SAMPLER2D(texSamplerBL, 3);

#include "material.sh"

void main()
{
	const vec3 diffuse = cBase_Alpha.xyz;
	const vec3 glossy = IS_METAL ? cBase_Alpha.xyz : cGlossy_ImageLerp.xyz*0.08;
	const vec3 specular = cClearcoat_EdgeAlpha.xyz * 0.08;
	const float edge = IS_METAL ? 1.0 : Roughness_WrapL_Edge_Thickness.z;

	const vec3 V = normalize(-v_worldPos_t1x.xyz);
	const vec3 N = normalize(v_normal_t1y.xyz);

	vec4 result = vec4(
		lightLoop(v_worldPos_t1x.xyz, N, V, diffuse, glossy, specular, edge, true, IS_METAL),
	    cBase_Alpha.a);

	if (cBase_Alpha.a < 1.0) {
		result.a = GeometricOpacity(dot(N,V),result.a,cClearcoat_EdgeAlpha.w,Roughness_WrapL_Edge_Thickness.w);

		if (fDisableLighting_top_below.y < 1.0) {
			result.xyz += lerp(sqrt(diffuse)*texture2DLod(texSamplerBL, vec4(float2(0.5*v_worldPos_t1x.w,-0.5*v_normal_t1y.w)+0.5, 0.,0.), 0.).xyz*result.a, 0., fDisableLighting_top_below.y); 
		}
	}

	gl_FragColor = result;
}