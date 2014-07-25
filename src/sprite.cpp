#include <config.hpp>
#include <SOIL/SOIL.h>
#include <tinyxml2.h>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>

#include "sprite.hpp"

namespace Sprite {

/** Default constructor. **/
Animation::Animation()
    : _id(static_cast<unsigned int>(-1))
    , _frames()
{}
/** Destructor. **/
Animation::~Animation()
{}
/**
 * Create animation.
 * @param [in] id    Animation identifier.
 * @param [in] count Frame count (optional) (default=0).
 */
void Animation::create(unsigned int id, size_t count)
{
    _id = id;
    if(count)
    {
        _frames.reserve(count);
    }
}
/**
 * Get animation id.
 * @return Animation identifier.
 */
unsigned int Animation::id() const
{
    return _id;
}
/**
 * Add frame to animation.
 * Frames are sorted by increasing time.
 * @param [in] frame  Frame to be added.
 */
void Animation::add(Framework::Frame const& frame)
{
    if(_frames.empty())
    {
        _frames.push_back(frame);
    }
    else
    {
        // Find the right spot and insert it.
        // We will search backwards as the array is already sorted.
        std::vector<Framework::Frame>::iterator target = _frames.end();
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
Framework::Frame const* Animation::getFrame(size_t offset) const
{
    if(offset >= _frames.size())
    {
        return NULL;
    }
    return &_frames[offset];
}

/** Default constructor. **/
Definition::Definition()
    : _id(static_cast<unsigned int>(-1))
    , _animations()
{}
/** Destructor. **/
Definition::~Definition()
{}
/**
 * Create sprite definition.
 * @param [in] id    Identifier.
 * @param [in] count Animation count. 
 */
void Definition::create(unsigned int id, size_t count)
{
    _id = id;
    _animations.resize(count);
}
/**
 * Get id.
 * @return Identifier.
 */
unsigned int Definition::id() const
{
    return _id;
}
/**
 * Get animation count.
 * @return Number of stored animations.
 */
size_t Definition::animationCount() const
{
    return _animations.size();
}
/**
 * Retrieve a given animation.
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
 * Retrieve a given animation.
 * @param [in] index  Animation number.
 * @warning Unsafe and non const.
 * @return Reference to the specified animation.
 */
Animation& Definition::getAnimation(size_t offset)
{
    return _animations[offset];
}
/**
 * Constructor.
 * @param path Path to description file.
 */
Atlas::Atlas()
    : _definitions()
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
Framework::Texture2D const& Atlas::texture() const
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
    if(index >= _definitions.size()) { return NULL; }
    return &_definitions[index];
}
/**
 * Provide the number of definitions.
 * @return Number of definitions.
 */
unsigned int Atlas::count() const
{ 
    return _definitions.size();
}

// [todo] make FrameReader, AnimationReader, DefinitionReader classes?

// Read a single frame from XML file.
static bool readFrame(tinyxml2::XMLElement* element, Framework::Frame& frame)
{
    float time = 0;
    int offsetx = 0, offsety = 0, width = 0, height = 0;
    int topu = 0, topv = 0, bottomu = 0, bottomv = 0;
    unsigned int index = 0;
    
    element->QueryFloatAttribute("time", &time);
    element->QueryIntAttribute("offsetX", &offsetx);
    element->QueryIntAttribute("offsetY", &offsety);
    element->QueryIntAttribute("width", &width);
    element->QueryIntAttribute("height", &height);
    element->QueryIntAttribute("topU", &topu);
    element->QueryIntAttribute("topV", &topv);
    element->QueryIntAttribute("bottomU", &bottomu);
    element->QueryIntAttribute("bottomV", &bottomv);
    element->QueryUnsignedAttribute("texture", &index);
    
    frame.time   = time / 1000.0;
    frame.offset = glm::ivec2(offsetx, offsety);
    frame.size   = glm::ivec2(width, height);
    frame.top    = glm::dvec2(topu, topv) /1024.0; // [todo] divide by _size
    frame.bottom = glm::dvec2(bottomu, bottomv)/1024.0; // [todo] divide by _size
    frame.layer  = index;
    return true;
}
// Read a single animation from XML.
static bool readAnimation(tinyxml2::XMLElement* element, Animation &animation)
{
    unsigned int id, size;
    int err;
    
    err = element->QueryUnsignedAttribute("id", &id);
    if(tinyxml2::XML_NO_ERROR != err) { return false; }
    
    err = element->QueryUnsignedAttribute("size", &size);
    if(tinyxml2::XML_NO_ERROR != err) { return false; }

    animation.create(id, size);

    // Read frames
    bool ret = true;
    tinyxml2::XMLElement* frameElement = element->FirstChildElement("frame");
    for(unsigned int i=0; ret && (i<size) && frameElement; ++i)
    {
        Framework::Frame frame;
        ret = readFrame(frameElement, frame);
        if(ret)
        {
            animation.add(frame);
            frameElement = frameElement->NextSiblingElement("frame");
        }
    }
    
    return ret;
}
// Read sprite definition from XML.
static bool readDefinition(tinyxml2::XMLElement* element, Definition &definition)
{
    unsigned int id, size;
    int err;
    
    err = element->QueryUnsignedAttribute("id", &id);
    if(tinyxml2::XML_NO_ERROR != err) { return false; }
    
    err = element->QueryUnsignedAttribute("size", &size);
    if(tinyxml2::XML_NO_ERROR != err) { return false; }
    
    definition.create(id, size);
    
    // Animation
    bool ret = true;
    tinyxml2::XMLElement *animElement = element->FirstChildElement("animation");
    for(unsigned int i=0; ret && (i<size) && animElement; i++)
    {
        ret = readAnimation(animElement, definition.getAnimation(i));
        animElement = animElement->NextSiblingElement("animation");
    }
    
    return ret;
}

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
    
    // -- Image filnames
    char const* imageFilenames = root->Attribute("path");
    if(NULL == imageFilenames)
    {
        Log_Error(Framework::Module::Render, "Invalid path to texture");
        return false;
    }

    loadTextures(imageFilenames); // [todo]
    
    // -- Definition count
    unsigned int size;
    err = root->QueryUnsignedAttribute("size", &size);
    if(tinyxml2::XML_NO_ERROR != err)
    {
        Log_Error(Framework::Module::Render, "An error occured while parsing %s: %s (%s)", filename.c_str(), xml.GetErrorStr1(), xml.GetErrorStr2());
        return false;
    }
    
    Log_Info(Framework::Module::Render, "Prepare to process %d sprite definition(s).", size);
    _definitions.resize(size);
    
    // Definitions
    bool ret = true;
    tinyxml2::XMLElement *defElement = root->FirstChildElement("definition");
    for(unsigned int i=0; ret && (i<size) && defElement; i++)
    {
        ret = readDefinition(defElement, _definitions[i]);
        defElement = defElement->NextSiblingElement("definition");
    }
    
    if(!ret)
    {
        Log_Error(Framework::Module::Render, "An error occured while parsing %s: %s (%s)", filename.c_str(), xml.GetErrorStr1(), xml.GetErrorStr2());
        return false;
    }
    
    return true;
}

void Atlas::loadTextures(const char *filename)
{
    // Parse all texture path.
    // The paths are separated by a semi-colon ';'.
    std::stringstream ss(filename);
    std::vector<std::string> items;

    int width = -1;
    int height = -1;
    int currentWidth;
    int currentHeight;
    int channels; // Should be 4. Else, rejects.
    
    std::string item;
    while (std::getline(ss, item, ';'))
    {
        items.push_back(item);
    }

    bool ret = true;
    unsigned int count = items.size();            
    for(size_t i=0; ret && (i<items.size()); i++)
    {
        unsigned char* data;
        
        Log_Info(Framework::Module::Render, "Loading %s", items[i].c_str());
        
        data = SOIL_load_image(items[i].c_str(), &currentWidth, &currentHeight, &channels, SOIL_LOAD_AUTO);
        if(NULL == data)
        {
            Log_Error(Framework::Module::Render, "Unable to load image %s!", items[i].c_str());
            ret = false;
            break;
        }
        
        if(channels != 4)
        {
            Log_Error(Framework::Module::Render, "Incorrect channel size = %d", channels);
            ret = false;
        }
    
        if(width == -1)
        {
            width  = currentWidth;
            height = currentHeight;
            ret = _texture.create(glm::ivec2(width, height), Framework::Texture::PixelFormat::RGBA_8, count);
        }
        else if((width != currentWidth) || (height != currentHeight))
        {
            Log_Error(Framework::Module::Render, "Textures are not of the same size !");
            ret = false;
        }

        if(ret)
        {
            _texture.setData(data, i);
        }

        SOIL_free_image_data(data);
    }

    if(ret)
    {
        _texture.bind();
            _texture.setMinFilter(Framework::Texture::MinFilter::LINEAR_TEXEL);
            _texture.setMagFilter(Framework::Texture::MagFilter::LINEAR_TEXEL);
            _texture.setWrap(Framework::Texture::Wrap::CLAMP_TO_EDGE, Framework::Texture::Wrap::CLAMP_TO_EDGE);
        _texture.unbind();
        
        _size.x = width;
        _size.y = height;

        Log_Info(Framework::Module::Render, "Atlas Size : %d x %d", width, height);
    }
}

} // Sprite

/*
   <atlas file="/path/to/imagefile.png;/path/to/otherfile.png;/path/to/layerfile.png" size="...">
   <!-- List of definitions -->

   <definition id="4" size="2">
       <!-- List of animations -->
       <animation id="0" size="1">
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
