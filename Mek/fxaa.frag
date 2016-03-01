#version 430

in vec2 uv;
layout (location = 0) out vec4 colour;
uniform sampler2D tex;

uniform vec2 screensize;

//predefined fxaa vals
uniform float FXAA_SPAN_MAX = 8.0; //max checking span
uniform float FXAA_REDCUE_MIN = 1.0/128.0; //1.f/128.f used for biasing the sample
uniform float FXAA_REDUCE_MUL = 1.0/8.0; //1.f/8.f, used for biasing the sample

void main()
{
	vec2 invVP = 1/screensize;

	vec3 l = vec3(0.299, 0.587, 0.114); //luminosity vector dot(l,colour.rgb) = luma value per pixel
	float lTL = dot(l, texture2D(tex, uv + (vec2(-1.0, -1.0) * invVP)).xyz);
	float lTR = dot(l, texture2D(tex, uv + (vec2(1.0, -1.0) * invVP)).xyz);
	float lBL = dot(l, texture2D(tex, uv + (vec2(-1.0, 1.0) * invVP)).xyz);
	float lBR = dot(l, texture2D(tex, uv + (vec2(1.0, 1.0) * invVP)).xyz);
	float lM  = dot(l, texture2D(tex, uv).xyz);

	vec2 blurDir;//if near 0,0 then the pixel is not blured
	blurDir.x = -((lTL + lTR) - (lBL + lBR));
	blurDir.y = ((lTL + lBL) - (lTR + lBR));
	//scale blurDir so smallest val = 1 texel
	float dirReduce = max((lTL + lTR + lBL + lBR) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDCUE_MIN);
	float blurScale = 1.0 / (min(abs(blurDir.x), abs(blurDir.y)) + dirReduce);

    //cap off the blurDir so it is between 8,8 and -8,-8, also transform into texel space
	blurDir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              blurDir * blurScale)) * invVP; //is this the problem?
	//blurDir = clamp(blurDir * blurScale, vec2(FXAA_SPAN_MAX), vec2(-FXAA_SPAN_MAX)) * invVP;

	//now we actually blur
	vec3 res1 = (0.5 * (texture2D(tex, uv + (blurDir * (1.0/3.0 - 0.5))).xyz + texture2D(tex, uv + (blurDir * (2.0/3.0 - 0.5))).xyz));
	vec3 res2 = (texture2D(tex, uv + (blurDir * (0.0/3.0 - 0.5))).xyz + texture2D(tex, uv + (blurDir * (3.0/3.0 - 0.5))).xyz);
	vec3 result = res1 * 0.5 + res2 * 0.25;

	//if the new luma is too far off, then we use res1
	float luma = dot(l, result);
	float lMin = min(lM, min(min(lTL, lTR), min(lBL, lBR)));
	float lMax = max(lM, max(max(lTL, lTR), max(lBL, lBR)));
	if (luma < lMin || luma > lMax)
		colour = vec4(res1, 1.0); //outside of range, use the acutal value
	else
		colour = vec4(result, 1.0);//result
}