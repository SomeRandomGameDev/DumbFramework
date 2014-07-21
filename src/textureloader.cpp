#include "log.hpp"
#include "file.hpp"
#include "textureloader.hpp"

#define STBI_NO_STDIO
#define STBI_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

namespace Framework {
namespace Texture   {

// Fill 'data' with 'size' bytes.  return number of bytes actually read 
static int stbi_io_read_callback(void *user, char *data, int size)
{
	Framework::File *in = reinterpret_cast<Framework::File*>(user);
	if(NULL == in)
	{
		return 0;
	}
	size_t nread = in->read (data, static_cast<size_t>(size));
	return static_cast<int>(nread);
}
// skip the next 'n' bytes, or 'unget' the last -n bytes if negative
static void stbi_io_skip_callback(void *user, int n)
{
	Framework::File *in = reinterpret_cast<Framework::File*>(user);
	if(NULL == in)
	{
		return;
	}
	in->seek(n, Framework::File::CURRENT);
}
// returns nonzero if we are at end of file/data
static int stbi_io_eof_callback(void *user)
{
	Framework::File *in = reinterpret_cast<Framework::File*>(user);
	if(NULL == in)
	{
		return 1;
	}
	return (in->eof() ? 1 : 0);
}
// stdi callbacks for Framework::File
static const stbi_io_callbacks stbi_io_File_callbacks =
{
	stbi_io_read_callback,
	stbi_io_skip_callback,
	stbi_io_eof_callback
};

Loader::Loader()
{}

Loader::~Loader()
{}

bool Loader::load(const std::string& name, Texture2D& out)
{
	glm::ivec2 size;
	int componentCount;
	unsigned char *data;
	Framework::File in;
	bool ret;
	
	Texture::PixelFormat pixelFormat;
	
	ret = in.open(name, Framework::File::READ_ONLY);
	if(!ret)
	{
		Log_Error(Module::Render, "Failed to open %s", name.c_str());
		return false;
	}
	
	data = stbi_load_from_callbacks(&stbi_io_File_callbacks, &in, &size.x, &size.y, &componentCount, 0);
	in.close();
	if(NULL == data)
	{
		Log_Error(Module::Render, "Failed to load %s: %s", name.c_str(), stbi_failure_reason());
		return false;
	}
	
	switch(componentCount)
	{
		case 1:
			pixelFormat = Texture::PixelFormat::LUMINANCE_8;
			break;
		case 3:
			pixelFormat = Texture::PixelFormat::RGB_8;
			break;
		case 4:
			pixelFormat = Texture::PixelFormat::RGBA_8;
			break;
		default:
			break;
	}

	if(!out.isValid())
	{
		ret = out.create(size, pixelFormat);
	}
	else
	{
		// [todo] upscaling, downscaling ?
		ret = ((out.size() == size) && (out.pixelFormat().value == pixelFormat.value)); // [todo] pixel format operator==
	}
	if(ret)
	{
		out.setData(data);
	}
	stbi_image_free(data);
	return ret;
}

bool Loader::load(const std::vector<std::string>& names, Texture2D& out)
{
	bool ret = true;
	// [todo]
	return ret;
}

bool Loader::loadLayer(const std::string& name, size_t layer, Texture2D& out)
{
	bool ret = true;
	// [todo]
	return ret;
}

} // Texture
} // Framework

