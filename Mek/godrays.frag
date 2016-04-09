#version 430


uniform sampler2D tex0;
uniform sampler2D rgbTexture;

//uniform mat4 mvp;
//uniform vec4 viewport = vec4(0.0, 0.0, 1280.0, 800.0);
//uniform vec2 depthRange = vec2(0.1, 1024.0);
uniform vec4 lightPositionOnScreen = vec4(0.3, 0.7, 0.0, 0.0);

uniform vec3 lightPositionInWorld;
uniform vec3 playerPositionInWorld;
uniform vec3 cameraForwardVector;

float exposure = 0.008;
float decay = 0.99;
float density = 1.0;
float weight = 5.0;
int numSamples = 50;

in vec2 uv;

layout(location = 0) out vec4 colour;


void main()
{
//	vec4 lightPos = mvp * lightPositionOnScreen;
//
//    double rhw = 1 / lightPos.w;
//	lightPos = vec4(
//	(1 + lightPos.x * rhw) * viewport[2] / 2 + viewport[0],
//   (1 - lightPos.y * rhw) * viewport[3] / 2 + viewport[1],
//   (lightPos.z * rhw) * (depthRange[1] - depthRange[0]) + depthRange[0],
//   rhw);
//
// 
//   lightPos.x /= 1280.0;
//   lightPos.y /= 800.0;



	vec2 dTexCoord = vec2(uv.xy - lightPositionOnScreen.xy);
//	dTexCoord = vec2(uv.xy - lightPos.xy);
	vec2 actualCoord = uv.xy;
	dTexCoord *= 1.0 / float(numSamples) * density;
	vec3 texSample = vec3(0.0, 0.0, 0.0);
	float illuminationDecay = 1.0;

	float pToLDotF = dot(normalize(lightPositionInWorld - playerPositionInWorld), cameraForwardVector);



	//for (int i = 0; i < numSamples; i++) //Stupid loop version
	//{
	//	texSample = texture2D(tex0, actualCoord).
	//	rgb;actualCoord -= dTexCoord;
	//	
	//	texSample *= illuminationDecay * weight;
	//
	//	colour.rgb += texSample;
	//
	//	illuminationDecay *= decay;
	//}

	//Slightly less stupid non-loop version. Currently at 100 samples.
		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;

		texSample = texture2D(tex0, actualCoord).
		rgb;actualCoord -= dTexCoord;		
		texSample *= illuminationDecay * weight;
		colour.rgb += texSample;
		illuminationDecay *= decay;
		//Fifty Samples

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;

		//texSample = texture2D(tex0, actualCoord).
		//rgb;actualCoord -= dTexCoord;		
		//texSample *= illuminationDecay * weight;
		//colour.rgb += texSample;
		//illuminationDecay *= decay;



	colour.rgb *= exposure;
	pToLDotF = max(pToLDotF, 0);
	//colour.rgb = colour.rgb * pToLDotF + texture2D(rgbTexture, uv.xy).rgb; 
	colour.a = 1.0;
}