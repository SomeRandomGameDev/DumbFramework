#include <DumbFramework/runner.hpp>
#include <DumbFramework/font.hpp>
#include <DumbFramework/file.hpp>
#include <DumbFramework/log.hpp>

/**
 * Demo : Minimal font engine example.
 */
class Example {
    DECLARE_WRAPPER_METHODS
    public:
        /**
         * Default Constructor.
         */
        Example() : _closeFlag(1), _engine(0), _frames(0), _elapsed(0) {
            // Nothing special to do.
        }

        /**
         * The mandatory destructor.
         */
        ~Example() {
            if(0 != _engine) {
                delete _engine;
            }
        }
    private:
        /**
         * If 0, close the app.
         */
        int _closeFlag;

        /**
         * Font engine.
         */
        Dumb::Font::Engine *_engine;

        /**
         * Screen size (in pixels).
         */
        glm::vec2 _screenSize;

        /**
         * Number of drawn frame.
         */
        unsigned long long int _frames;

        /**
         * Time in frame rendering.
         */
        double _elapsed;

        /**
         * Application starting time.
         */
        double _start;
};

#define DFE_DEBUG

void Example::init(Dumb::Core::Application::Adviser *adviser) {
#ifndef DFE_DEBUG
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode mode = monitor.getCurrentMode();
    adviser->setVideoMode(mode);
#else
    Dumb::Core::Application::Video::Mode mode(glm::vec2(640, 480), glm::vec3(8, 8, 8), 60);
    Dumb::Core::Application::Video::Monitor monitor(0);
    adviser->setMonitor(monitor);
    adviser->setVideoMode(mode);
#endif
    _screenSize = mode.getResolution();
    adviser->setTitle("Font Test");
}

void Example::postInit() {
    using namespace Dumb::Font;
    std::string fontPath = Framework::File::executableDirectory() + "/resources/fonts/";
    // Now, load the font and build the font atlas.
    std::vector<Range> range;
    std::vector<Oversample> oversample;
    std::vector<Resource> resource;
    // Load Vera.
    range.push_back(Range("Vera-12", 32, 95, 12.0));
    range.push_back(Range("Vera-24", 32, 95, 24.0));
    oversample.push_back(Oversample(glm::vec2(1, 1), range));
    range.clear();
    range.push_back(Range("Vera-16-ovr", 32, 95, 16.0));
    range.push_back(Range("Vera-24-ovr", 32, 95, 24.0)); 
    oversample.push_back(Oversample(glm::vec2(4, 4), range));
    resource.push_back(Resource(fontPath + "Vera.ttf", oversample));
    // Load Vera italic.
    range.clear();
    oversample.clear();
    range.push_back(Range("Vera-Italic", 32, 95, 16.0));
    oversample.push_back(Oversample(glm::vec2(4, 4), range));
    resource.push_back(Resource(fontPath + "VeraIt.ttf", oversample));

    _engine = new Engine(resource, 2048, 4096);

    _start = glfwGetTime();
}

int Example::render() {
    double startTime = glfwGetTime();
    glViewport(0, 0, _screenSize.x, _screenSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
    renderer.depthTest(false);
    renderer.culling(false);
    renderer.texture2D(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::SRC_ALPHA,
            Framework::Render::BlendFunc::ONE_MINUS_SRC_ALPHA);
    _engine->setViewport(_screenSize.x, _screenSize.y);
    icu::UnicodeString toPrint("This is a [Super] test");
    const Dumb::Font::Wrapper *font = _engine->getFont("Vera-16-ovr");
    const Dumb::Font::Wrapper *italic = _engine->getFont("Vera-Italic");
    const Dumb::Font::Wrapper *big = _engine->getFont("Vera-24-ovr");
    _engine->print(font, glm::vec2(50, 50), toPrint, glm::fvec3(0.2f, 0.5f, 0.9f));

    glm::vec3 colorWhite = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 colorRed = glm::vec3(1.0f, 0.3f, 0.3f);
    glm::vec3 colorGreen = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 colorBlue = glm::vec3(0.5f, 0.5f, 1.0f);

    icu::UnicodeString decorated("This is a decorated text with a big font here.");
    _engine->print(font, glm::vec2(50, 150), decorated, colorWhite,
            { Dumb::Font::Decoration(glm::vec2(0, 4), 0, &colorGreen, false, false),
            Dumb::Font::Decoration(glm::vec2(10, 9), italic, 0, false, false),
            Dumb::Font::Decoration(glm::vec2(32, 13), big, &colorRed, false, false) });

    icu::UnicodeString interleaved("Let's try blended decoration.");
    _engine->print(font, glm::vec2(50, 250), interleaved, colorWhite,
            { Dumb::Font::Decoration(glm::vec2(9, 10), 0, &colorBlue, false, false),
            Dumb::Font::Decoration(glm::vec2(4, 10), big, 0, false, false) });

    _elapsed += glfwGetTime() - startTime;
    ++_frames;
    return _closeFlag;
}

void Example::handleUnicodeCharacter(unsigned int) {}
void Example::handleUnicodeModifierCharacter(unsigned int,int) {}
void Example::handleKey(int,int,int,int) {
    if(_frames > 0) {
        Log_Info(Framework::Module::Render, "Time per frame : %.0fns",
                ((_elapsed * 1000000000) / (double) _frames));
        double elapsed = glfwGetTime() - _start;
        Log_Info(Framework::Module::Render, "FPS : %.3f",
                _frames / elapsed);
    }
    _closeFlag = 0;
}
void Example::handleMousePosition(double xp,double yp) {}
void Example::handleMouseButton(int,int,int) {}
void Example::handleMouseScroll(double,double) {}
void Example::handleWindowClose() {}
void Example::handleWindowSize(int, int) {}
void Example::handleWindowIconify(int) {}
void Example::close() {}

SIMPLE_APP(Example)
