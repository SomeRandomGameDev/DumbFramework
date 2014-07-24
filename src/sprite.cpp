#include <config.hpp>
#include <sprite.hpp>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>

#define MAX_LOADABLE_TEXTURES            32

#define STATE_FAULTED                    -1
#define STATE_NEW                         0
#define STATE_PARSE_DEFINITION            1
#define STATE_PARSE_ANIMATION             2
#define STATE_PARSE_FRAME                 3
#define STATE_READY                       4

namespace Sprite {

    /**
     * Constructor.
     * @param time Time of appearance in seconds.
     * @param offset Offset in pixels.
     * @param size Size in pixels.
     * @param top Top texture coordinates.
     * @param bottom Bottom texture coordinates.
     * @param ind Texture index in the texture array.
     */
    Frame::Frame(double time, 
            const glm::ivec2& offset, const glm::ivec2& size,
            const glm::dvec2& top, const glm::dvec2& bottom,
            unsigned int ind)
        : _time(time)
          , _offset(offset)
          , _size(size)
          , _top(top)
          , _bottom(bottom)
          , _texture(ind)
    {}

    /**
     * Constructor.
     * @param path Path to description file.
     */
    Atlas::Atlas(const char *path)
        : _definitions(0)
          , _texture(0)
          , _state(STATE_NEW)
          , _width(0)
          , _height(0)
          , _lastDefinition(0)
          , _lastAnimation(0)
          , _lastFrameId(0)
    {
        XML::Parser<Atlas>::parse(this, path);
    }

    /**
     * Destructor.
     */
    Atlas::~Atlas() {
        if(STATE_READY == _state) {
            int defCount = _definitions->capacity();
            for(int i = 0; i < defCount; ++i) {
                Definition *def = _definitions->get(i);
                int animCount = def->capacity();
                for(int j = 0; j < animCount; ++j) {
                    Animation *anim = def->get(j);
                    int frameCount = anim->capacity();
                    for(int k = 0; k < frameCount; ++k) {
                        Frame *frame = anim->get(k);
                        delete frame;
                    }
                    delete anim;
                }
                delete def;
            }
            delete _definitions;
        }
    }

    /**
     * Access texture identifier.
     * @return OGL-wise texture identifier.
     */
    GLuint Atlas::texture() const
    {
        return _texture;
    }

    /**
     * Access to definitions.
     * @param index Definition number.
     * @return Definition.
     */
    Definition *Atlas::get(unsigned int index)
    {
        return (STATE_READY == _state)?_definitions->get(index):0;
    }

    /**
     * Provide the number of definitions.
     * @return Number of definitions.
     */
    unsigned int Atlas::count() const
    { 
        return _definitions->capacity();
    }

