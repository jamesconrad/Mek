#version 430

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 Depth;
layout(location = 2) out vec4 Normal;
layout(location = 3) out vec4 LightObscurers;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec3 Tangent0;
in vec3 Bitangent0;
in vec3 FragPos;
in vec4 FragPosLightSpace;

const int MAX_POINT_LIGHTS = 16;
const int MAX_SPOT_LIGHTS = 16;
vec3 warmColour = vec3(0.05, 0.0, 0.0);
float alphaWarm = 0.1;
vec3 coolColour = vec3(0.0, 0.0, 0.05);
float alphaCool = 0.8;

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform sampler2D gColorMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
uniform float textureCutoff;
//Uniforms had to be included below.
//uniform PointLight gPointLights[MAX_POINT_LIGHTS]; //a ball of light
//uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS]; //a flashlight

struct VSOutput
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 WorldPos;
};

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};


uniform PointLight gPointLights[MAX_POINT_LIGHTS]; //a ball of light
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS]; //a flashlight

//New code by Jordan Culver
vec3 PointLambert(VSOutput In, PointLight Light)
{
	vec3 DiffuseColour = vec3(0.0);
	vec3 VtoLight; float LightFactor;
	float Distance; float Attenuation;

	VtoLight = Light.Position - In.WorldPos;
	Distance = length(VtoLight);
	VtoLight = normalize(VtoLight);
	LightFactor = 0.5 * dot(In.Normal, VtoLight) + 0.5;
	LightFactor *= LightFactor;
	DiffuseColour = Light.Base.Color * LightFactor * Light.Base.DiffuseIntensity;
	Attenuation = Light.Atten.Constant +
				  Light.Atten.Linear * Distance +
				  Light.Atten.Exp * Distance * Distance;
	DiffuseColour /= Attenuation;
	return DiffuseColour;
}

vec3 SpotLambert(VSOutput In, SpotLight Light)
{
	vec3 DiffuseColour = vec3(0.0);
	vec3 VtoLight; float LightFactor;
	float Distance; float Attenuation;

	VtoLight = Light.Base.Position - In.WorldPos;
	Distance = length(VtoLight);
	VtoLight = normalize(VtoLight);
	LightFactor = 0.5 * dot(In.Normal, VtoLight) + 0.5;
	LightFactor *= LightFactor;
	float SpotFactor = dot(VtoLight, Light.Direction);
	if (SpotFactor > Light.Cutoff)
	{
		DiffuseColour = Light.Base.Base.Color * LightFactor * Light.Base.Base.DiffuseIntensity;
		Attenuation = Light.Base.Atten.Constant +
					  Light.Base.Atten.Linear * Distance +
					  Light.Base.Atten.Exp * Distance * Distance;
		DiffuseColour *= (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - Light.Cutoff));
		DiffuseColour /= Attenuation;
		return DiffuseColour;
	}
	else
	{
		return vec3(0.0);
	}
}

vec3 PointSpecular(VSOutput In, PointLight Light)
{
	float Specular;
	vec3 viewDirection = normalize(gEyeWorldPos - In.WorldPos);
	vec3 lightToPoint = normalize(In.WorldPos - Light.Position);
	vec3 reflectedLight = normalize(reflect(lightToPoint, In.Normal));

	//vec3 ReflectedLight = normalize(reflect(normalize(Light.Position - In.WorldPos), In.Normal));
	Specular = max(dot(reflectedLight, viewDirection),0.0); //normalize(gEyeWorldPos - Light.Position));
	Specular = pow(Specular, 24);
	return Light.Base.Color * Specular * gMatSpecularIntensity; // * specularMask;
}

float getSpecFresnelTerm(VSOutput In)
{
	vec3 EtoV = In.WorldPos - gEyeWorldPos;
	//float fresnelFactor = dot(In.Normal, EtoV);
	float fres = max(0.0, min(1.0, 0.1 + 0.5 * (1.0 - dot(In.Normal, EtoV))));
	//float fres = fresnelFactor*fresnelFactor;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	return fres;
}

