
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "seal/window.h"

extern void Seal_InitializeInputBuffering(GLFWwindow *window);

struct Seal_Window {
	GLFWwindow *window;
	Seal_Size x, y;
	Seal_Size width;
	Seal_Size height;
	Seal_Bool isFullscreen;
};

Seal_Window *Seal_CreateWindow(Seal_Size w, Seal_Size h, const char *title, Seal_WindowOptions *opts) {
	static Seal_WindowOptions default_options = {
		.scalable = SEAL_FALSE,
		.fullscreen = SEAL_FALSE,
		.decorated = SEAL_TRUE,
		.centered = SEAL_TRUE,
		.show = SEAL_TRUE
	};
	static Seal_Bool glewInitilized = SEAL_FALSE;

	opts = opts ? opts : &default_options;

	glfwWindowHint(GLFW_DECORATED, opts->decorated);
	glfwWindowHint(GLFW_RESIZABLE, opts->scalable);
	Seal_Window *win = malloc(sizeof(Seal_Window));

	GLFWmonitor *monitor =	glfwGetPrimaryMonitor();
	const GLFWvidmode *video = glfwGetVideoMode(monitor);

	win->window = glfwCreateWindow(w, h, title, opts->fullscreen ? monitor : NULL, NULL);
	win->isFullscreen = opts->fullscreen;
	
	glfwSetWindowSizeLimits(win->window, 1, 1, video->width - 1, video->height - 1);
	if (opts->show) glfwShowWindow(win->window);
	if (opts->centered) {
		Seal_Size x = (video->width - w)>> 1, y = (video->height - h) >> 1;
		glfwSetWindowPos(win->window, x, y);
	}

	glfwGetWindowSize(win->window, &win->width, &win->height);
	glfwGetWindowPos(win->window, &win->x, &win->y);

	glfwMakeContextCurrent(win->window);
	if(!glewInitilized) {
		Seal_Bool glewSuccessful = glewInit() != GLEW_OK ? SEAL_FALSE : SEAL_TRUE;
		if (!glewSuccessful) Seal_LogError("Failed to initialize glew: %s", SEAL_TRUE, gluErrorString(glGetError()));
	}

	glViewport(0, 0, win->width, win->height);
	Seal_InitializeInputBuffering(win->window);
	
	return win;
}

void Seal_DestroyWindow(Seal_Window *window) {
	if(window) {
		glfwDestroyWindow(window->window);
		free(window);
	}
}

Seal_Bool Seal_WindowContinue(Seal_Window *window) {
	if (!window)
		return SEAL_FALSE;
	
	Seal_Bool shouldQuit = glfwWindowShouldClose(window->window);
	glfwPollEvents();
	return !shouldQuit;
}

void Seal_WindowSwapBuffers(Seal_Window *window) {
	glfwSwapBuffers(window->window);
}

inline Seal_Bool Seal_GetWindowFullscreen(Seal_Window *window) {
	return window->isFullscreen;
}

void Seal_SetWindowFullscreen(Seal_Window *window, Seal_Bool fullscreen) {
	if (window->isFullscreen == fullscreen) return;

	if (fullscreen) {
		glfwGetWindowSize(window->window, &window->width, &window->height);
		glfwGetWindowPos(window->window, &window->x, &window->y);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(window->window, monitor, 0, 0, window->width, window->height, 0);
	}else {
		glfwSetWindowMonitor(window->window, NULL, window->x, window->y, window->width, window->height, 0);
	}
	window->isFullscreen = fullscreen;
}