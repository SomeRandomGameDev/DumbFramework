#include <cstddef>
#include <functional>
#include "geometry.hpp"

#define GEOMETRY_VALIDATE_STATE(expected) \
do \
{ \
	if(_state != (expected)) \
	{ \
		Log_Error(ModuleID::RENDER, "Invalid state (expected \"" #expected "\") !");  \
		return false; \
	} \
} while(0);

namespace Render {

Geometry::Attribute::Attribute()
	: buffer(nullptr)
	, index(0)
	, type(GL_NONE)
	, size(0)
	, stride(0)
	, offset(0)
	, divisor(0)
{}

Geometry::Attribute::Attribute(std::shared_ptr<BufferObject> data, GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset, GLuint divisor)
	: buffer(data)
	, index(index)
	, type(type)
	, size(size)
	, stride(stride)
	, offset(offset)
	, divisor(divisor)
{}

Geometry::Attribute::Attribute(const Geometry::Attribute& attr)
	: buffer(attr.buffer)
	, index(attr.index)
	, type(attr.type)
	, size(attr.size)
	, stride(attr.stride)
	, offset(attr.offset)
	, divisor(attr.divisor)
{}

Geometry::Attribute::~Attribute()
{}

Geometry::Attribute& Geometry::Attribute::operator= (const Geometry::Attribute& attr)
{
	buffer = attr.buffer;
	index = attr.index;
	type = attr.type;
	size = attr.size;
	stride = attr.stride;
	offset = attr.offset;
	divisor = attr.divisor;
	return *this;
}

Geometry::Primitive::Primitive()
	: buffer(nullptr) 
	, primitive(GL_NONE)
	, type(GL_NONE)
	, size(0)
	, count(0)
	, offset(0)
{}

Geometry::Primitive::Primitive(std::shared_ptr<BufferObject> data, GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset)
	: buffer(data) 
	, primitive(primitive)
	, type(type)
	, size(size)
	, count(count)
	, offset(offset)
{}

Geometry::Primitive::Primitive(const Geometry::Primitive& prim)
	: buffer(prim.buffer) 
	, primitive(prim.primitive)
	, type(prim.type)
	, size(prim.size)
	, count(prim.count)
	, offset(prim.offset)
{}

Geometry::Primitive::~Primitive()
{}

Geometry::Primitive& Geometry::Primitive::operator= (const Geometry::Primitive& prim)
{
	buffer = prim.buffer; 
	primitive = prim.primitive;
	type = prim.type;
	size = prim.size;
	count = prim.count;
	offset = prim.offset;
	return *this;
}

Geometry::Geometry()
	: _state(Geometry::UNITIALIZED)
	, _attributes()
	, _primitive()
	, _vao(0)
	, _drawInstanced(NULL)
	, _drawSingle(NULL)
{}

Geometry::~Geometry()
{
	Destroy();
}

bool Geometry::Create()
{
	GEOMETRY_VALIDATE_STATE(Geometry::UNITIALIZED);

	if(_vao)
	{
		Log_Error(ModuleID::RENDER, "Inconsistent state.");
		return false;
	}

	_attributes.clear();
	
	glGenVertexArrays(1, &_vao);
	GLenum err = glGetError();
	if(GL_NONE != err)
	{
		Log_Error(ModuleID::RENDER, "An error occured while creating vertex array object: %s", gluErrorString(err));
		return false;
	}

	return true;	
}

void Geometry::Destroy()
{
	_attributes.clear();
	_primitive = Geometry::Primitive();
	if(_vao)
	{
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}
	_state = Geometry::UNITIALIZED;
}

bool Geometry::SetAttribute(std::shared_ptr<BufferObject> buffer, GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset, GLuint divisor)
{
	GEOMETRY_VALIDATE_STATE(Geometry::INITIALIZED);

	for(size_t i=0; i<_attributes.size(); i++)
	{
		if(i == index)
		{
			Log_Error(ModuleID::RENDER, "Attributes #%d already in use.", index);
			return false;
		}
	}
	_attributes.push_back(Geometry::Attribute(buffer, index, type, size, stride, offset, divisor));
	return true;
}

bool Geometry::SetPrimitive(std::shared_ptr<BufferObject> buffer, GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset)
{
	GEOMETRY_VALIDATE_STATE(Geometry::INITIALIZED);

	// [todo]
	return true;
}
		
bool Geometry::Compile()
{
	GEOMETRY_VALIDATE_STATE(Geometry::INITIALIZED);

	// Sanity check
	// [todo] make a isValid like method
	if(!_vao)
	{
		Log_Error(ModuleID::RENDER, "Invalid vertex array object id: %x\n", _vao);
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

		for(GLuint i=0; i<_attributes.size(); i++)
		{
			_attributes[i].buffer->bind();
				glVertexAttribPointer(_attributes[i].index, _attributes[i].size, _attributes[i].type, GL_FALSE, _attributes[i].stride, (GLvoid*)_attributes[i].offset);
				glVertexAttribDivisor(_attributes[i].index, _attributes[i].divisor);
			_attributes[i].buffer->unbind();
		}

		if(nullptr != _primitive.buffer)
		{
			_primitive.buffer->bind();
			_drawInstanced = &Geometry::DrawElementsInstanced;
			_drawSingle = &Geometry::DrawElements;
		}
		else
		{
			_drawInstanced = &Geometry::DrawArraysInstanced;
			_drawSingle = &Geometry::DrawArrays;
		}
	glBindVertexArray(0);

	_state = Geometry::COMPILED;
	return true;
}

void Geometry::DrawElements()
{
	glDrawElements(_primitive.primitive, _primitive.count, _primitive.type, (GLvoid*)_primitive.offset);
}

void Geometry::DrawArrays()
{
	glDrawArrays(_primitive.primitive, 0, _primitive.count);
}

void Geometry::DrawElementsInstanced(GLsizei count)
{
	glDrawElementsInstanced(_primitive.primitive, _primitive.count, _primitive.type, (GLvoid*)_primitive.offset, count);
}

void Geometry::DrawArraysInstanced(GLsizei count)
{
	glDrawArraysInstanced(_primitive.primitive, 0, _primitive.count, count);
}

void Geometry::Draw()
{
	glBindVertexArray(_vao);
		(this->*(_drawSingle))();
	glBindVertexArray(0);
}

void Geometry::Draw(GLsizei count)
{
	glBindVertexArray(_vao);
		(this->*(_drawInstanced))(count);
	glBindVertexArray(0);
}

}