float getRimFresnelTerm(VSOutput In)
{
	vec3 EtoV = In.WorldPos - gEyeWorldPos;
	//float fresnelFactor = dot(In.Normal, EtoV);
	float fres = max(0.0, min(1.0, 0.0 + 0.1 * (1.0 - dot(EtoV, In.Normal))));
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	fres *= fres;
	//float fres = 1.0 - fresnelFactor*fresnelFactor*fresnelFactor*fresnelFactor; 
	return fres;
}

vec3 getGoochColour(VSOutput In, vec3 coolColour, vec3 warmColour, PointLight Light)
{
	vec3 lightVector = normalize(Light.Position - In.WorldPos);
	float revisedDotProduct = (1.0 + dot(lightVector, In.Normal))/2;
	vec3 kfinal = vec3(((revisedDotProduct * coolColour + (1 - revisedDotProduct) * warmColour)));
	//vec3 kfinal = kCool;
	return kfinal;
}

in float vertDepth;
void main()
{

    VSOutput In;
    In.TexCoord = TexCoord0;
    //In.Normal   = normalize(Normal0); //we use normal maps now
    In.WorldPos = WorldPos0;

	mat3 TBN = mat3(normalize(Tangent0),normalize(Bitangent0),normalize(Normal0));
	In.Normal = texture2D(normalMap, TexCoord0).rgb;
	In.Normal = normalize(In.Normal * 2.0 - 1.0);   
	In.Normal = normalize(TBN * In.Normal);

	//vec3 specularMask; = vec3(texture(gSpecularMap, In.TexCoord.xy));

	//vec3 N = In.normal;
	//vec3 V = normalize(gEyeWorldPos - In.WorldPos);
	//vec3 L = normalize(lightPos - vIn.vertex);
	//vec3 H = normalize(L+V);

	vec3 Albedo = vec3(texture(gColorMap, In.TexCoord.xy));
	float transparency = texture(gColorMap, In.TexCoord.xy).a;

	vec3 kCool = coolColour + alphaWarm * Albedo;
	vec3 kWarm = warmColour + alphaCool * Albedo;

	vec3 Lambert = vec3(0.0);

	vec3 goochColour = vec3(0.0);
	vec3 ConstantAmbient = vec3(0.1);

	float SpecFresnel = getSpecFresnelTerm(In);

	vec3 SpecularHighlights  = vec3(0.0);

	float RimFresnel = getRimFresnelTerm(In);

	vec3 RimHighlights = vec3(0.0);
	vec3 WorldUp = vec3(0.0, 1.0, 0.0);


	for (int i = 0; i < gNumPointLights; i++)
	{
		Lambert += PointLambert(In, gPointLights[i]);
		SpecularHighlights += max(PointSpecular(In, gPointLights[i]) * SpecFresnel, PointSpecular(In, gPointLights[i]) * RimFresnel);
	}
	for (int i = 0; i < gNumSpotLights; i++)
	{
		Lambert += SpotLambert(In, gSpotLights[i]);
		//SpecularHighlights += PointSpecular(In, gSpotLights[i].Base) * SpecFresnel;
		SpecularHighlights += PointSpecular(In, gSpotLights[i].Base) * SpecFresnel;
		goochColour += getGoochColour(In, kWarm, kCool, gSpotLights[i].Base);
	}
	RimHighlights += clamp(dot(In.Normal,WorldUp), 0.0, 1.0) * RimFresnel * ConstantAmbient; /** (ConstantAmbient * normalize(gEyeWorldPos - In.WorldPos));*/
	
	float ShadowCoeff = 1;
	if (texture2D(shadowMap, FragPosLightSpace.xy).r < FragPosLightSpace.z)
		ShadowCoeff = 0.25;
	FragColor = vec4((((goochColour) * (Lambert + ConstantAmbient)) + (SpecularHighlights + RimHighlights)) /* ShadowCoeff*/, 1.0) * transparency;
	Depth = vec4(vec3(vertDepth), 1.0) * transparency;
	Normal = vec4(vec3(In.Normal * 0.5 + 0.5), 1.0) * transparency;
	LightObscurers = vec4(0.0, 0.0, 0.0, 1.0) * transparency;
	if (In.TexCoord.x > textureCutoff)
	{
		FragColor.a = 0.0;
		Depth.a = 0.0;
		Normal.a = 0.0;
		LightObscurers.a = 0.0;
	}
	//FragColor = vec4((Albedo + goochColour), 1.0);
}