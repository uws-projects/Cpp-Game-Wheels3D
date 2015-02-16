// difficult parallax fragment shader :)
// Parallax shader that calculates the position of the 
// point found at the intersection of the camera direction and heightmap
// that point`s coordinates will be used to render the colour from the colourMap

#version 330

// Some drivers require the following
precision highp float;

struct Light
{
	vec4 La;
	vec4 Ld;
	vec4 Ls;
	vec4 position;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	vec4 Ka;
	vec4 Kd;
	vec4 Ks;
	float shininess;
};

uniform Light light[3];
uniform Material material;

uniform sampler2D colourMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

uniform float parallaxScale;
uniform float parallaxOffset;
uniform bool parallaxEnabled;

in vec2 originalCoordinates;
in float lightRayLength[2];

in vec3 cameraTangent;
in vec3 lightTan[2];

float parallaxHeight;
float numberOfSegments;

layout(location = 0) out vec4 finalColour;


vec2 ParallaxCalculation(vec3 V)
{
	// we`ll set about 20 segments so we can have each segment
	// with a height of 0.05, which is less noticeable when applying parallax
	// and chances are that less aliasing effects will occur
	// so the division of the coordinates will be smoother
	// if the depth is high, the segments will become visible
	numberOfSegments = 20;

	// find the height of one subdivision
	float segmentHeight = 1.0/numberOfSegments;
	
	// start from 0 with default coordinates
	float actualSegmentHeight = 0;
	vec2 modifiedTextureCoordinates = originalCoordinates;
	
	// get the displacement value for each segment
	vec2 textureDifference = (parallaxScale * V.xy) / numberOfSegments;
	
	// get the height from the heightmap
	float heightFromTexture = texture2D(heightMap, originalCoordinates).r;

	// apply parallax for each segment while we`re not lower than the segment`s height
	// basically apply parallax while possible
	// actualSegmentHeight will increase with each application of the displacement
	// while the modifiedTextureCoordinates will symmetrically shrink

	while (heightFromTexture > actualSegmentHeight)
	{
		actualSegmentHeight += segmentHeight;
		modifiedTextureCoordinates -=textureDifference;
		heightFromTexture = texture(heightMap, modifiedTextureCoordinates).r;
	}

	// get the last used coordinates to create last step`s height
	// get the next step`s height
	// calculate the difference
	// and find what proportion of the segment is the ... how to call it..
	// factor of deepness... if it were positive it would have been bold effect

	vec2 lastCoordinates = modifiedTextureCoordinates - textureDifference;
	
	float nextSegmentHeight = heightFromTexture - actualSegmentHeight;
	float prevSegmentHeight = texture(heightMap, lastCoordinates).r - actualSegmentHeight + segmentHeight;
	float difference = nextSegmentHeight - prevSegmentHeight;

	float bold = nextSegmentHeight / difference;
	
	parallaxHeight	= actualSegmentHeight + prevSegmentHeight * bold 
					+ nextSegmentHeight * (1.0 - bold);
	parallaxHeight -= 0.5;
	// once we got the "bold" effect we apply it to last height, and for the rest
	// of the texture coordinates we apply the unafected area by bold
	return lastCoordinates * bold + modifiedTextureCoordinates * (1.0 - bold);
}

float ShadowCalculation(vec3 lightPosition) 
{
	
	float shadowResult = 1.0;

	if (dot(vec3(0.0, 0.0, 1.0), lightPosition) > 0)
	{
		shadowResult = 0.0;
		float samples = 0;
		float segmentHeight = parallaxHeight / numberOfSegments;
		vec2 shadowDifference = parallaxScale * lightPosition.xy / lightPosition.z /numberOfSegments;

		float actualSegmentHeight = parallaxHeight - segmentHeight;
		vec2 modifiedTextureCoordinates = originalCoordinates + shadowDifference;
		float heightFromTexture = texture(heightMap, modifiedTextureCoordinates).r;

		int segmentIndex = 1;

		while(actualSegmentHeight > 0)
		{
			if (heightFromTexture < actualSegmentHeight)
			{
				samples += 1;
				float currentShadowHeight = (actualSegmentHeight - heightFromTexture) * (1.0 - segmentIndex/numberOfSegments);
				shadowResult = max(shadowResult, currentShadowHeight);
			}

			segmentIndex++;
			actualSegmentHeight -= segmentHeight;
			modifiedTextureCoordinates -= shadowDifference;
			heightFromTexture = texture(heightMap, modifiedTextureCoordinates).r;
		}

		if (samples < 1) shadowResult = 1.0;
		else shadowResult = 1.0 - shadowResult;

	}
	return shadowResult;
}

vec4 LightCalculation(Light light, vec2 T, vec3 lightTangent, float distance)
{	
	float attConst = light.constant;
	float attLinear = light.linear;
	float attQuadratic = light.quadratic;
	
	// original intensity
	vec4 ambientI = light.La * material.Ka;
	vec4 diffuseI = light.Ld * material.Kd;
	vec4 specularI = light.Ls * material.Ks;

	// recalculated intensity based on normal direction
	// get the normal from the normal map and clamp it in -1..1 interval
	
	vec3 N = normalize( (texture( normalMap, T).rgb-0.5) * 2 );
	
	diffuseI = diffuseI * max(dot(N,normalize(lightTangent)),0);
	diffuseI *= texture(colourMap, T);

	vec3 R = normalize(reflect(-lightTangent,N));
	specularI = specularI * pow(max(dot(R,cameraTangent),0), material.shininess);

	float attenuation = 
	attConst		+ 
	attLinear		*	distance	+ 
	attQuadratic	*	distance	* distance;
	
	// this is to avoid display errors if attenuation is not sent to GPU
	if (attenuation == 0) attenuation = 1.0;
	
	float shadow =1.005;//ShadowCalculation(light.position);

	return vec4((diffuseI* 2.2).rgb, 1.0f );
}


void main(void) {
	
	// values passed from vertex shader are interpolated, so need to normalize
	vec3 viewVector = normalize(cameraTangent);
	vec2 modifiedCoordinates = originalCoordinates;
	
	//if (parallaxEnabled)	modifiedCoordinates = ParallaxCalculation(viewVector);
	
	vec4 lightResult = vec4(0.0f);
	int i = 0;
	
	// comment the for line to add only one light
	for ( ; i < 4; i++) 
	{
	lightResult+= LightCalculation(light[i], modifiedCoordinates, lightTan[i], lightRayLength[i]);
	}
	finalColour = lightResult;
	
}

