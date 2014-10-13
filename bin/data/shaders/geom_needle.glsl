#version 120
#extension GL_EXT_geometry_shader4 : enable

varying vec3 normal;

//--------------------------------------------------------------
vec3 quaternionRotation(vec3 original, float angle, vec3 axis) {
    normalize(axis);
    float hcos = cos(angle / 2);
    float hsin = sin(angle / 2);
    float q10 = hcos;
    float q11 = axis.x * hsin;
    float q12 = axis.y * hsin;
    float q13 = axis.z * hsin;
    
    float q20 = hcos;
    float q21 = -axis.x * hsin;
    float q22 = -axis.y * hsin;
    float q23 = -axis.z * hsin;

    float qo0 = 0;
    float qo1 = original.x;
    float qo2 = original.y;
    float qo3 = original.z;
    
    float qt0 = q10 * qo0 - q11 * qo1 - q12 * qo2 - q13 * qo3;
    float qt1 = q10 * qo1 + q11 * qo0 + q12 * qo3 - q13 * qo2;
    float qt2 = q10 * qo2 - q11 * qo3 + q12 * qo0 + q13 * qo1;
    float qt3 = q10 * qo3 + q11 * qo2 - q12 * qo1 + q13 * qo0;
    
    float qe0 = qt0 * q20 - qt1 * q21 - qt2 * q22 - qt3 * q23;
    float qe1 = qt0 * q21 + qt1 * q20 + qt2 * q23 - qt3 * q22;
    float qe2 = qt0 * q22 - qt1 * q23 + qt2 * q20 + qt3 * q21;
    float qe3 = qt0 * q23 + qt1 * q22 - qt2 * q21 + qt3 * q20;
    
    return vec3(qe1, qe2, qe3);
}
//--------------------------------------------------------------

uniform float radius;

void main() {
	float PI = 3.14159265359;
	float TWO_PI =  PI * 2;

	vec3 p0 = gl_PositionIn[0].xyz;
	vec3 p1 = gl_PositionIn[1].xyz;
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector
	
	vec3 dir = normalize(p1 - p0);			// normalized direction vector from p0 to p1
	vec3 right = normalize(cross(dir, up));	// right vector
	vec3 norm = cross(right, dir);
	vec3 center = (p0 +p1) / 2.0;
	float len = distance(p0, p1);
	
	//float radius = 0.5 - abs(10.0 - len) * 0.1;
	if (len > 190.0) {
		len = (20.0 - len) / 5.0 * 5.0;
	}
	int hslices = 5; 
	int vslices = 4; 
	
	vec3 d, n;
	for (int i = 0; i < hslices; i ++) {
		float vTheta0 = PI / hslices * i - PI / 2;
		float cosV0 = cos(vTheta0);
		float sinV0 = sin(vTheta0);
		float vTheta1 = PI / hslices * (i + 1) - PI / 2;
		float cosV1 = cos(vTheta1);
		float sinV1 = sin(vTheta1);
		
		vec3 c0 = center + dir * len / 2 * sinV0;
		vec3 c1 = center + dir * len / 2 * sinV1;
		vec3 r0 = right * cosV0;
		vec3 r1 = right * cosV1;
	 
		for (int j = 0; j <= vslices; j ++) {
			d = quaternionRotation(r0, TWO_PI * j / vslices, dir);
			gl_Position = gl_ModelViewProjectionMatrix * vec4(c0 + d * radius, 1.0);
			normal = normalize(d + normalize(c0) / 2.0);
			EmitVertex();
			
			d = quaternionRotation(r1, TWO_PI * j / vslices, dir);
			gl_Position = gl_ModelViewProjectionMatrix * vec4(c1 + d * radius, 1.0);
			normal = normalize(d + normalize(c1) / 2.0);
			EmitVertex();
			
		}
	}
}
