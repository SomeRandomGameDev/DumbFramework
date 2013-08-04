#include <GL/glew.h>
#include <GL/glfw.h>

#include <file.hpp>
#include <program.hpp>
#include <material.hpp>

int main()
{
	Log::LogProcessor::instance().start(new Log::LogBuilder<Log::AllPassFilter,Log::SimpleMessageFormat>, new Log::FileOutputPolicy); 
	glfwInit();

	glfwOpenWindow(800, 600, 8, 8, 8, 8, 0, 0, GLFW_WINDOW );
	glfwSetWindowTitle("foobar");

	glewInit();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	Render::Program prog;
	Render::Shader vertShader, fragShader;

	std::string data;

	File input;

	input.open("resources/std.vs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	vertShader.create(Render::Shader::VERTEX_SHADER, data.c_str());

	input.open("resources/materialTest.fs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	fragShader.create(Render::Shader::FRAGMENT_SHADER, data.c_str());

	prog.create();
	prog.attach(vertShader);
	prog.attach(fragShader);
	prog.link();

	// [todo] create texture 

	Render::Material mat;
	mat.Create(prog.getId());
	mat.Ambient(glm::vec4(0.0, 1.0, 0.0, 1.0));
	
	// [todo] create geometry

	do
	{
		glDrawBuffer(GL_BACK);

		prog.begin();
		mat.Bind();
			// [todo] draw something
		mat.Unbind();
		prog.end();

		glfwSwapBuffers();
	}while((glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && glfwGetWindowParam(GLFW_OPENED));

	mat.Destroy();

	prog.destroy();
	vertShader.destroy();
	fragShader.destroy();

	glfwCloseWindow();
	glfwTerminate();

	Log::LogProcessor::instance().stop();

	return 0;
}