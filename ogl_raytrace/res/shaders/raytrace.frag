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
	float spec;
};

struct AmbLight {
    float intensity;
    vec3 _padding[1];
};

struct PointLight {
    float intensity;
    vec3 pos;
};

struct DirLight {
    float intensity;
    vec3 dir;
};

Sphere s[NUM_SPHERES] = {
	Sphere(vec3(0, 0, 5), 0.2, vec4(0.6, 0.1, 0.1, 1), 15), 
	Sphere(vec3(0.4, 0, 5), 0.2, vec4(0.1, 0.6, 0.1, 1), 15), 
	Sphere(vec3(-0.4, 0, 5), 0.2, vec4(0.1, 0.1, 0.6, 1), 15)
};

PointLight pl = PointLight(0.4, vec3(0, 0, 2.5));

vec2 intersect_sphere(Sphere s, vec3 rayOrigin, vec3 rayDir);
vec4 ray_trace(vec3 rayDir, vec3 rayOrigin);

float len_sqr(vec3 v) { return dot(v, v); }
bool in_range(float v, float mi, float ma) { return v > mi && v < ma; }

vec2 intersect_sphere(Sphere s, vec3 rayOrigin, vec3 rayDir) {
    // assuming rayDir is already normalised
    const float k1 = 1.0f;
	const float k2 = 2 * dot(rayOrigin - s.pos, rayDir);
    const float k3 = len_sqr(rayOrigin - s.pos) - s.rad * s.rad;

    const float det = k2 * k2 - 4 * k1 * k3;
	vec2 res;

    if(det < 0) {
        // doesnt intersect
        res = vec2(INF, INF);
		return res;
    }
    
    // intersects
    res.x = (-k2 + sqrt(det)) / 2 * k1;
    res.y = (-k2 - sqrt(det)) / 2 * k1;
	return res;
}

float calc_point_light(PointLight pl, float spec, vec3 point, vec3 norm, vec3 view) {
    // assumed norm and view is normalised
    float intensity = 0.0f;

    vec3 lightDir = pl.pos - point;
    lightDir = normalize(lightDir);
    float lightDir_dot_norm = dot(lightDir, norm);
    intensity += pl.intensity * max(lightDir_dot_norm, 0);

	// Negate it, its a hack to make it work but its ok!!
    vec3 refl = -reflect(lightDir, norm);
    refl = normalize(refl); // maybe redundant?
    float refl_dot_view = dot(refl, view);

    intensity += pl.intensity * pow(max(refl_dot_view, 0), spec);
    return intensity;
}


vec4 ray_trace(vec3 rayDir, vec3 rayOrigin) {
	Sphere closest_sphere = Sphere(vec3(0, 0, 0), 0, BACKGROUND_COLOR, 0.0);
	float closest_t = INF;
	vec4 col;

	for(int i = 0;i < NUM_SPHERES;++i)
	{
		vec2 t = intersect_sphere(s[i], rayOrigin, rayDir);
		if(in_range(t[0], 0, 100) && t[0] < closest_t) {
			closest_t = t[0];
			closest_sphere = s[i];
		}
		if(in_range(t[1], 0, 100) && t[1] < closest_t) {
			closest_t = t[1];
			closest_sphere = s[i];
		}
	}
	
	col = closest_sphere.col;
	
	if(col != BACKGROUND_COLOR) {
		vec3 point = rayOrigin + rayDir * closest_t;
		vec3 norm = point - closest_sphere.pos;
		norm = normalize(norm);
		vec3 view = rayOrigin - point;
		view = normalize(view);

		float intensity = calc_point_light(pl, closest_sphere.spec, point, norm, view);
		//intensity = 0.5;
		col *= intensity;
	}

	return col;
}

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
	pl.pos = vec3(pl.pos.x, 10 * sin(5 * uTime.now), 10 * cos(5 * uTime.now));


	rayDir = normalize(rayDir);

	oColor = ray_trace(rayDir, vec3(0, 0, 0));
}