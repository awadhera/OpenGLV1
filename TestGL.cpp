#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Log.h"
#include "Common/MathsFuncs.h"
#include <assert.h>


int g_gl_width;
int g_gl_height;

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	g_gl_width = width; g_gl_height = height; /* update any perspective matrices used here */
}

void log_gl_params() {
	GLenum params[] =
	{
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] =
	{
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	gl_log("GL Context Params:\n");
	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}
	int v[2]; v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	GLboolean s = false;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], static_cast<unsigned int>(s));
	gl_log("-----------------------------\n");
}

void _update_fps_counter(GLFWwindow* window)
{
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25)
	{
		previous_seconds = current_seconds;
		double fps = static_cast<double>(frame_count) / elapsed_seconds;
		char tmp[128];
		sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

int main()
{
	assert(restart_gl_log());

	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	/*	int monitors = 2;
		GLFWmonitor** mon = glfwGetMonitors(&monitors);
		const GLFWvidmode* vmode = glfwGetVideoMode(mon[0]);
		GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon[0], nullptr);
		int g_gl_width = vmode->width;
		int g_gl_height = vmode->height;*/


		/*	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/


	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
	int g_gl_width = 640;
	int g_gl_height = 480;


	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	log_gl_params();

	gl_log("Starting GLFW\n%s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetWindowSizeCallback(window, glfw_window_size_callback);

	GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
	GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat translation[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f };
	GLuint vbo[2];
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	std::ifstream infile;
	infile.open("Shaders/VertexShader.vert");
	std::stringstream buffer;
	buffer << infile.rdbuf();
	auto cStringVertexShader = buffer.str();
	const char* vertex_shader = cStringVertexShader.c_str();
	infile.close();
	buffer.str(std::string());
	infile.open("Shaders/FragmentShader.frag");
	buffer << infile.rdbuf();
	auto cStringFragmentShader = buffer.str();
	const char* fragment_shader = cStringFragmentShader.c_str();
	infile.close();
	buffer.str(std::string());

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, nullptr);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, nullptr);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glBindAttribLocation(shader_programme, 0, "vp");
	glBindAttribLocation(shader_programme, 1, "vc");
	glLinkProgram(shader_programme);
	int tm_location = glGetUniformLocation(shader_programme, "tm");
	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	float speed = 0.0f;
	float last_position = 0.0f;
	float cam_speed = 1.0f;
	float cam_yaw_speed = 10.0f;
	float cam_pos[] = { 0.0f, 0.0f, 2.0f };
	float cam_yaw = 0.0f;
	float near = 0.1f;
	float far = 100.f;
	float fov = 67.0f * ONE_DEG_IN_RAD;
	mat4 T = translate(identity_mat4(), vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg(identity_mat4(), -cam_yaw);
	mat4 view_mat = R * T;
	float aspect = (float)g_gl_width / (float)g_gl_height;
	float range = tan(fov*0.5f)*near;
	float sx = (2.0f*near) / (range*aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
	float proj_mat[] = { sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, -1.0f, 0.0f, 0.0f, pz, 0.0f };
	int view_mat_location = glGetUniformLocation(shader_programme, "view");
	int proj_mat_location = glGetUniformLocation(shader_programme, "proj");
	glUseProgram(shader_programme);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, proj_mat);
	while (!glfwWindowShouldClose(window))
	{
		bool cam_moved = false;
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;
		if (fabs(last_position) > 1.0f)
			speed = -speed;
		translation[12] = static_cast<float>(elapsed_seconds * speed + last_position);
		glUniformMatrix4fv(tm_location, 1, GL_FALSE, translation);
		last_position = translation[12];

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			cam_pos[0] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_D))
		{
			cam_pos[0] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP))
		{
			cam_pos[1] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN))
		{
			cam_pos[1] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_W))
		{
			cam_pos[2] += cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_S))
		{
			cam_pos[2] -= cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			cam_yaw += cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			cam_yaw -= cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (cam_moved)
		{
			mat4 T = translate(identity_mat4(), vec3(-cam_pos[0], -cam_pos[1], -cam_pos[2]));
			mat4 R = rotate_y_deg(identity_mat4(), -cam_yaw);
			mat4 view_mat = R * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}
		_update_fps_counter(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwPollEvents();
		glfwSwapBuffers(window);
		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}
	}
	glfwTerminate();
	return 0;
}
