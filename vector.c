#include <math.h>
#include "vector.h"

float vec2_length(vec2_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

float vec3_length(vec3_t v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
	vec2_t result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
	return result;
}

vec3_t vec3_add(vec3_t v1, vec3_t v2) {
	vec3_t result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {
	vec2_t result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
	return result;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2) {
	vec3_t result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	return result;
}

vec2_t vec2_mul(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x * factor,
		.y = v.y * factor
	};
	return result;
}

vec3_t vec3_mul(vec3_t v, float factor) {
	vec3_t result = {
		.x = v.x * factor,
		.y = v.y * factor,
		.z = v.z * factor
	};
	return result;
}

vec2_t vec2_div(vec2_t v, float factor) {
	vec2_t result = {
		.x = v.x / factor,
		.y = v.y / factor
	};
	return result;
}
vec3_t vec3_div(vec3_t v, float factor) {
	vec3_t result = {
		.x = v.x / factor,
		.y = v.y / factor,
		.z = v.z / factor
	};
	return result;
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2) {
	vec3_t result = {
		.x = v1.y * v2.z - v1.z * v2.y,
		.y = v1.z * v2.x - v1.x * v2.z,
		.z = v1.x * v2.y - v1.y * v2.x
	};
	return result;
}

float vec2_dot(vec2_t v1, vec2_t v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float vec3_dot(vec3_t v1, vec3_t v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

float vec4_dot(vec4_t v1, vec4_t v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}

void vec2_normalize(vec2_t* v) {
	float length = vec2_length(*v);
	v->x /= length;
	v->y /= length;
}

void vec3_normalize(vec3_t* v) {
	float length = vec3_length(*v);
	v->x /= length;
	v->y /= length;
	v->z /= length;
}

void vec3_rotate_x(vec3_t* v, float angle) {
	float original_y = v->y;
	double sine = sin(angle);
	double cosine = cos(angle);
	v->y = (original_y * cosine) - ((v->z) * sine);
	v->z = (original_y * sine) + ((v->z) * cosine);
}

void vec3_rotate_y(vec3_t* v, float angle) {
	float original_x = v->x;
	double sine = sin(angle);
	double cosine = cos(angle);
	v->x = (original_x * cosine) - ((v->z) * sine);
	v->z = (original_x * sine) + ((v->z) * cosine);
}

void vec3_rotate_z(vec3_t* v, float angle) {
	float original_x = v->x;
	double sine = sin(angle);
	double cosine = cos(angle);
	v->x = (original_x * cosine) - ((v->y) * sine);
	v->y = (original_x * sine) + ((v->y) * cosine);
}

vec3_t vec3_from_vec4(vec4_t v) {
	vec3_t m = {
		v.x,
		v.y,
		v.z
	};
	return m;
}

vec4_t vec4_from_vec3(vec3_t v) {
	vec4_t m = {
		v.x,
		v.y,
		v.z,
		1.0
	};
	return m;
}