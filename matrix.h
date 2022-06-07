#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
	float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float scaleX, float scaleY, float scaleZ);
mat4_t mat4_make_translation(float tX, float tY, float tZ);

mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2);
void mat4_mul_mat4_faster(mat4_t* res, mat4_t* m1, mat4_t* m2);

#endif
