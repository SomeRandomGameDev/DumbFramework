#ifndef _DUMB_FW_MATERIAL_
#define _DUMB_FW_MATERIAL_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

#define MATERIAL_MAX_TEXTURE_UNITS 8

namespace Render
{
	class Material
	{
		public:
			Material();
			~Material();

			bool create(GLuint shaderId);
			void destroy();

			void bind();
			void unbind();

			const glm::vec4& ambient() const;
			void ambient(const glm::vec4& color);

			const glm::vec4& diffuse() const;
			void diffuse(const glm::vec4& color);

			const glm::vec4& specular() const;
			void specular(const glm::vec4& color);

			const glm::vec4& emissive() const;
			void emissive(const glm::vec4& color);

			GLuint shaderId() const;

			GLuint textureId(unsigned int unit) const;
			bool textureId(unsigned int unit, GLuint id);

			GLuint bufferId() const { return _colorsUBO; } // [todo] remove
			GLuint blockIndex() const { return _colorsBlockIndex; } // [todo] remove

		public:
			enum
			{
				AMBIENT = 0,
				DIFFUSE,
				SPECULAR,
				EMISSIVE
			};

		protected:
			bool _updateNeeded; // [todo] better "dirty" flag?
			GLuint _shader; ///< Program Id. [todo] Use program?

			/** Material colors.
			 *    0: Ambient color (rgba).
			 *    1: Diffuse color (rgba).
			 *    2: Specular color (rgba).
			 *    3: Emissive color (rgba).
			 */
			glm::vec4 _colors[4];

			GLuint  _colorsUBO;         // [todo] use bufferObjects? uniform block helpers?
			GLuint  _colorsBlockIndex;
			GLsizei _colorsBlockSize;
			GLint   _colorsOffset[4];

			GLuint _texture[MATERIAL_MAX_TEXTURE_UNITS]; ///< Texture id for each unit. [todo] Texture struct (targetType, id, etc...) ?
	};
}

#endif // _DUMB_FW_MATERIAL_