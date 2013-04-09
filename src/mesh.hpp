#ifndef _DUMB_FW_MESH_
#define _DUMB_FW_MESH_

#include <vector>

#include "bufferobject.hpp"
#include "program.hpp"

class Mesh
{
	public:
		Mesh();
		~Mesh();

		bool Create(std::shared_ptr<BufferObject*> vertices, std::shared_ptr<BufferObject*> indices/* [todo] default=null? */);
		void Destroy();

		bool SetAttribute(GLuint index, GLenum type, GLint size, GLsizei stride, GLsizei offset);
		bool SetPrimitive(GLenum primitive, GLenum type, GLint size, GLint count, GLsizei offset);
		
		bool Compile();

		void Draw(GLsizei count); 
		// [todo] if(indices) drawArraysInstanced
		//        else drawElementsInstanced
		//
	protected:
		struct Attribute
		{
			GLuint  index;
			GLenum  type;
			GLint   size;
			GLsizei stride;
			GLsizei offset;
		};

		struct Primitive
		{
			GLenum	primitive;
			GLenum	type;
			GLint	size;
			GLint	count;
			GLsizei offset;
		};

		// [todo] bool _status; ?
		std::shared_ptr<BufferObject*> _vertex; ///< Vertex data.
		std::shared_ptr<BufferObject*> _index; ///< Index data.
		std::vector<Attribute> _attributes; ///< Vertex data attributes.
		Primitive _primitive;
		GLint _vao;	///< Vertex array object.
};

#endif /* _DUMB_FW_MESH_ */