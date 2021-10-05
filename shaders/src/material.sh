#define NUM_LIGHTS 2
#define iLightPointNum NUM_LIGHTS

uniform vec4 Roughness_WrapL_Edge_Thickness;
uniform vec4 fDisableLighting_top_below;
uniform vec4 cAmbient_LightRange;
uniform vec4 fenvEmissionScale_TexWidth;
uniform vec4 hdrEnvTextures;

SAMPLER2D(texSampler1, 1);
SAMPLER2D(texSampler2, 2);

float GeometricOpacity(const float NdotV, const float alpha, const float blending, const float t)
{
	const float x = abs(NdotV);
	const float g = blending - blending * (x / (x * (1.0 - t) + t));
	return lerp(alpha, 1.0, g);
}

vec3 FresnelSchlick(const vec3 spec, const float LdotH, const float edge)
{
	return spec + (vec3(edge, edge, edge) - spec) * pow(1.0 - LdotH, 5);
}

vec3 DoPointLight(const vec3 pos, const vec3 N, const vec3 V, const vec3 diffuse, const vec3 glossy, const float edge, const float glossyPower, const int i, const bool is_metal)
{
	if (fDisableLighting_top_below.x == 1.0)
	{
		return diffuse;
	}

	vec3 lights = vec3(1, 1, 1);

	const vec3 lightDir = mul_w1(lights, matView) - pos;
	const vec3 L = normalize(lightDir);
	const float NdotL = dot(N, L);
	vec3 Out = vec3(0.0, 0.0, 0.0);

	if (!is_metal && (NdotL + Roughness_WrapL_Edge_Thickness.y > 0.0))
	{
		Out = diffuse * ((NdotL + Roughness_WrapL_Edge_Thickness.y) / sqr(1.0 + Roughness_WrapL_Edge_Thickness.y));
	}

	if (NdotL > 0.0)
	{
		const vec3 H = normalize(L + V);
		const float NdotH = dot(N, H);
		const float LdotH = dot(L, H);
		const float VdotH = dot(V, H);
		if ((NdotH > 0.0) && (LdotH > 0.0) && (VdotH > 0.0))
		{
			Out += FresnelSchlick(glossy, LdotH, edge) * (((glossyPower + 1.0) / (8.0 * VdotH)) * pow(NdotH, glossyPower));
		}
	}

	const float sqrl_lightDir = dot(lightDir, lightDir);
	float fAtten = saturate(1.0 - sqrl_lightDir * sqrl_lightDir / (cAmbient_LightRange.w * cAmbient_LightRange.w * cAmbient_LightRange.w * cAmbient_LightRange.w));
	fAtten = fAtten * fAtten / (sqrl_lightDir + 1.0);

	vec3 ambient = glossy;

	if (!is_metal)
	{
		ambient += diffuse;
	}

	// TODO: LIGHTS[i]

	const vec3 result = Out * /*lights.vEmission * */ fAtten + ambient * cAmbient_LightRange.xyz;
	if (fDisableLighting_top_below.x != 0.0)
	{
		return lerp(result, diffuse, fDisableLighting_top_below.x);
	}
	else
	{
		return result;
	}
}

vec3 DoEnvmapDiffuse(const vec3 N, const vec3 diffuse)
{
	const vec2 uv = vec2(
		0.5 + atan2_approx_div2PI(N.y, N.x),
		acos_approx_divPI(N.z));

	vec3 env = texture2DLod(texSampler2, vec4(uv, 0., 0.), 0).xyz;

	if (!hdrEnvTextures.x)
	{
		env = InvGamma(env);
	}

	return diffuse * env * fenvEmissionScale_TexWidth.x;
}

vec3 DoEnvmapGlossy(const vec3 N, const vec3 V, const vec2 Ruv, const vec3 glossy, const float glossyPower)
{
	const float mip = min(log2(fenvEmissionScale_TexWidth.y * sqrt(3.0)) - 0.5 * log2(glossyPower + 1.0), log2(fenvEmissionScale_TexWidth.y) - 1.);
	vec3 env = texture2DLod(texSampler1, vec4(Ruv, 0., mip), 0).xyz;
	
	if (!hdrEnvTextures.x) 
	{
		env = InvGamma(env);
	}

	return glossy * env * fenvEmissionScale_TexWidth.x;
}

vec3 DoEnvmap2ndLayer(const vec3 color1stLayer, const vec3 pos, const vec3 N, const vec3 V, const float NdotV, const vec2 Ruv, const vec3 specular)
{
	const vec3 w = FresnelSchlick(specular, NdotV, Roughness_WrapL_Edge_Thickness.z);
	vec3 env = texture2DLod(texSampler1, vec4(Ruv, 0., 0.), 0).xyz;
	
	if (!hdrEnvTextures.x) 
	{
		env = InvGamma(env);
	}

	return lerp(color1stLayer, env * fenvEmissionScale_TexWidth.x, w);
}

vec3 lightLoop(const vec3 pos, vec3 N, const vec3 V, vec3 diffuse, vec3 glossy, const vec3 specular, const float edge, const bool fix_normal_orientation, const bool is_metal)
{
	const float diffuseMax = max(diffuse.x, max(diffuse.y, diffuse.z));
	const float glossyMax = max(glossy.x, max(glossy.y, glossy.z));
	const float specularMax = max(specular.x, max(specular.y, specular.z));
	const float sum = diffuseMax + glossyMax;
	
	if (sum > 1.0)
	{
		const float invsum = 1.0 / sum;
		diffuse *= invsum;
		glossy *= invsum;
	}

	float NdotV = dot(N, V);
	if (fix_normal_orientation && (NdotV < 0.0))
	{
		N = -N;
		NdotV = -NdotV;
	}

	vec3 color = vec3(0.0, 0.0, 0.0);

	if ((!is_metal && (diffuseMax > 0.0)) || (glossyMax > 0.0))
	{
		for (int i = 0; i < iLightPointNum; i++)
		{
			color += DoPointLight(pos, N, V, diffuse, glossy, edge, Roughness_WrapL_Edge_Thickness.x, i, is_metal);
		}
	}

	if (!is_metal && (diffuseMax > 0.0))
	{
		color += DoEnvmapDiffuse(normalize(mul(matView, N).xyz), diffuse);
	}

	if ((glossyMax > 0.0) || (specularMax > 0.0))
	{
		vec3 R = (2.0 * NdotV) * N - V;
		R = normalize(mul(matView, R).xyz);

		const vec2 Ruv = vec2(
			0.5 + atan2_approx_div2PI(R.y, R.x),
			acos_approx_divPI(R.z));

		if (glossyMax > 0.0)
		{
			color += DoEnvmapGlossy(N, V, Ruv, glossy, Roughness_WrapL_Edge_Thickness.x);
		}

		if (specularMax > 0.0)
		{
			color = DoEnvmap2ndLayer(color, pos, N, V, NdotV, Ruv, specular);
		}
	}

	return color;
}