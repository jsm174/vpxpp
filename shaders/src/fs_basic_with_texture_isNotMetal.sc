$input v_texcoord0, v_worldPos, v_normal

#include <bgfx_shader.sh>
#include "helpers.sh"

#define IS_METAL 0
#define DO_NORMAL_MAPPING 0

uniform vec4 alphaTestValue;
uniform vec4 cBase_Alpha;
uniform vec4 cGlossy_ImageLerp;
uniform vec4 cClearcoat_EdgeAlpha;
uniform vec4 Roughness_WrapL_Edge_Thickness;
uniform vec4 fDisableLighting_top_below;
uniform vec4 objectSpaceNormalMap;

uniform mat4 matWorldViewProj;
uniform mat4 matWorldView;
uniform mat4 matWorldViewInverseTranspose;
uniform mat4 matView;

SAMPLER2D(texSampler0, 0);
SAMPLER2D(texSamplerBL, 3);
SAMPLER2D(texSamplerN, 4);

#include "material.sh"

float3x3 TBN_trafo(const vec3 N, const vec3 V, const vec2 uv, const vec3 dpx, const vec3 dpy)
{
	const vec2 duvx = ddx(uv);
	const vec2 duvy = ddy(uv);

	const vec3 dp2perp = cross(N, dpy);
	const vec3 dp1perp = cross(dpx, N);
	const vec3 T = dp2perp * duvx.x + dp1perp * duvy.x;
	const vec3 B = dp2perp * duvx.y + dp1perp * duvy.y;

	return float3x3(T, B, N * sqrt(max(dot(T, T), dot(B, B))));
}

vec3 normal_map(const vec3 N, const vec3 V, const vec2 uv)
{
	vec3 tn = texture2D(texSamplerN, uv).xyz * (255. / 127.) - (128. / 127.);

	const vec3 dpx = ddx(V);
	const vec3 dpy = ddy(V);

	if (objectSpaceNormalMap.x)
	{
		tn.z = -tn.z;
		return normalize(mul(tn, matWorldViewInverseTranspose).xyz);
	}

	return normalize(mul(TBN_trafo(N, V, uv, dpx, dpy),
						 tn));
}

void main()
{
	vec4 pixel = texture2D(texSampler0, v_texcoord0.xy);

	clip(pixel.a <= alphaTestValue.x ? -1 : 1);

	pixel.a *= cBase_Alpha.a;
	const vec3 t = (pixel.xyz);

	const vec3 diffuse = t * cBase_Alpha.xyz;
	const vec3 glossy = IS_METAL ? diffuse : (t * cGlossy_ImageLerp.w + (1.0 - cGlossy_ImageLerp.w)) * cGlossy_ImageLerp.xyz * 0.08;
	const vec3 specular = cClearcoat_EdgeAlpha.xyz * 0.08;
	const float edge = IS_METAL ? 1.0 : Roughness_WrapL_Edge_Thickness.z;

	const vec3 V = normalize(-v_worldPos);
	vec3 N = normalize(v_normal);

	if (DO_NORMAL_MAPPING)
	{
		N = normal_map(N, V, v_texcoord0.xy);
	}

	vec4 result = vec4(
		lightLoop(v_worldPos, N, V, diffuse, glossy, specular, edge, !DO_NORMAL_MAPPING, IS_METAL),
		pixel.a);

	if (cBase_Alpha.a < 1.0 && result.a < 1.0)
	{
		result.a = GeometricOpacity(dot(N, V), result.a, cClearcoat_EdgeAlpha.w, Roughness_WrapL_Edge_Thickness.w);

		if (fDisableLighting_top_below.y < 1.0)
		{
			result.xyz += lerp(sqrt(diffuse) * texture2DLod(texSamplerBL, vec4(vec2(0.5 * v_texcoord0.z, -0.5 * v_texcoord0.w) + 0.5, 0., 0.), 0.).xyz * result.a, 0., fDisableLighting_top_below.y);
		}
	}

	gl_FragColor = result;
}