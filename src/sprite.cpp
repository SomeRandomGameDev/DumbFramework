/*
 * Copyright 2015 Stoned Xander
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <DumbFramework/config.hpp>
#include <tinyxml2.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>

#include <DumbFramework/render/textureloader.hpp>
#include <DumbFramework/sprite.hpp>

namespace Dumb {
    namespace Sprite {
        
        /** Default constructor. **/
        Animation::Animation()
            : _name("")
              , _frames()
        {}
        /** Destructor. **/
        Animation::~Animation()
        {}
        /**
         * Create animation.
         * @param [in] name  Animation name.
         */
        void Animation::create(std::string const& name)
        {
            _name = name;
            _frames.clear();
        }
        /**
         * Get animation name.
         * @return Animation name.
         */
        std::string const& Animation::name() const
        {
            return _name;
        }
        /**
         * Add frame to animation.
         * Frames are sorted by increasing time.
         * @param [in] frame  Frame to be added.
         */
        void Animation::add(Frame const& frame)
        {
            if(_frames.empty())
            {
                _frames.push_back(frame);
            }
            else
            {
                // Find the right spot and insert it.
                // We will search backwards as the array is already sorted.
                std::vector<Frame>::iterator target = _frames.end();
                for(off_t offset=_frames.size()-1; offset>=0; offset--)
                {
                    if(_frames[offset].time < frame.time)
                    {
                        break;
                    }
                    target = _frames.begin() + offset;
                }
                _frames.insert(target, frame);
            }
        }
        /**
         * Get frame count.
         * @return Number of frames in animation.
         */
        size_t Animation::frameCount() const
        {
            return _frames.size();
        }
        /**
         * Retrieve a given frame.
         * @param [in] index  Frame number.
         * @return A const pointer to the specified frame or NULL if the
         *         index is out of bound.
         */
        Frame const* Animation::getFrame(size_t offset) const
        {
            if(offset >= _frames.size())
            {
                return NULL;
            }
            return &_frames[offset];
        }

        /** Default constructor. **/
        Definition::Definition()
            : _name("")
              , _animations()
        {}
        /** Destructor. **/
        Definition::~Definition()
        {}
        /**
         * Create sprite definition.
         * @param [in] name  Identifier.
         * @param [in] count Animation count. 
         */
        void Definition::create(std::string const& name, size_t count)
        {
            _name = name;
            _animations.resize(count);
        }
        /**
         * Get name.
         * @return name.
         */
        std::string const& Definition::name() const
        {
            return _name;
        }
        /**
         * Get animation count.
         * @return Number of stored animations.
         */
        size_t Definition::animationCount() const
        {
            return _animations.size();
        }
#if 0
        /**
         * Add animation provided the definition does not already an
         * animation with the same name.
         * @param [in] animation  Animation to add.
         * @return true if the animation was added, or false an animation
         *         with the same name is already present.
         */
        bool Definition::add(Animation const& animation)
        {
            if(_dict.end() != _dict.find(animation.name()))
            {
                Log_Error(Framework::Module::Render, "The definition \"%s\" already contains an animation called \"%s\"", _name.c_str(), animation.name().c_str());
                return false;
            }
            _animations.push_back(animation); // <- performance issue
            _dict[animation.name()] = _animations.size() - 1;
            return true;
        }
