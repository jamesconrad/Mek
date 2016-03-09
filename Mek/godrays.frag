#version 430


uniform sampler2D tex0;
uniform sampler2D rgbTexture;

//uniform mat4 mvp;
//uniform vec4 viewport;
uniform vec4 lightPositionOnScreen = vec4(0.3, 0.7, 0.0, 0.0);

float exposure = 0.006;
float decay = 1.0;
float density = 1.7;
float weight = 3.0;
int numSamples = 100;

in vec2 uv;

layout(location = 0) out vec4 colour;


void main()
{
	vec2 dTexCoord = vec2(uv.xy - lightPositionOnScreen.xy);
	vec2 actualCoord = uv.xy;
	dTexCoord *= 1.0 / float(numSamples) * density;
	vec3 texSample = vec3(0.0, 0.0, 0.0);
	float illuminationDecay = 1.0;



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
	colour.rgb = colour.rgb + texture2D(rgbTexture, uv.xy).rgb; 
	colour.a = 1.0;
}