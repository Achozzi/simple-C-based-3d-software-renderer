#include <math.h>
#include "matrix.h"
#include "vector.h"

mat4_t mat4_identity(void) {
	mat4_t identity_matrix = {{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	}};

	return identity_matrix;
}

mat4_t mat4_make_scale(float scaleX, float scaleY, float scaleZ) {
	mat4_t m = mat4_identity();
	m.m[0][0] = scaleX;
	m.m[1][1] = scaleY;
	m.m[2][2] = scaleZ;
	return m;
}

mat4_t mat4_make_translation(float tX, float tY, float tZ) {
	mat4_t m = mat4_identity();
	m.m[0][3] = tX;
	m.m[1][3] = tY;
	m.m[2][3] = tZ;
	return m;
}

mat4_t mat4_make_rotation_x(float angle) {
	float cosine = cos(angle);
	float sine = sin(angle);
	mat4_t m = mat4_identity();
	m.m[1][1] = cosine;
	m.m[1][2] = -sine;
	m.m[2][1] = sine;
	m.m[2][2] = cosine;
	return m;
}

mat4_t mat4_make_rotation_y(float angle) {
	float cosine = cos(angle);
	float sine = sin(angle);
	mat4_t m = mat4_identity();
	m.m[0][0] = cosine;
	m.m[0][2] = sine;
	m.m[2][0] = -sine;
	m.m[2][2] = cosine;
	return m;
}

mat4_t mat4_make_rotation_z(float angle) {
	float cosine = cos(angle);
	float sine = sin(angle);
	mat4_t m = mat4_identity();
	m.m[0][0] = cosine;
	m.m[0][1] = -sine;
	m.m[1][0] = sine;
	m.m[1][1] = cosine;
	return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
	mat4_t m = { { { 0 } } };
	double tan_factor = 1 / tan(fov / 2);
	m.m[0][0] = aspect * tan_factor;
	m.m[1][1] = tan_factor;
	m.m[2][2] = zfar / (zfar - znear);
	m.m[2][3] = (-zfar * znear) / (zfar - znear);
	m.m[3][2] = 1.0;
	return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
	vec4_t result = mat4_mul_vec4(mat_proj, v);
	if (result.w != 0.0) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
	vec4_t result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
	return result;
}

mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2) {
	mat4_t m;
	
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			m.m[row][col] = 
				  m1.m[row][0] * m2.m[0][col] 
				+ m1.m[row][1] * m2.m[1][col] 
				+ m1.m[row][2] * m2.m[2][col] 
				+ m1.m[row][3] * m2.m[3][col];
		}
	}

	return m;
}

void mat4_mul_mat4_faster(mat4_t* res, mat4_t* m1, mat4_t* m2) {
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			res->m[row][col] =
				  m1->m[row][0] * m2->m[0][col]
				+ m1->m[row][1] * m2->m[1][col]
				+ m1->m[row][2] * m2->m[2][col]
				+ m1->m[row][3] * m2->m[3][col];
		}
	}
}