#ifndef _DUMB_FW_TEXTURE_LOADER_
#define _DUMB_FW_TEXTURE_LOADER_

#include <vector>
#include <string>
#include <texture.hpp>
#include <texture2d.hpp>

namespace Framework {
namespace Texture   {

class Loader
{
	public:
		Loader();
		~Loader();
		
		bool load(const std::string& name, Texture2D& out);
		bool load(const std::vector<std::string>& names, Texture2D& out);
		bool loadLayer(const std::string& name, size_t layer, Texture2D& out);
	private:
		// [todo]
};

} // Texture
} // Framework

#endif /* _DUMB_FW_TEXTURE_LOADER_ */
