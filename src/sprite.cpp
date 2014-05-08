#include <config.hpp>
#include <sprite.hpp>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string.h>

#define STATE_FAULTED                    -1
#define STATE_NEW                         0
#define STATE_PARSE_DEFINITION            1
#define STATE_PARSE_ANIMATION             2
#define STATE_PARSE_FRAME                 3
#define STATE_READY                       4

namespace Sprite {

Atlas::Atlas(const char *path) {
  _state = STATE_NEW;
  XML::Parser<Atlas>::parse(this, path);
}

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
        std::cerr << "##! Invalid number of definition" << std::endl;
      } else {
      std::cout << "##? Prepare to read " << size
                << " sprite definition(s)." << std::endl;
        _definitions = new Container<Definition *>(size);
        for(int i = 0; i < size; ++i) {
          *(_definitions->data(i)) = 0;
        }
        if(0 != filename) {
          _texture = SOIL_load_OGL_texture( filename, SOIL_LOAD_AUTO,
                                            SOIL_CREATE_NEW_ID,
                                            SOIL_FLAG_COMPRESS_TO_DXT );
          if(0 != _texture) {
            glBindTexture(0, _texture);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
            if(_height <= 0) {
              _height = 1;
            }
            if(_width <= 0) {
              _width = 1;
            }
            std::cout << "##? Atlas Size : " << _width << "x" << _height << std::endl;
            _state = STATE_PARSE_DEFINITION;
          } else {
            _state = STATE_FAULTED;
            std::cerr << "##! Invalid path to texture" << std::endl;
          }
        } else {
            std::cerr << "##! No filename specified for the sprite atlas."
                      << std::endl;
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
        std::cerr << "##! Invalid number of animations ("
                  << size << ") in def " << id << std::endl;
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
        std::cerr << "##! Invalid number of frames" << std::endl;
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
          }
          // TODO Add Length.
        }
        Frame *frame = new Frame(
                        time, glm::ivec2(offsetx, offsety),
                        glm::ivec2(width, height),
                        glm::dvec2(topu/(double)_width, topv/(double)_height),
                        glm::dvec2(bottomu/(double)_width, bottomv/(double)_height));
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


}

/*
<atlas file="/path/to/imagefile.dat" size="...">
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
              length="6"
              asize="5"
              startX="32" startY="32"
              nextX="33" nextY="40"
              width="32" height="32"/>
</atlas>
*/
