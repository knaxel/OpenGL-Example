#version 330

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct TexturePack{
	sampler2D diffuse_dirt;
	sampler2D diffuse_grass;
	sampler2D diffuse_stone;
	sampler2D diffuse_sand;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in float visibility;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform TexturePack textures; 
uniform PointLight pointLight;
uniform vec3 cameraPos;

//Functions 
vec3 calculateAmbient(Material material)
{
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0-vs_position);
	float diffuse = clamp(dot(normalize(vs_normal), posToLightDirVec), 0, 1.f );
	vec3 diffuseFinal = material.diffuse * diffuse;

	float darkness = 1;
	if(vs_position.y <= 0){
		darkness = 1-atan(-(vs_position.y+15)/2000); 
	}
	return darkness*diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant * texture(textures.diffuse_dirt, vs_texcoord).rgb; // SPECULAR TEXTURE O
	//vec3 specularFinal = material.specular * specularConstant;

	float darkness = 1; 
	if(vs_position.y <= 0){
		darkness = 0; 
	}
	return darkness*specularFinal;
}
vec4 calcTexture(Material material){
	vec3 grass = (dot(normalize(vs_normal), vec3(0,1,0))) *texture(textures.diffuse_grass, vs_texcoord ).rgb;
	vec3 dirt = (1-(dot(normalize(vs_normal), vec3(0,1,0)))) *texture(textures.diffuse_dirt, vs_texcoord ).rgb;
	float sandAmount= 0;
	vec3 sandColor = vec3(0);
	float darkness = 1;
	if(vs_position.y <= 0){
		sandAmount = clamp(atan(-(vs_position.y)/25), 0.f,1.f);
		sandColor = sandAmount*texture(textures.diffuse_sand, vs_texcoord).rgb; 

		darkness = clamp(1-atan(-(vs_position.y+15)/200),.2f,1.f); 
	}
	float stoneAmount= 0;
	vec3 stoneColor = vec3(0);
	if(vs_position.y >= 300){
		stoneAmount = clamp(atan((vs_position.y-400)/25), 0.f,1.f);
		stoneColor = stoneAmount*texture(textures.diffuse_stone, vs_texcoord).rgb; 

	}
	vec3 applied_sand = (grass+dirt)*(1-sandAmount)+darkness*sandColor;
	vec3 applied_stone =applied_sand*(1-stoneAmount) + stoneColor;
	return vec4(applied_stone,1);


}
void main()
{
	//Ambient light
	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);

	//Specular light
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, cameraPos);
	
	vec4 fakewater = vec4(0);

	if(vs_position.y <= -30 ){
		float blueness = clamp(1-atan(-(vs_position.y+15)/200),.005f,.05f); 
		fakewater = vec4(blueness,blueness, -blueness ,0);

	}

	fs_color = calcTexture(material) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f))-fakewater;
	//fs_color =  vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f)  + vec4(specularFinal, 1.f);
	vec4 fogColor = vec4(186.f/255, 232.f/255, 255.f/255,1);
	fs_color = mix(fogColor,fs_color,visibility);
}
