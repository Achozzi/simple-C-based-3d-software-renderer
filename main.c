#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };

float fov_factor = 640;

bool is_running = false;

int display_mode = 2; //default 2
int backface_culling_mode = 1; //default 1 (enabled)

bool setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	if (!color_buffer) {
		fprintf(stderr, "Error creating the color buffer. Probably not enough avaliable memory.\n");
		return false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	load_cube_mesh_data();
	//char* filename = "assets\\cube.obj";
	//load_obj_file_data(filename);

	return true;
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			else if (event.key.keysym.sym == SDLK_1) {
				display_mode = 1;
			}
			else if (event.key.keysym.sym == SDLK_2) {
				display_mode = 2;
			}
			else if (event.key.keysym.sym == SDLK_3) {
				display_mode = 3;
			}
			else if (event.key.keysym.sym == SDLK_4) {
				display_mode = 4;
			}
			else if (event.key.keysym.sym == SDLK_c) {
				backface_culling_mode = 1;
			}
			else if (event.key.keysym.sym == SDLK_d) {
				backface_culling_mode = 0;
			}
			break;
	}
}

vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = fov_factor * point.x / point.z,
		.y = fov_factor * point.y / point.z
	};
	return projected_point;
}

void update(void) {
	SDL_Delay(16); // 30 ms

	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	//loop all triangle faces of cube mesh
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec3_t transformed_vertices[3];
		 //loop all three vertices of this face and apply transforms
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			vec3_rotate_x(&transformed_vertex, mesh.rotation.x);
			vec3_rotate_y(&transformed_vertex, mesh.rotation.y);
			vec3_rotate_z(&transformed_vertex, mesh.rotation.z);

			//translate the vertex array from the camera
			transformed_vertex.z += 5;

			transformed_vertices[j] = transformed_vertex;
		}

		//backface culling
		if (backface_culling_mode) {
			vec3_t vector_a = transformed_vertices[0];
			vec3_t vector_b = transformed_vertices[1];
			vec3_t vector_c = transformed_vertices[2];

			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);

			vec3_t camera_ray = vec3_sub(camera_position, vector_a);
			float dot_normal_camera = vec3_dot(normal, camera_ray);

			if (dot_normal_camera < 0) {
				continue;
			}
		}

		vec2_t projected_points[3];
		for (int j = 0; j < 3; j++) {
			//project the current vertex
			projected_points[j] = project(transformed_vertices[j]);

			//scale and translate projected points to middle of screen
			projected_points[j].x += (window_width / 2);
			projected_points[j].y += (window_height / 2);

		}
		float avg_depth = (transformed_vertices[0].z +
			transformed_vertices[1].z +
			transformed_vertices[2].z) / 3.0;

		triangle_t projected_triangle = {
			.points = {
				{projected_points[0].x, projected_points[0].y},
				{projected_points[1].x, projected_points[1].y},
				{projected_points[2].x, projected_points[2].y}
			 },
			.color = mesh_face.color,
			.avg_depth = avg_depth
		};
		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

	// sort triangles by depth (bubble sort)
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {
		for (int j = i; j < num_triangles; j++) {
			if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}
}

void render() {
	
	int num_triangles = array_length(triangles_to_render);

	for (int i = 0; i < num_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];

		if (display_mode == 3) {
			draw_filled_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				triangle.color);
		}
		else if (display_mode == 2) {
			draw_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				0xFFFFFFFF);
		}
		else if (display_mode == 1) {
			draw_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				0xFFFFFFFF);

			draw_rectangle(triangle.points[0].x, triangle.points[0].y, 6, 6, 0xFFFF0000);
			draw_rectangle(triangle.points[1].x, triangle.points[1].y, 6, 6, 0xFFFF0000);
			draw_rectangle(triangle.points[2].x, triangle.points[2].y, 6, 6, 0xFFFF0000);
		}
		else if (display_mode == 4) {
			draw_filled_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				0xFFFFFFFF);
			draw_triangle(
				triangle.points[0].x,
				triangle.points[0].y,
				triangle.points[1].x,
				triangle.points[1].y,
				triangle.points[2].x,
				triangle.points[2].y,
				0xFFFF0000);
		}
	}

	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);

}

void free_resources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}

int main(int argc, char* args[]) {

	is_running = initialize_window();

	vec3_t myVec = { 2, 4, 6 };

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}