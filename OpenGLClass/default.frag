#version 330 core

// Outputs
out vec4 FragColor;

// Inputs (Note that these are in order of vertex | POS | NORM | COLOR | TEXUV )
in vec3 currentPos;
in vec3 normal;
in vec3 color;
in vec2 texturePos;

// Uniform
uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform int lightOption;

float blinnPhongSpecular(float specularMultiplier, float diffuse, vec3 cameraDirection, vec3 norm, vec3 lightDirection){
	if(diffuse == 0.0f)
		return 0.0f;
	vec3 middle = normalize(cameraDirection + lightDirection);
	return pow(max(dot(norm, middle), 0.0f), 16) * specularMultiplier;
}


float phongSpecular(float specularMultiplier, vec3 cameraDirection, vec3 reflectDirection){
	return pow(max(dot(cameraDirection, reflectDirection), 0.0f), 16) * specularMultiplier;
}

// Point Light 
// input : ambient factor, specular factor, a and b for intensity
// attenuation = 1/a*d^2 + b*d + c (where d = distance of light)
vec4 point(float ambience, float specularMultiplier, float a, float b, float c){
	// get vector of light relative to current position
	vec3 light = lightPos - currentPos;

	// normalize
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(light);
	vec3 cameraDirection = normalize(cameraPos - currentPos);

	// diffuse
	float diffuse = dot(norm, lightDirection);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specular = blinnPhongSpecular(specularMultiplier, diffuse, cameraDirection, norm, lightDirection);

	// intensity (attenuation)
	float distance = length(light);
	float intensity = 1.0f / (a * distance * distance + b * distance + c);

	// result
	/*                   default texture				diffuse + ambient light        specular light (RED CH)     */
	vec4 result = texture(diffuse0, texturePos) * (diffuse * intensity + ambience) + texture(specular0, texturePos).r * specular * intensity;
	return result * lightColor;
}

// Directional Light
// input : ambient factor, specular factor, and vec3 of where the directional light should be (default should be up)
vec4 directional(float ambience, float specularMultiplier, vec3 direction){
	// get vector of light relative to current position
	vec3 light = lightPos - currentPos;

	// normalize
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(direction);
	vec3 cameraDirection = normalize(cameraPos - currentPos);

	// diffuse
	float diffuse = dot(norm, lightDirection);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specular = blinnPhongSpecular(specularMultiplier, diffuse, cameraDirection, norm, lightDirection);

	// result
	/*                   default texture		diffuse + ambient light        specular light (RED CH)     */
	vec4 result = texture(diffuse0, texturePos) * (diffuse + ambience) + texture(specular0, texturePos).r * specular;
	return result * lightColor;
}

// Spot light
// Directional Light
// input : ambient factor, specular factor, epsilon, phi, direction of spot light
vec4 spot(float ambience, float specularMultiplier, float gamma, float phi, vec3 direction){

	// change gamma and phi from degree to radians
	gamma = cos(gamma);
	phi = cos(phi);

	// get vector of light relative to current position
	vec3 light = lightPos - currentPos;

	// normalize
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(light);
	vec3 cameraDirection = normalize(cameraPos - currentPos);

	// diffuse
	float diffuse = dot(norm, lightDirection);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specular = blinnPhongSpecular(specularMultiplier, diffuse, cameraDirection, norm, lightDirection);

	// epsilon = phi - gamma
	float epsilon = phi - gamma;
	// intensity = (theta - gamma) / epsilon
	float theta = dot(direction, -lightDirection); 
	float intensity = clamp((theta - gamma) / epsilon, 0.0f, 1.0f);

	// result
	/*                   default texture		diffuse + ambient light        specular light (RED CH)     */
	vec4 result = texture(diffuse0, texturePos) * (diffuse * intensity + ambience) + texture(specular0, texturePos).r * specular * intensity;
	return result * lightColor;
}

// test light
vec4 test(){
	vec3 light = lightPos - currentPos;

	// normalize
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(light);
	vec3 cameraDirection = normalize(cameraPos - currentPos);

	// diffuse
	float diffuse = dot(norm, lightDirection);

	// specular
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specular = pow(max(dot(cameraDirection, reflectDirection), 0.0f), 16) * 0.1f;


	vec4 result = texture(diffuse0, texturePos) * diffuse;
	return result;
}

float linearizeDepth(float depth, float near, float far){
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logDepth(float depth, float steepness, float offset, float near, float far){
	float z = linearizeDepth(depth, near, far);
	return (1 / (1 + exp(-steepness * (z - offset))));
}

void main(){
	switch(lightOption){
		case 0:
			FragColor = point(0.2f, 0.5f, 1.0f, 0.2f, 1.0f);
			break;
		case 1:
			FragColor = directional(0.2f, 0.5f, vec3(1.0f, 1.0f, 0.0f));
			break;
		case 2:
			FragColor = spot(0.2f, 0.5f, 19.5f, 12.5f, vec3(0.0f, -1.0f, 0.0f));
			break;
		default:
			FragColor = point(0.2f, 0.5f, 1.0f, 0.2f, 1.0f);
			break;
	}
	float depth = logDepth(gl_FragCoord.z, 0.01f, 250.0f, 0.1f, 500.0f);
	FragColor = FragColor * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.9f), 1.0f);
}