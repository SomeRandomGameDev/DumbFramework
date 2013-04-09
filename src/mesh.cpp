#include <cstddef>
#include "mesh.hpp"

Mesh::Mesh()
	: _vertex(nullptr)
	, _index(nullptr)
	, _attributes()
	, _primitive()
	, _vao(0)
{}

Mesh::~Mesh()
{
	Destroy();
}

bool Mesh::Create(std::shared_ptr<BufferObject> vertices, std::shared_ptr<BufferObject> indices)
{
	// [todo] check and set status
	if(_vao)
	{
		Log_Error(ModuleID::RENDER, "Wrong state.");
		return false;
	}
	_vertex = vertices;
	_index  = indices;
	
	// [todo] reset attributes and _primitive
	
	glGenVertexArrays(1, &_vao);
	GLenum err = glGetError();
	if(GL_NONE != err)
	{
		Log_Error(ModuleID::RENDER, "An error occured while creating vertex array object: %s", gluErrorString(err));
		return false;
	}

	return true;	
}

void Mesh::Destroy()
{
	_vertex = nullptr;
	_index  = nullptr;
	_attributes.clear();
	// [todo] _primitive = 0;
	if(_vao)
	{
		glDeleteVertexArrays(1, &_vao);
	}
}

bool Mesh::SetAttribute(GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset)
{
	// [todo]
	return true;
}

bool Mesh::SetPrimitive(GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset)
{
	// [todo]
	return true;
}
		
bool Mesh::Compile()
{
	// [todo] check state
	// Sanity check
	// [todo] make a isValid like method
	if(!_vao)
	{
		Log_Error(ModuleID::RENDER, "Invalid vertex array object id: %x\n", _vao);
		return false;
	}
	
	if(nullptr == _vertex)
	{
		Log_Error(ModuleID::RENDER, "Invalid vertex buffer.");
		return false;
	}

	if(0 == _attributes.size())
	{
		Log_Error(ModuleID::RENDER, "Empty attributes.");
		return false;
	}

	glBindVertexArray(_vao);
		for(GLuint i=0; i<_attributes.size(); i++)
		{
			glEnableVertexAttribArray(_attributes[i].index);
		}

		_vertex->bind();
		for(GLuint i=0; i<_attributes.size(); i++)
		{
			glVertexAttribPointer(_attributes[i].index, _attributes[i].size, _attributes[i].type, GL_FALSE, _attributes[i].stride, (GLvoid*)_attributes[i].offset);
		}
		_vertex->unbind();

		if(nullptr != _index)
		{
			_index->bind();
		}
	glBindVertexArray(0);
	
	// [todo] choose the right draw call.

	return true;
}

void Mesh::Draw(GLsizei count)
{
	// [todo] if(indices) drawArraysInstanced
	//        else drawElementsInstanced
}