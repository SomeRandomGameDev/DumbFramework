#include <GL/glew.h>
#include <GL/glfw.h>

#include <file.hpp>
#include <program.hpp>

int main()
{
	glfwInit();

	glfwOpenWindow(800, 600, 8, 8, 8, 8, 0, 0, GLFW_WINDOW );
	glfwSetWindowTitle("foobar");

	glewInit();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	Program prog;
	Shader vertShader, fragShader;

	std::string data;

	File input;

	input.open("resources/std.vs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	vertShader.create(Shader::VERTEX_SHADER, data.c_str());

	input.open("resources/materialTest.fs", File::READ_ONLY);
	data.resize(input.size()+1);
	input.read(&data[0], data.size());
	input.close();
	
	fragShader.create(Shader::FRAGMENT_SHADER, data.c_str());

	prog.create();
	prog.attach(vertShader);
	prog.attach(fragShader);
	prog.bindFragDataLocation(0, "fragData");
	prog.link();

	// [todo] create texture
	// [todo] create material
	// [todo] create geometry

	do
	{
		glDrawBuffer(GL_BACK);

		glfwSwapBuffers();
	}while((glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS) && glfwGetWindowParam(GLFW_OPENED));

	prog.destroy();
	vertShader.destroy();
	fragShader.destroy();

	glfwCloseWindow();
	glfwTerminate();

	return 0;
}