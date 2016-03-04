#version 430


uniform sampler2D rgbTexture;
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;

uniform int sobelCorrection = 1;
uniform int magnitudeCorrection = -1;
uniform int magnitudeFix = 0;

uniform vec3 fboSize;

mat3 Gx = mat3(-1, -2, -1,
			    0,  0,  0, 
				1,  2,  1);
mat3 Gy = mat3 (-1, 0, 1, 
				-2, 0, 2, 
				-1, 0, 1);
//

// Vertex shader outputs
in vec2 uv;


layout(location = 0) out vec4 FragColour;


void main()
{
	vec2 pixelSize;
	pixelSize.x = 1/fboSize.x;
	pixelSize.y = 1/fboSize.y;

	float sumX = 0.0;
	float sumY = 0.0;

	//////Depth Texture
	sumX = sumX + texture2D(depthTexture, vec2(uv.x + pixelSize.x * -1, uv.y)).r * -8;
	sumX = sumX + texture2D(depthTexture, vec2(uv.x + pixelSize.x * 1, uv.y)).r * 8;
	
	sumY = sumY + texture2D(depthTexture, vec2(uv.x, uv.y + pixelSize.y * -1)).r * -8;
	sumY = sumY + texture2D(depthTexture, vec2(uv.x, uv.y + pixelSize.y * 1)).r * 8;



	//////Normal Texture
	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * -1, uv.y)).r * -2;
	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * 1, uv.y)).r * 2;
	
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y * -1)).r * -2;
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y *1)).r * 2;
	
	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * -1, uv.y)).g * -2;
	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * 1, uv.y)).g * 2;
	
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y * -1)).g * -2;
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y * 1)).g * 2;
	

	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * -1, uv.y)).b * -2;
	sumX = sumX + texture2D(normalTexture, vec2(uv.x + pixelSize.x * 1, uv.y)).b * 2;
	
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y * -1)).b * -2;
	sumY = sumY + texture2D(normalTexture, vec2(uv.x, uv.y + pixelSize.y *1)).b * 2;


	float magnitude = sobelCorrection + length(vec2(sumX, sumY)) * magnitudeCorrection;
	vec3 rgbColour = texture2D(rgbTexture, uv.xy).rgb;
	
	//This is a temporary and incorrect assignment of fragColour
	FragColour.rgb = vec3(magnitude);

	//This line is the correct assignment of fragColour
	FragColour.rgb = sobelCorrection * (rgbColour * vec3(magnitude)) + (magnitudeFix * vec3(magnitude));

	FragColour.a = 1.0;
}

