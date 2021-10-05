#define PI 3.1415926535897932384626433832795

float sqr(const float v)
{
	return v * v;
}

vec2 sqr(const vec2 v)
{
	return v * v;
}

vec3 sqr(const vec3 v)
{
	return v * v;
}

vec3 mul_w1(const vec3 v, const float4x4 m)
{
	return v.x * m[0].xyz + (v.y * m[1].xyz + (v.z * m[2].xyz + m[3].xyz));
}

vec3 mul_w1(const vec3 v, const float4x3 m)
{
	return v.x * m[0] + (v.y * m[1] + (v.z * m[2] + m[3]));
}

vec3 mul_w0(const vec3 v, const float4x3 m)
{
	return v.x * m[0] + v.y * m[1] + v.z * m[2];
}

float acos_approx_divPI(const float v)
{
	const float x = abs(v);
	if (1. - x < 0.0000001)
	{
		return (v >= 0.) ? 0. : 1.;
	}
	const float res = ((-0.155972 / PI) * x + (1.56467 / PI)) * sqrt(1. - x);
	return (v >= 0.) ? res : 1. - res;
}

float atan2_approx_div2PI(const float y, const float x)
{
	const float abs_y = abs(y);
	const float abs_x = abs(x);

	if (abs_x < 0.0000001 && abs_y < 0.0000001)
	{
		return 0.;
	}

	const float r = (abs_x - abs_y) / (abs_x + abs_y);
	const float angle = ((x < 0.) ? (3. / 8.) : (1. / 8.)) + (0.211868 / (2. * PI) * r * r - 0.987305 / (2. * PI)) * ((x < 0.) ? -r : r);
	return (y < 0.) ? -angle : angle;
}

#define BURN_HIGHLIGHTS 0.25

float InvsRGB(const float color)
{
	if (color <= 0.04045)
		return color * (1.0 / 12.92);
	else
		return pow(color * (1.0 / 1.055) + (0.055 / 1.055), 2.4);
}

float InvRec709(const float color)
{
	if (color <= 0.081)
		return color * (1.0 / 4.5);
	else
		return pow(color * (1.0 / 1.099) + (0.099 / 1.099), 1.0 / 0.45);
}

float InvGamma(const float color)
{
	return InvsRGB(color);
}

vec3 InvGamma(const vec3 color)
{
	return vec3(InvGamma(color.x), InvGamma(color.y), InvGamma(color.z));
}

vec3 InvToneMap(const vec3 color)
{
	const float inv_2bh = 0.5 / BURN_HIGHLIGHTS;
	const float bh = 4.0 * BURN_HIGHLIGHTS - 2.0;
	return (color - 1.0 + sqrt(color * (color + bh) + 1.0)) * inv_2bh;
}

float sRGB(const float f)
{
	float s;
	if (f <= 0.0031308)
		s = 12.92 * f;
	else
		s = 1.055 * pow(f, 1.0 / 2.4) - 0.055;

	return s;
}

float Rec709(const float f)
{
	float s;
	if (f <= 0.018)
		s = 4.5 * f;
	else
		s = 1.099 * pow(f, 0.45) - 0.099;

	return s;
}

float FBGamma(const float color)
{
	return sRGB(color);
}

vec2 FBGamma(const vec2 color)
{
	return vec2(FBGamma(color.x), FBGamma(color.y));
}

vec3 FBGamma(const vec3 color)
{
	return vec3(FBGamma(color.x), FBGamma(color.y), FBGamma(color.z));
}

float FBToneMap(const float l)
{
	return l * ((l * BURN_HIGHLIGHTS + 1.0) / (l + 1.0));
}

vec2 FBToneMap(const vec2 color)
{
	const float l = dot(color, vec2(0.176204 + 0.0108109 * 0.5, 0.812985 + 0.0108109 * 0.5));
	return color * ((l * BURN_HIGHLIGHTS + 1.0) / (l + 1.0));
}

vec3 FBToneMap(const vec3 color)
{
	const float l = dot(color, vec3(0.176204, 0.812985, 0.0108109));
	return color * ((l * BURN_HIGHLIGHTS + 1.0) / (l + 1.0));
}

vec4 Additive(const vec4 cBase, const vec4 cBlend, const float percent)
{
	return cBase + cBlend * percent;
}

vec3 Screen(const vec3 cBase, const vec3 cBlend)
{
	return 1.0 - (1.0 - cBase) * (1.0 - cBlend);
}

vec4 Screen(const vec4 cBase, const vec4 cBlend)
{
	return 1.0 - (1.0 - cBase) * (1.0 - cBlend);
}

vec3 ScreenHDR(const vec3 cBase, const vec3 cBlend)
{
	return max(1.0 - (1.0 - cBase) * (1.0 - cBlend), vec3(0., 0., 0.));
}

vec4 ScreenHDR(const vec4 cBase, const vec4 cBlend)
{
	return max(1.0 - (1.0 - cBase) * (1.0 - cBlend), vec4(0., 0., 0., 0.));
}

vec4 Multiply(const vec4 cBase, const vec4 cBlend, const float percent)
{
	return cBase * cBlend * percent;
}

vec4 Overlay(const vec4 cBase, const vec4 cBlend)
{
	vec4 cNew = step(0.5, cBase);

	cNew = lerp(cBase * cBlend * 2.0, 1.0 - 2.0 * (1.0 - cBase) * (1.0 - cBlend), cNew);

	return cNew;
}

vec4 OverlayHDR(const vec4 cBase, const vec4 cBlend)
{
	vec4 cNew = step(0.5, cBase);

	cNew = max(lerp(cBase * cBlend * 2.0, 1.0 - 2.0 * (1.0 - cBase) * (1.0 - cBlend), cNew), vec4(0., 0., 0., 0.));

	return cNew;
}