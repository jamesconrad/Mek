#version 330

const int MAX_POINT_LIGHTS = 16;
const int MAX_SPOT_LIGHTS = 16;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in vec4 Debug0;
in ivec4 Debug1;

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

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
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

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight; //the "sun"
uniform PointLight gPointLights[MAX_POINT_LIGHTS]; //a ball of light
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS]; //a flashlight
uniform sampler2D gColorMap;
uniform vec3 gEyeWorldPos;
uniform vec3 EyeViewVec;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;


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
	vec3 ReflectedLight = normalize(reflect(normalize( Light.Position - In.Normal), In.Normal));
	Specular = dot(EyeViewVec, ReflectedLight);
	return Light.Base.Color * Specular * gMatSpecularIntensity;
}

float getSpecFresnelTerm(VSOutput In)
{
	vec3 EtoV = In.WorldPos - gEyeWorldPos;
	float fres = max(0.0, min(1.0, 0.5 + 1.0 * (1.0 + dot(EtoV, In.Normal))));
	//fres *= fres;
	return fres;
}

float getRimFresnelTerm(VSOutput In)
{
	vec3 EtoV = In.WorldPos - gEyeWorldPos;
	float fres = max(0.0, min(1.0, 0.0 + 0.1 * (1.0 + dot(EtoV, In.Normal))));
	fres *= fres;
	fres *= fres;
	return fres;
}


out vec4 FragColor;


void main()
{

    VSOutput In;
    In.TexCoord = TexCoord0;
    In.Normal   = normalize(Normal0);
    In.WorldPos = WorldPos0;

	//vec3 N = In.normal;
	//vec3 V = normalize(gEyeWorldPos - In.WorldPos);
	//vec3 L = normalize(lightPos - vIn.vertex);
	//vec3 H = normalize(L+V);

	vec3 Albedo = vec3(texture(gColorMap, In.TexCoord.xy));

	vec3 Lambert = vec3(0.0);

	vec3 ConstantAmbient = vec3(0.3);

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
		SpecularHighlights += max(PointSpecular(In, gSpotLights[i].Base) * SpecFresnel, PointSpecular(In, gSpotLights[i].Base) * RimFresnel);
	}
	RimHighlights += clamp(dot(In.Normal,WorldUp), 0.0, 1.0) * RimFresnel * (ConstantAmbient * normalize(gEyeWorldPos - In.WorldPos));


	FragColor = vec4((Albedo * (Lambert + ConstantAmbient)) + (SpecularHighlights + RimHighlights), 1.0);
}

//Old Code., By James Conrad
//vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, VSOutput In)
//{
//    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
//    float DiffuseFactor = dot(In.Normal, -LightDirection);
//
//    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
//    vec4 SpecularColor = vec4(0, 0, 0, 0);
//
//    if (DiffuseFactor > 0.0)
//	{
//        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);
//
//        vec3 VertexToEye = normalize(gEyeWorldPos - In.WorldPos);
//        vec3 LightReflect = normalize(reflect(LightDirection, In.Normal));
//	    //vec3 LightReflect = normalize(LightDirection + VertexToEye); //literally all I had to add to go from phong to blinn-phong lighting... feh.
//        float SpecularFactor = dot(VertexToEye, LightReflect);
//        if (SpecularFactor > 0.0)
//		{
//            SpecularFactor = pow(SpecularFactor, gSpecularPower);
//            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
//        }
//    }
//
//    return (AmbientColor + DiffuseColor + SpecularColor);
//}
//
//vec4 CalcDirectionalLight(VSOutput In)
//{
//    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, In);
//}
//
//vec4 CalcPointLight(PointLight l, VSOutput In)
//{
//    vec3 LightDirection = In.WorldPos - l.Position;
//    float Distance = length(LightDirection);
//    LightDirection = normalize(LightDirection);
//
//    vec4 Color = CalcLightInternal(l.Base, LightDirection, In);
//    float Attenuation =  l.Atten.Constant +
//                         l.Atten.Linear * Distance +
//                         l.Atten.Exp * Distance * Distance;
//
//    return Color / Attenuation;
//}
//
//
//vec4 CalcSpotLight(SpotLight l, VSOutput In)
//{
//    vec3 LightToPixel = normalize(l.Base.Position - In.WorldPos);
//    float SpotFactor = dot(LightToPixel, l.Direction);
//
//    if (SpotFactor > l.Cutoff)
//	{
//        vec4 Color = CalcPointLight(l.Base, In);
//        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
//    }
//    else 
//	{
//        return vec4(0,0,0,0);
//    }
//}


//FragColor = vec4(SpecularHighlights + RimHighlights, 1.0);

    //vec4 TotalLight;// = CalcDirectionalLight(In);
	//
    //for (int i = 0 ; i < gNumPointLights ; i++)
	//{
    //    TotalLight += CalcPointLight(gPointLights[i], In);
    //}
	//
    //for (int i = 0 ; i < gNumSpotLights ; i++)
	//{
    //    TotalLight += CalcSpotLight(gSpotLights[0], In);
    //}
    //
	//FragColor = texture(gColorMap, In.TexCoord.xy);// * TotalLight;
	//FragColor = CalcSpotLight(gSpotLights[0], In);
	//FragColor = vec4(gSpotLights[0].Base.Position, 1);
	//FragColor = vec4(CalcPointLight(gSpotLights[0].Base, In).xyz, 1);