#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;

int window_width = 800;
int window_height = 800;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_info;
	SDL_GetCurrentDisplayMode(0, &display_info);

	window_width = display_info.w;
	window_height = display_info.h;
	printf("your refresh rate is: %d", display_info.refresh_rate);

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating an SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating an SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_rectangle(int x, int y, int height, int width, uint32_t color) {
	for (int row = y; row < height + y; row++) {
		for (int col = x; col < width + x; col++) {
			//color_buffer[(window_width * row) + col] = color;
			draw_pixel(col, row, color);
		}
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height)
		color_buffer[window_width * y + x] = color;
}

//DDA
void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_length; i++) {
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

//Bresenham's line drawing method, significantly faster than DDA, copied from Wikipedia
void draw_line_bresenham(int x0, int y0, int x1, int y1, uint32_t color) {
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int error = dx + dy;

	int e2;
	while (true) {
		//draw_pixel(x0, y0, color);

		//draw_pixel function body copied here to avoid sending the same color value
		//every time for efficiency purposes
		if (x0 >= 0 && x0 < window_width && y0 >= 0 && y0 < window_height)
			color_buffer[window_width * y0 + x0] = color;

		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = 2 * error;
		if (e2 >= dy) {
			if (x0 == x1) {
				break;
			}
			error = error + dy;
			x0 = x0 + sx;
		}
		if (e2 <= dx) {
			if (y0 == y1) {
				break;
			}
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

//for flat top flat bottom triangle rasterization
void draw_horizontal_line(int x0, int y0, int x1, uint32_t color) {
	if (x0 < x1) {
		for (x0; x0 <= x1; ++x0) {
			if (x0 >= 0 && x0 < window_width && y0 >= 0 && y0 < window_height)
				color_buffer[window_width * y0 + x0] = color;
		}
	}
	else {
		for (x1; x1 <= x0; ++x1) {
			if (x1 >= 0 && x1 < window_width && y0 >= 0 && y0 < window_height)
				color_buffer[window_width * y0 + x1] = color;
		}
	}
}

void draw_vertical_line(int x0, int y0, int y1, uint32_t color) {
	if (y0 < y1) {
		for (y0; y0 <= y1; ++y0) {
			draw_pixel(x0, y0, color);
		}
	}
	else {
		for (y1; y1 <= y0; ++y1) {
			draw_pixel(x0, y1, color);
		}
	}
}

void draw_diagonal_line(int x0, int y0, int x1, int y1, uint32_t color) {
	if (y0 < y1) {
		for (y0; y0 <= y1; ++y0) {
			draw_pixel(x0, y0, color);
		}
	}
	else {
		for (y1; y1 <= y0; ++y1) {
			draw_pixel(x0, y1, color);
		}
	}
}

//TODO: 
//draw_vertical_line
//draw_diagonal_line



void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	draw_pixel(x0, y0, color);
	draw_pixel(x1, y1, color);
	draw_pixel(x2, y2, color);
	draw_line_bresenham(x0, y0, x1, y1, color);
	draw_line_bresenham(x1, y1, x2, y2, color);
	draw_line_bresenham(x2, y2, x0, y0, color);
}


void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
	for (int row = 0; row < window_height; row++) {
		for (int col = 0; col < window_width; col++) {
			color_buffer[(window_width * row) + col] = color;
		}
	}
}

void destroy_window(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}