#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/transform.hpp>

#include <file.hpp>
#include <program.hpp>
#include <material.hpp>
#include <bufferobject.hpp>

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

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	Render::Shader vertShader, fragShader;

	std::string data;

	File input;

	input.open("resources/std.vs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	vertShader.create(Render::Shader::VERTEX_SHADER, data.c_str());

	input.open("resources/materialTest.fs", File::READ_ONLY);
//	input.open("resources/std.fs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	fragShader.create(Render::Shader::FRAGMENT_SHADER, data.c_str());

	Render::Program prog;
	prog.create();
	prog.attach(fragShader);
	prog.attach(vertShader);
	prog.link();

	GLfloat vertexData[] = 
	{
		-0.5f,-0.5f, 1.0f,
		-0.5f, 0.5f, 1.0f,
		 0.5f,-0.5f, 1.0f,
		 0.5f, 0.5f, 1.0f,
    };
	Render::BufferObject quad;
	quad.create(GL_ARRAY_BUFFER, 3*4*sizeof(GLfloat) ,GL_STATIC_DRAW ,vertexData);
	
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	quad.bind();
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindVertexArray (0);
	quad.unbind();

	Render::Material mat;
	mat.create(prog.getId());
	mat.ambient(glm::vec4(0.0, 1.0, 0.0, 1.0));

	GLint modelviewProjectionId = glGetUniformLocation(prog.getId(), "g_ModelviewProjection");
	glm::mat4 proj = glm::ortho<float>(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, 0.1, 10.0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(1.0, 0.0, 0.0, 0.0);
	do
	{
		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT);
	
		mat.bind();

		prog.begin();
			glUniformMatrix4fv(modelviewProjectionId, 1, GL_FALSE, glm::value_ptr(proj));
		
			glUniformBlockBinding(prog.getId(), mat.blockIndex(), 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, mat.bufferId());
		
			glBindVertexArray (vao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		prog.end();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}while((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && !glfwWindowShouldClose(window));

	mat.destroy();
	 
	prog.destroy();
	vertShader.destroy();
	fragShader.destroy();

	quad.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	Log::LogProcessor::instance().stop();

	return 0;
}