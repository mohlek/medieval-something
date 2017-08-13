#include "Window.h"
#include <exception>

#include <string>
#include <iostream>
#include <signal.h>

using namespace Engine;

Window::Window() {
    if (!glfwInit()) {
        fprintf(stderr, "Error: could not init GLFW\n");
        return;
    }
    
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    this->monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        throw "Could not get primary monitor";
    }
    
    this->mode = glfwGetVideoMode(this->monitor);
    
    if (!this->mode) {
        throw "Could not get videomode";
    }
    
    glfwWindowHint(GLFW_RED_BITS, this->mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, this->mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, this->mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, this->mode->refreshRate);
}

void Window::create() {
    
    if (!monitor) {
        fprintf(stderr, "monitor not initialized");
    }

    int width = this->mode->width;
    int height = this->mode->height;

    this->window = glfwCreateWindow(width, height - 15, "Medieval something", NULL, NULL);
    if (!this->window) {
        fprintf(stderr, "Could not create window");
        glfwTerminate();
        return;
    }
   
    glfwMakeContextCurrent(this->window);

    glfwMaximizeWindow(this->window);
    //glfwSetWindowMonitor(this->window, this->monitor, 0, 0, this->mode->width, this->mode->height, this->mode->refreshRate);

    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageCallbackARB(openGLDebugOutput, NULL);

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    glfwSetKeyCallback(this->window, keyCallback);
}

bool Window::loop() {
    glfwSwapBuffers(this->window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
    
    return !glfwWindowShouldClose(this->window);
}

void Window::close() {
    if (this->window) {
        glfwDestroyWindow(window);
        window = NULL;
    }
    glfwTerminate();
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    fprintf(stdout, "Key %s\n", glfwGetKeyName(key, 0));
}

void Window::errorCallback(int error, const char* desc) {
    fprintf(stderr, "Error: %s\n", desc);
    return;
}

void Window::openGLDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam) {
	
    // Thanks to Luca :)
    
    /*if (std::string(message, std::string("Buffer detailed info").size()) == "Buffer detailed info")
	{
		//filter it
		return;
	}

	std::string error_source, error_type, error_severity;
	switch(source) {
		case GL_DEBUG_SOURCE_API_ARB:
			error_source = "OpenGL";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
			error_source = "Windows";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
			error_source = "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
			error_source = "Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION_ARB:
			error_source = "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER_ARB:
			error_source = "Other";
			break;
		default:
			error_source = "UNKNOW";
			break;
	}

	switch (type) {
		case GL_DEBUG_TYPE_ERROR_ARB:
			error_type = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
			error_type = "Deprecated behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
			error_type = "Undefined behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY_ARB:
			error_type = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
			error_type = "Performance";
			break;
		case GL_DEBUG_TYPE_OTHER_ARB:
			error_type = "Other";
		default:
			error_type = "UNKNOW";
			break;
	}

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH_ARB:
			error_severity = "High";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
			error_severity = "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW_ARB:
			error_severity = "Low";
			break;
		default:
			error_severity = "UNKNOW";
			break;
	}

	std::cout << "Source: " << error_source << "  Type: " << error_type << "  Severity: " << error_severity << "  Message: " << message << std::endl;

	if (severity == GL_DEBUG_SEVERITY_HIGH_ARB) {
		raise(SIGTRAP);
	}*/
}
