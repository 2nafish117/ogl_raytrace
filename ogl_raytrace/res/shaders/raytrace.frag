#version 430 core

#define NUM_SPHERES			3
#define INF					100000000.0f
#define BACKGROUND_COLOR	vec4(0.1, 0.1, 0.1, 1)
#define PROJ_DIST			1.5

struct Time { 
	float now, dt;
};

struct Sphere {
	vec3 pos;
	float rad;
	vec4 col;
};

Sphere s[NUM_SPHERES] = {
	Sphere(vec3(0, 0, 5), 0.2, vec4(0.6, 0.1, 0.1, 1)), 
	Sphere(vec3(0.4, 0, 5), 0.2, vec4(0.1, 0.6, 0.1, 1)), 
	Sphere(vec3(-0.4, 0, 5), 0.2, vec4(0.1, 0.1, 0.6, 1))
};

vec2 intersect_sphere(Sphere s, vec3 rayOrigin, vec3 rayDir);
vec4 ray_trace(vec3 rayDir, vec3 rayOrigin);

float len_sqr(vec3 v) { return dot(v, v); }
float smallest_positive(float f1, float f2) { return min(min(f1, 0), min(f2, 0)); }
bool in_range(float v, float mi, float ma) { return v > mi && v < ma; }

vec2 intersect_sphere(Sphere s, vec3 rayOrigin, vec3 rayDir)
{
    // assuming rayDir is already normalised
    const float k1 = 1.0f;
	const float k2 = 2 * dot(rayOrigin - s.pos, rayDir);
    const float k3 = len_sqr(rayOrigin - s.pos) - s.rad * s.rad;

    const float det = k2 * k2 - 4 * k1 * k3;
	vec2 res;

    if(det < 0)
    {
        // doesnt intersect
        res = vec2(INF, INF);
		return res;
    }
    
    // intersects
    res.x = (-k2 + sqrt(det)) / 2 * k1;
    res.y = (-k2 - sqrt(det)) / 2 * k1;
	return res;
}

vec4 ray_trace(vec3 rayDir, vec3 rayOrigin) {
	Sphere closest_sphere = Sphere(vec3(0, 0, 0), 0, BACKGROUND_COLOR);
	float closest_t = INF;
	vec4 col;

	for(int i = 0;i < NUM_SPHERES;++i)
	{
		vec2 t = intersect_sphere(s[i], rayOrigin, rayDir);
		if(in_range(t[0], 0, 100) && t[0] < closest_t)
		{
			closest_t = t[0];
			closest_sphere = s[i];
		}
		if(in_range(t[1], 0, 100) && t[1] < closest_t)
		{
			closest_t = t[1];
			closest_sphere = s[i];
		}
	}
	
	col = closest_sphere.col;

	return col;
}

// Input

// Output
out vec4 oColor;

// Uniforms
layout(location = 3) uniform float uWindowWidth;
layout(location = 4) uniform float uWindowHeight;
layout(location = 5) uniform Time uTime;

void main()
{
	vec3 uv = vec3(gl_FragCoord.x / uWindowWidth, gl_FragCoord.y / uWindowHeight, 0) - vec3(0.5, 0.5, 0);
	vec3 rayDir = uv + vec3(0, 0, PROJ_DIST);

	//s[0].pos = vec3(0.9 * sin(uTime.now), 0.9 * cos(uTime.now), s[0].pos.z);

	rayDir = normalize(rayDir);

	oColor = ray_trace(rayDir, vec3(0, 0, 0));
}