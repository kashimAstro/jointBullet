#version 120
varying vec3 normal;
uniform vec4 colorAdd;

void main(void) {
	vec3 lightDir = vec3(1.0,1.0,1.0);
	vec4 ambientColor = vec4(0.4, 0.4, 0.4, 1.0);
	vec4 diffuseColor = vec4(0.9, 0.9, 0.9, 1.0);
	vec4 specularColor = vec4(0.5, 0.5, 0.5, 1.0);

	vec3 normal = normalize(gl_NormalMatrix * normal).xyz;
	float diffuse = max(0.0, dot(normalize(normal), normalize(lightDir)));
	float diffuse2 = max(0.0, dot(-normalize(normal), normalize(lightDir)));
	vec4 flagColor = diffuse * diffuseColor + diffuse2 * diffuseColor;

	flagColor += ambientColor;
	vec3 vReflection = normalize(reflect(-normalize(lightDir), normalize(normal)));
	float specular = pow(max(0.0, dot(normalize(normal), vReflection)), 128.0);
	flagColor += specular * specularColor;
	gl_FragColor = flagColor;
}
