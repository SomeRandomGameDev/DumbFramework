#ifndef _DUMB_FW_MATERIAL_
#define _DUMB_FW_MATERIAL_

#include <GL/glew.h>
#include <GL/glfw.h>

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

			bool Create(GLuint shaderId);
			void Destroy();

			void Bind();
			void Unbind();

			const glm::vec4& Ambient() const;
			void Ambient(const glm::vec4& color);

			const glm::vec4& Diffuse() const;
			void Diffuse(const glm::vec4& color);

			const glm::vec4& Specular() const;
			void Specular(const glm::vec4& color);

			const glm::vec4& Emissive() const;
			void Emissive(const glm::vec4& color);

			GLuint ShaderId() const;

			GLuint TextureId(unsigned int unit) const;
			bool TextureId(unsigned int unit, GLuint id);

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