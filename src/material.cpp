#include "material.hpp"

namespace Render {

Material::Material()
	: _updateNeeded(true)
	, _shader(0)
	, _colorsUBO(0)
	, _colorsBlockIndex(0)
	, _colorsBlockSize(0)
{
	for(size_t i=0; i<4; i++)
	{
		_colorsOffset[i] = 0;
	}

	for(size_t i=0; i<MATERIAL_MAX_TEXTURE_UNITS; i++)
	{
		_texture[i] = 0;
	}
}

Material::~Material()
{
	Destroy();
}

const glm::vec4& Material::Ambient() const
{
	return _colors[Material::AMBIENT];
}

void Material::Ambient(const glm::vec4& color)
{
	_colors[Material::AMBIENT] = color;
	_updateNeeded = true;
}

const glm::vec4& Material::Diffuse() const
{
	return _colors[Material::DIFFUSE];
}

void Material::Diffuse(const glm::vec4& color)
{
	_colors[Material::DIFFUSE] = color;
	_updateNeeded = true;
}

const glm::vec4& Material::Specular() const
{
	return _colors[Material::SPECULAR];
}

void Material::Specular(const glm::vec4& color)
{
	_colors[Material::SPECULAR] = color;
	_updateNeeded = true;
}

const glm::vec4& Material::Emissive() const
{
	return _colors[Material::EMISSIVE];
}

void Material::Emissive(const glm::vec4& color)
{
	_colors[Material::EMISSIVE] = color;
	_updateNeeded = true;
}

GLuint Material::ShaderId() const
{
	return _shader;
}

GLuint Material::TextureId(unsigned int unit) const
{
	if(unit >= MATERIAL_MAX_TEXTURE_UNITS)
	{
		return 0;
	}
	return _texture[unit];
}

bool Material::TextureId(unsigned int unit, GLuint id)
{
	if(unit >= MATERIAL_MAX_TEXTURE_UNITS)
	{
		return false;
	}
	_texture[unit] = id;
	return true;
}


bool Material::Create(GLuint shaderId)
{
	// Setup uniform buffer
	const GLchar* colorsUniformNames[4] = 
	{
		"material.ambient",
		"material.diffuse",
		"material.specular",
		"material.emissive",
	};

	GLuint colorsUniformIndices[4];

	_shader = shaderId;

	glGenBuffers (1, &_colorsUBO);
	glBindBuffer (GL_UNIFORM_BUFFER, _colorsUBO);
	{
		// [todo] check errors
		_colorsBlockIndex = glGetUniformBlockIndex (_shader, "Material");
		glGetActiveUniformBlockiv(_shader, _colorsBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_colorsBlockSize);

		glGetUniformIndices (_shader, 4, colorsUniformNames, colorsUniformIndices);	
		glGetActiveUniformsiv (_shader, 4, colorsUniformIndices, GL_UNIFORM_OFFSET, _colorsOffset);
			
		glBufferData(GL_UNIFORM_BUFFER, _colorsBlockSize, NULL, GL_DYNAMIC_DRAW);
		glUniformBlockBinding (_shader, _colorsBlockIndex, 0); // [todo] static const GLuint _colorsBindingId = 0
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, _colorsUBO);
	}
	glBindBuffer (GL_UNIFORM_BUFFER, 0);
	
	// Associate samplers with texture units.
	char textureUniformName[16];
	GLint uid;
	glUseProgram(_shader);
	for(size_t i=0; i<MATERIAL_MAX_TEXTURE_UNITS; i++)
	{
		sprintf(textureUniformName, "Channel%0d", i);
		uid = glGetUniformLocation(_shader, textureUniformName);
		glUniform1i(uid, i);
	}
	glUseProgram(0);

	return true;
}

void Material::Destroy()
{
	_updateNeeded = false;
	_shader = 0;
	if(_colorsUBO)
	{
		glDeleteBuffers(1, &_colorsUBO);
		_colorsUBO = 0;
	}

	_colorsBlockIndex = 0;
	_colorsBlockSize  = 0;
	for(size_t i=0; i<4; i++)
	{
		_colorsOffset[i] = 0;
	}

	for(size_t i=0; i<MATERIAL_MAX_TEXTURE_UNITS; i++)
	{
		_texture[i] = 0;
	}
}

void Material::Bind()
{
	// Bind and update colors.
	glBindBuffer(GL_UNIFORM_BUFFER, _colorsUBO);
	if(_updateNeeded)
	{
		GLubyte* ptr;
		GLfloat *color;
		ptr = (GLubyte*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
			
		for(size_t i=0; i<4; i++)
		{
			color = reinterpret_cast<GLfloat*>(ptr+_colorsOffset[0]);
			memcpy(color, glm::value_ptr(_colors[i]), sizeof(GLfloat[4]));
		}
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	// Bind textures.
	for(size_t i=0; i<MATERIAL_MAX_TEXTURE_UNITS; i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, _texture[i]); // [todo]
	}
}

void Material::Unbind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for(size_t i=0; i<MATERIAL_MAX_TEXTURE_UNITS; i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, 0); // [todo]
	}
}

}