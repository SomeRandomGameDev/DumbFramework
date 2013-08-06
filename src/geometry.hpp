#ifndef _DUMB_FW_GEOMETRY_
#define _DUMB_FW_GEOMETRY_

#include <vector>

#include "bufferobject.hpp"
#include "program.hpp"

namespace Render {

class Geometry
{
	public:
		Geometry();
		~Geometry();

		bool Create();
		void Destroy();

		bool SetAttribute(std::shared_ptr<BufferObject> data, GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset, GLuint divisor=0);
		bool SetPrimitive(std::shared_ptr<BufferObject> data, GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset);
		// [todo] SetPrimitive without bufferObject

		bool Compile();

		void Draw();
		void Draw(GLsizei count); 

	protected:
		void DrawElements();
		void DrawArrays();
		void DrawElementsInstanced(GLsizei count);
		void DrawArraysInstanced(GLsizei count);
		typedef void (Geometry::*DrawSingleMethod)();
		typedef void (Geometry::*DrawInstancedMethod)(GLsizei count);

	protected:
		enum State
		{
			UNITIALIZED,
			INITIALIZED,
			COMPILED
		};

		struct Attribute
		{
			std::shared_ptr<BufferObject> buffer; 
			GLuint  index;
			GLenum  type;
			GLint   size;
			GLsizei stride;
			GLsizei offset;
			GLuint  divisor;

			Attribute();
			Attribute(std::shared_ptr<BufferObject> data, GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset, GLuint divisor=0);
			Attribute(const Attribute& attr);
			~Attribute();
			Attribute& operator= (const Attribute& attr);
		};

		struct Primitive
		{
			std::shared_ptr<BufferObject> buffer; 
			GLenum	primitive;
			GLenum	type;
			GLint	size;
			GLint	count;
			GLsizei offset;

			Primitive();
			Primitive(std::shared_ptr<BufferObject> data, GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset);
			Primitive(const Primitive& prim);
			~Primitive();
			Primitive& operator= (const Primitive& prim);
		};

		State _state;
		std::vector<Attribute> _attributes; ///< Vertex data attributes.
		Primitive _primitive;
		GLuint _vao;	///< Vertex array object.
		DrawSingleMethod _drawSingle;
		DrawInstancedMethod _drawInstanced;
};

}

#endif /* _DUMB_FW_GEOMETRY_ */