#endif
        /**
         * Retrieve a given animation by its offset..
         * @param [in] index  Animation number.
         * @return A const pointer to the specified animation or NULL if the
         *         index is out of bound.
         */
        Animation const* Definition::getAnimation(size_t offset) const
        {
            if(offset >= _animations.size())
            { return NULL; }
            return &_animations[offset];
        }
        /**
         * Retrieve a given animation by its offset..
         * @param [in] index  Animation number.
         * @return A pointer to the specified animation or NULL if the
         *         index is out of bound.
         */
        Animation* Definition::getAnimation(size_t offset)
        {
            if(offset >= _animations.size())
            { return NULL; }
            return &_animations[offset];
        }
        /**
         * Retrieve a given animation by its name.
         * @param [in] name  Animation name.
         * @return A const pointer to the animation with the corresponding
         *         name, or NULL if no matching animation was found.
         */
        Animation const* Definition::getAnimation(std::string const& name) const
        {
            std::unordered_map<std::string, unsigned int>::const_iterator it = _dict.find(name);
            if(it == _dict.cend()) { return NULL; }
            if(it->second >= _animations.size()) { return NULL; }
            return &_animations[it->second];
        }
        /**
         * Retrieve a given animation by its name.
         * @param [in] name  Animation name.
         * @return A pointer to the animation with the corresponding
         *         name, or NULL if no matching animation was found.
         */
        Animation* Definition::getAnimation(std::string const& name)
        {
            std::unordered_map<std::string, unsigned int>::iterator it = _dict.find(name);
            if(it == _dict.end()) { return NULL; }
            if(it->second >= _animations.size()) { return NULL; }
            return &_animations[it->second];
        }

        /**
         * Constructor.
         */
        Atlas::Atlas()
            : _definitions()
              , _dict()
              , _texture()
              , _size(0)
        {}

        /**
         * Destructor.
         */
        Atlas::~Atlas()
        {}

        /**
         * Access texture.
         * @return texture object.
         */
        Framework::Render::Texture2D const& Atlas::texture() const
        {
            return _texture;
        }
        /**
         * @brief Access to definitions.
         * Return a pointer to the definition stored at @a index, or
         * NULL if the index is out of storage bound.
         * @param index Definition number.
         * @return Definition pointer or NULL if the index is out of
         *         storage bound..
         */
        Definition* Atlas::get(unsigned int index)
        {
            if(index >= _definitions.size()) { return NULL; }
            return &_definitions[index];
        }
        /**
         * @brief Access to definitions (const version).
         * Return a pointer to the definition stored at @a index, or
         * NULL if the index is out of storage bound.
         * @param index Definition number.
         * @return Definition pointer or NULL if the index is out of
         *         storage bound..
         */
        Definition const* Atlas::get(unsigned int index) const
        {
            if(index >= _definitions.size()) { return NULL; }
            return &_definitions[index];
        }
        /**
         * Retrieve a given definition by its name.
         * @param [in] name  Definition name.
         * @return A pointer to the definition with the corresponding
         *         name, or NULL if no matching definition was found.
         */
        Definition* Atlas::get(std::string const& name)
        {
            std::unordered_map<std::string, unsigned int>::iterator it = _dict.find(name);
            if(it == _dict.end()) { return NULL; }
            if(it->second >= _definitions.size()) { return NULL; }
            return &_definitions[it->second];
        }
        /**
         * Retrieve a given definition by its name.
         * @param [in] name  Definition name.
         * @return A const pointer to the definition with the corresponding
         *         name, or NULL if no matching definition was found.
         */
        Definition const* Atlas::get(std::string const& name) const
        {
            std::unordered_map<std::string, unsigned int>::const_iterator it = _dict.find(name);
            if(it == _dict.cend()) { return NULL; }
            if(it->second >= _definitions.size()) { return NULL; }
            return &_definitions[it->second];
        }
        /**
         * Provide the number of definitions.
         * @return Number of definitions.
         */
        unsigned int Atlas::count() const
        { 
            return _definitions.size();
        }

        // XML Frame Reader.
        // Read a single frame from a tinyxml2::XMLElement.
        struct XMLFrameReader
        {
            // Read a frame from a tinyxml2::XMLElement.
            // @param [in]  element  XML Element containing the frame to read.
            // @param [out] frame    Frame read from the XML element.
            // @return true if the frame was successfully read.
            bool read(tinyxml2::XMLElement* element, Frame& frame);
            // Atlas image size used to normalize frame coordinates.
            glm::ivec2 imageSize;
        };
        bool XMLFrameReader::read(tinyxml2::XMLElement* element, Frame& frame)
        {
            float time = 0;
            glm::ivec2 offset(0);
            glm::ivec2 size(0);
            glm::ivec2 top(0);
            glm::ivec2 bottom(0);
            unsigned int index = 0;

            element->QueryFloatAttribute("time", &time);
            element->QueryIntAttribute("offsetX", &offset.x);
            element->QueryIntAttribute("offsetY", &offset.y);
            element->QueryIntAttribute("width",  &size.x);
            element->QueryIntAttribute("height", &size.y);
            element->QueryIntAttribute("topU", &top.x);
            element->QueryIntAttribute("topV", &top.y);
            element->QueryIntAttribute("bottomU", &bottom.x);
            element->QueryIntAttribute("bottomV", &bottom.y);
            element->QueryUnsignedAttribute("texture", &index);

            frame.time   = time / 1000.0;
            frame.offset = offset;
            frame.size   = size;
            frame.top    = glm::dvec2(top) / glm::dvec2(imageSize);
            frame.bottom = glm::dvec2(bottom) / glm::dvec2(imageSize);
            frame.layer  = index;
            return true;
        }
        // XML Animation Reader.
        // Read a single animation from a tinyxml2::XMLElement.
        class XMLAnimationReader
        {
            public:
                // Read an animation from a tinyxml2::XMLElement.
                // @param [in]  element    XML Element containing the animation to read.
                // @param [out] animation  Animation read from the XML element.
                // @return true if the animation was successfully read.
                bool read(tinyxml2::XMLElement* element, Animation& animation);
                // Frame reader.
                // As an animation contains a list of frames, it will be used to read
                // them.
                XMLFrameReader frameReader;
        };
        bool XMLAnimationReader::read(tinyxml2::XMLElement* element, Animation &animation)
        {
            unsigned int size;
            int err;
            tinyxml2::XMLElement* frameElement;

            err = element->QueryUnsignedAttribute("size", &size);
            if(tinyxml2::XML_NO_ERROR != err)
            {
                // Count frames.
                frameElement = element->FirstChildElement("frame");
                for(size=0; frameElement; size++, frameElement=frameElement->NextSiblingElement("frame"))
                {}
            }

            animation._frames.reserve(size);

            // Read frames
            bool ret = true;
            frameElement = element->FirstChildElement("frame");
            for(unsigned int i=0; ret && (i<size) && frameElement; ++i)
            {
                Frame frame;
                ret = frameReader.read(frameElement, frame);
                if(ret)
                {
                    animation.add(frame);
                    frameElement = frameElement->NextSiblingElement("frame");
                }
            }
            return ret;
        }
        // XML Definition Reader.
        // Read a single definition from a tinyxml2::XMLElement.
        class XMLDefinitionReader
        {
            public:
                // Read a definition (animation list) from a tinyxml2::XMLElement.
                // @param [in]  element     XML Element containing the definition to read.
                // @param [out] definition  Definition read from the XML element.
                // @return true if the definition was successfully read.
                bool read(tinyxml2::XMLElement* element, Definition& definition);
                // Animation reader.
                // As a definition contains a list of animations, it will be used to read
                // them.
                XMLAnimationReader animationReader;
        };
        bool XMLDefinitionReader::read(tinyxml2::XMLElement* element, Definition &definition)
        {
            char const* name;
            int err;
            unsigned int size;
            tinyxml2::XMLElement *animElement;

            name = element->Attribute("name");
            if(NULL == name) { return false; }

            err = element->QueryUnsignedAttribute("size", &size);
            if(tinyxml2::XML_NO_ERROR != err)
            {
                // Count frames.
                animElement = element->FirstChildElement("animation");
                for(size=0; animElement; size++, animElement=animElement->NextSiblingElement("animation"))
                {}        
            }

            if(0 == size)
            {
                Log_Error(Framework::Module::Render, "Missing animation for definition %s", name);
                return false;
            }

            definition.create(name, size);

            // Animation
            bool ret = true;
            animElement = element->FirstChildElement("animation");
            for(unsigned int i=0; ret && (i<size) && animElement; i++)
            {
                char const* animName;
                animName = animElement->Attribute("name");
                if(NULL == animName)
                {
                    Log_Error(Framework::Module::Render, "Missing animation name!"); 
                    return false;
                }
                // Check if there is not already an animation with the same name.
                if(definition._dict.end() != definition._dict.find(animName))
                {
                    Log_Error(Framework::Module::Render, "Duplication animation \"%s\"", animName);
                    return false;
                }

                definition._dict[animName] = i;
                definition._animations[i].create(name);

                ret = animationReader.read(animElement, definition._animations[i]);
                animElement = animElement->NextSiblingElement("animation");
            }
            return ret;
        }
        /**
         * Read atlas from a XML file.
         * @param [in] filename  XML filename.
         * @return true if the atlas was succesfully read.
         */
        bool Atlas::read(std::string const& filename)
        {
            tinyxml2::XMLDocument xml;
            int err;

            // Load xml file.
            err = xml.LoadFile(filename.c_str());
            if(tinyxml2::XML_NO_ERROR != err)
            {
                Log_Error(Framework::Module::Render, "An error occured while parsing %s: %s (%s)", filename.c_str(), xml.GetErrorStr1(), xml.GetErrorStr2());
                return false; 
            }

            // Atlas
            tinyxml2::XMLElement *root = xml.FirstChildElement("atlas");
            if(NULL == root)
            {
                Log_Error(Framework::Module::Render, "Missing atlas element!");
                return false;
            }

            // -- Image filenames
            char const* imageFilenames = root->Attribute("path");
            if(NULL == imageFilenames)
            {
                Log_Error(Framework::Module::Render, "Invalid path to texture");
                return false;
            }

            // Load images and create texture.
            // This must be done before parsing anything else because we are
            // using its size to normalize sprite coordinates.
            err = loadTextures(imageFilenames);
            if(!err)
            {
                return false;
            }

            // -- Initialize XML readers.
            XMLDefinitionReader definitionReader;
            definitionReader.animationReader.frameReader.imageSize = _texture.size();

            unsigned int size;
            tinyxml2::XMLElement *defElement;

            err = root->QueryUnsignedAttribute("size", &size);
            if(tinyxml2::XML_NO_ERROR != err)
            {
                // Count definitions
                defElement = root->FirstChildElement("definition");
                for(size=0; defElement; size++, defElement=defElement->NextSiblingElement("definition"))
                {}
            }

            if(0 == size)
            {
                Log_Error(Framework::Module::Render, "Missing definition for atlas");
                return false;
            }

            Log_Info(Framework::Module::Render, "Prepare to process %d sprite definition(s).", size);
            _definitions.resize(size);

            // Definitions
            bool ret = true;
            defElement = root->FirstChildElement("definition");
            for(unsigned int i=0; ret && (i<size) && defElement; i++)
            {
                const char *defName = defElement->Attribute("name");
                if(NULL == defName)
                {
                    Log_Error(Framework::Module::Render, "Missing definition name!"); 
                    return false;
                }
                // Check if there is not already an animation with the same name.
                if(_dict.end() != _dict.find(defName))
                {
                    Log_Error(Framework::Module::Render, "Duplication definition \"%s\"", defName);
                    return false;
                }
                _dict[defName] = i;

                ret = definitionReader.read(defElement, _definitions[i]);
                defElement = defElement->NextSiblingElement("definition");
            }

            if(!ret)
            {
                Log_Error(Framework::Module::Render, "An error occured while parsing %s: %s (%s)", filename.c_str(), xml.GetErrorStr1(), xml.GetErrorStr2());
                return false;
            }

            return true;
        }
        /**
         * Load images from files and create a multiple layered texture out of them.
         * @param [in] filename  Image filenames list. Filenames are separated by a semi-colon.
         * @return true if the texture was succesfully created.
         */
        bool Atlas::loadTextures(const char *filename)
        {
            // Parse all texture path.
            // The paths are separated by a semi-colon ';'.
            std::stringstream ss(filename);
            std::vector<std::string> items;

            std::string item;
            while (std::getline(ss, item, ';'))
            {
                items.push_back(item);
            }

            bool ret;
            ret = Framework::Render::Texture::load(_texture, items, Framework::Render::Texture::PixelFormat::RGBA_8);
            if(ret)
            {
                _texture.bind();
                _texture.setMinFilter(Framework::Render::Texture::MinFilter::LINEAR_TEXEL);
                _texture.setMagFilter(Framework::Render::Texture::MagFilter::LINEAR_TEXEL);
                _texture.setWrap(Framework::Render::Texture::Wrap::CLAMP_TO_EDGE, Framework::Render::Texture::Wrap::CLAMP_TO_EDGE);
                _texture.unbind();

                _size = _texture.size();

                Log_Info(Framework::Module::Render, "Atlas Size : %d x %d", _size.x, _size.y);
            }
            return ret;
        }

    } // 'Sprite' namespace.
} // 'Dumb' namespace.

/* Enhancement proposal!
   <atlas layers="4">
   <images>
   <image layer="0" path="/path/to/imagefile.png"/>
   <image layer="1" path="/path/to/otherfile.png"/>
   <image layer="2" path="/path/to/layerfile.png"/>
   </images>
 * 
 <!-- List of definitions -->

 <definition name="little guy">
 <!-- List of animations -->
 <animation name="walking">
 <!-- List of frames -->
 <frame time="0"
 offsetX="0" offsetY="0"
 width="22" height="16"
 topU="64" topV="32"
 bottomU="96" bottomV="48"/>
 <!-- ... -->
 </animation>
 <!-- ... -->
 </definition>

 <!-- ... -->

 <definition id="6">
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
