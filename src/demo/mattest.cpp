#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/transform.hpp>

#include <DumbFramework/file.hpp>
#include <DumbFramework/program.hpp>
#include <DumbFramework/material.hpp>
#include <DumbFramework/bufferobject.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
	Log::LogProcessor::instance().start(new Log::LogBuilder<Log::AllPassFilter,Log::SimpleMessageFormat>, new Log::FileOutputPolicy); 
	glfwInit();

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "foobar", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwShowWindow (window);

	GLenum err = glewInit();

	err = glGetError();
	
	Render::Shader vertShader, fragShader;

	std::string data;

	File input;

	input.open("resources/std.vs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	vertShader.create(Render::Shader::VERTEX_SHADER, data.c_str());

	data.clear();

	input.open("resources/materialTest.fs", File::READ_ONLY);
//	input.open("resources/std.fs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	fragShader.create(Render::Shader::FRAGMENT_SHADER, data.c_str());

	Render::Program prog;
	prog.create();
	prog.attach(vertShader);
	prog.attach(fragShader);
	prog.link();
	
	vertShader.destroy();
	fragShader.destroy();

	GLfloat vertexData[] = 
	{
		 0.25f, 0.25f,-1.0f,
		 0.25f, 0.75f,-1.0f,
		 0.75f, 0.25f,-1.0f,
		 0.75f, 0.75f,-1.0f,
    };
	Render::BufferObject quad;
	quad.create(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat) ,GL_STATIC_DRAW ,vertexData);
	
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	quad.bind();
	glEnableVertexAttribArray (0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)0);
	glBindVertexArray (0);
	
	Render::Material mat;
	mat.create(prog.getId());
	mat.ambient(glm::vec4(0.0, 0.0, 1.0, 1.0));
	
	GLint modelviewProjectionId = glGetUniformLocation(prog.getId(), "g_ModelviewProjection");
	glm::mat4 proj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 10.0f);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(1.0, 0.0, 0.0, 0.0);

	do
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT);

		prog.begin();
			glUniformMatrix4fv(modelviewProjectionId, 1, GL_FALSE, glm::value_ptr(proj));
			mat.bind();
				glBindVertexArray (vao);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			mat.unbind();
		prog.end();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}while((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && !glfwWindowShouldClose(window));

	mat.destroy();
	 
	prog.destroy();

	quad.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	Log::LogProcessor::instance().stop();

	return 0;
}