    void Atlas::startElement(const XML_Char *tag, const XML_Char **attr) {
        // Ugly code ahead !
        if((STATE_NEW == _state) && (strcasecmp(tag, "atlas") == 0)) {
            const char *filename = 0;
            int size = 0;
            for(int i = 0; attr[i] != 0; i += 2) {
                if(strcasecmp(attr[i], "path") == 0) {
                    filename = reinterpret_cast<const char*>(attr[i+1]);
                }
                if(strcasecmp(attr[i], "size") == 0) {
                    size = atoi(attr[i+1]);
                }
            }
            if(size <= 0) {
                _state = STATE_FAULTED;
                Log_Error(Framework::Module::Render, "Invalid number of definition.");
            } else {
				Log_Info(Framework::Module::Render, "Prepare to read %d sprite definition(s).", size);
                _definitions = new Framework::Container<Definition *>(size);
                for(int i = 0; i < size; ++i) {
                    *(_definitions->data(i)) = 0;
                }
                if(0 != filename) {
                    loadTextures(filename);
                } else {
                    _state = STATE_FAULTED;
					Log_Error(Framework::Module::Render, "Invalid path to texture");
                }
            }
        }

        if((STATE_PARSE_DEFINITION == _state) && (strcasecmp(tag, "definition") == 0)) {
            int size = -1;
            int id = -1;
            for(int i = 0; attr[i] != 0; i+= 2) {
                if(strcasecmp(attr[i], "id") == 0) {
                    id = atoi(attr[i + 1]);
                }
                if(strcasecmp(attr[i], "size") == 0) {
                    size = atoi(attr[i + 1]);
                }
            }
            if((id < 0) || (size <= 0)) {
                _state = STATE_FAULTED;
                Log_Error(Framework::Module::Render, "Invalid number of animations (%d) in def", size);
            } else {
                Definition *definition = new Definition((unsigned int) size);
                *(_definitions->data(id)) = definition;
                _lastDefinition = definition;
                for(int i = 0; i < size; ++i) {
                    *(definition->data(i)) = 0;
                }
                _state = STATE_PARSE_ANIMATION;
            }
        }

        if((STATE_PARSE_ANIMATION == _state) && (strcasecmp(tag, "animation") == 0)) {
            // Same as for definition. (bweee)
            int size = 0;
            int id = -1;
            for(int i = 0; attr[i] != 0; i+= 2) {
                if(strcasecmp(attr[i], "id") == 0) {
                    id = atoi(attr[i + 1]);
                }
                if(strcasecmp(attr[i], "size") == 0) {
                    size = atoi(attr[i + 1]);
                }
            }
            if((id < 0) || (size <= 0)) {
                _state = STATE_FAULTED;
                Log_Error(Framework::Module::Render, "Invalid number of frames");
            } else {
                Animation *animation = new Animation((unsigned int) size);
                *(_lastDefinition->data(id)) = animation;
                _lastAnimation = animation;
                _lastFrameId = 0;
                for(int i = 0; i < size; ++i) {
                    *(animation->data(i)) = 0;
                }
                _state = STATE_PARSE_FRAME;
            }
        }

        if((STATE_PARSE_FRAME == _state) && (strcasecmp(tag, "frame") == 0)) {
            if(_lastFrameId < _lastAnimation->capacity()) {
                double time = 0;
                int offsetx = 0, offsety = 0, width = 0, height = 0;
                int topu = 0, topv = 0, bottomu = 0, bottomv = 0;
                unsigned int index = 0;
                for(int i = 0; attr[i] != 0; i += 2) {
                    if(strcasecmp(attr[i], "time") == 0) {
                        time = atof(attr[i+1])/1000.0;
                    } else if(strcasecmp(attr[i], "offsetx") == 0) {
                        offsetx = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "offsety") == 0) {
                        offsety = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "width") == 0) {
                        width = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "height") == 0) {
                        height = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "topu") == 0) {
                        topu = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "topv") == 0) {
                        topv = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "bottomu") == 0) {
                        bottomu = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "bottomv") == 0) {
                        bottomv = atoi(attr[i+1]);
                    } else if(strcasecmp(attr[i], "texture") == 0) {
                        index = (unsigned int) atoi(attr[i+1]);
                    }
                    // TODO Add Length.
                }
                Frame *frame = new Frame(
                        time, glm::ivec2(offsetx, offsety),
                        glm::ivec2(width, height),
                        glm::dvec2(topu/(double)_width, topv/(double)_height),
                        glm::dvec2(bottomu/(double)_width, bottomv/(double)_height),
                        index);
                *(_lastAnimation->data(_lastFrameId ++)) = frame;
            }
        }

    }

    void Atlas::endElement(const XML_Char *tag) {
        if((STATE_PARSE_FRAME == _state) && (strcasecmp(tag, "animation") == 0)) {
            _state = STATE_PARSE_ANIMATION;
        }

        if((STATE_PARSE_ANIMATION == _state) && (strcasecmp(tag, "definition") == 0)) {
            _state = STATE_PARSE_DEFINITION;
        }

        if((STATE_PARSE_DEFINITION == _state) && (strcasecmp(tag, "atlas") == 0)) {
            _state = STATE_READY;
        }
    }

    void Atlas::setFaulted(const char *msg,
        unsigned int cnt,
        unsigned char **textures) {
		Log_Error(Framework::Module::Render, msg);
        _state = STATE_FAULTED;
        for(unsigned int i = 0; i < cnt; ++i) {
            SOIL_free_image_data(textures[i]);
        }
    }

    void Atlas::loadTextures(const char *filename) {
        // Parse all texture path.
        // The paths are separated by a semi-colon ';'.
        std::stringstream ss(filename);
        std::string item;
        int width = -1;
        int currentWidth;
        int height = -1;
        int currentHeight;
        int channels; // Should be 4. Else, rejects.
        unsigned char *loadedTextures[MAX_LOADABLE_TEXTURES];
        unsigned int count = 0;
        while (std::getline(ss, item, ';')) {
            Log_Info(Framework::Module::Render, "Loading %s", item.c_str());
            loadedTextures[count] = SOIL_load_image(
                item.c_str(),
                &currentWidth, &currentHeight, &channels,
                SOIL_LOAD_AUTO);
            if(channels != 4) {
				Log_Error(Framework::Module::Render, "Channel = %d", channels);
                setFaulted("Incorrect channel size !", count,
                    loadedTextures);
                return;
            }
            if(width == -1) {
                width = currentWidth;
                height = currentHeight;
            } else {
                if((width != currentWidth) || (height != currentHeight)) {
                    setFaulted("Textures are not of the same size !",
                        count,
                        loadedTextures);
                    return;
                }
            }
            ++count;
        }

        // Make a big buffer.
        unsigned char *bigBuffer = new unsigned char[
                width*height*channels*count];
        // Concatenate the loaded textures.
        unsigned int size = width * height * channels;
        for(unsigned int i = 0; i < count; ++i) {
            memcpy(bigBuffer + (i * size), loadedTextures[i], size);
        }
        std::cout << std::dec;

        // Make GL Texture.
        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _texture);

        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
            width, height, count);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
            width, height, count, GL_RGBA, GL_UNSIGNED_BYTE,
            bigBuffer);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER,
            GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);


        for(unsigned int i = 0; i < count; ++i) {
            SOIL_free_image_data(loadedTextures[i]);            
        }
        delete []bigBuffer;

        _width = width;
        _height = height;

		Log_Info(Framework::Module::Render, "Atlas Size : %d x %d", _width, _height);
        _state = STATE_PARSE_DEFINITION;
    }
}

/*
   <atlas file="/path/to/imagefile.png;/path/to/otherfile.png;/path/to/layerfile.png" size="...">
   <!-- List of definitions -->

   <definition id="4" size="2">
   <!-- List of animations -->
   <animation id="0" size="1">
   <!-- List of frames -->
   <frame time="0"
   offsetX="0" offsetY="0"
   width="32" height="16"
   topU="64" topV="32"
   bottomU="96" bottomV="48"/>

   <!-- ... -->

   </animation>

   <!-- ... -->

   </definition>

   <!-- ... -->

   <definition id="6" size="5">
   <!-- Animation identifier incremented by default -->

   <!-- Uniform distribution over time -->
   <!-- No offset -->
   <!-- Constant size -->
   <!-- Uniform distribution on the atlas -->
   <animation size="6" length="5"
   startX="32" startY="32" nextX="33" nextY="0"
   width="32" height="32"/>

   </definition>

   <!-- Packed definition -->
   <!-- Fixed length animations -->
   <!-- One animation per line -->
   <definition size="6"
   texture="1"
   length="6"
   asize="5"
   startX="32" startY="32"
   nextX="33" nextY="40"
   width="32" height="32"/>
   </atlas>
   */
