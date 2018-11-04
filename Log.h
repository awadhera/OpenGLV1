#pragma once
#include<time.h>
#include<stdarg.h>

#define GL_LOG_FILE "gl.log"

inline bool restart_gl_log()
{
	FILE* file;
	errno_t err = fopen_s(&file, GL_LOG_FILE, "w");
	if (err!=0)
	{
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	}
	time_t now = time(nullptr);
	char str[26];
	ctime_s(str, sizeof(str), &now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", str);
	fclose(file);
	return true;
}

inline bool gl_log(const char* message, ...)
{
	va_list argptr; 
	FILE* file;
	errno_t err = fopen_s(&file, GL_LOG_FILE, "a");
	if (err!=0)
	{
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	} 
	va_start(argptr, message);
	vfprintf(file, message, argptr); 
	va_end(argptr); 
	fclose(file);
	return true;
}

inline bool gl_log_err(const char* message, ...)
{
	va_list argptr;
	FILE* file;
	errno_t err = fopen_s(&file, GL_LOG_FILE, "a");
	if (err != 0)
	{
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

inline void glfw_error_callback(int error, const char* description)
{
	gl_log_err("GLFW ERROR: code %i msg: %s \n", error, description);
}



