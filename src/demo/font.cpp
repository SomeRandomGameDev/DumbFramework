#include <DumbFramework/runner.hpp>
#include <DumbFramework/font.hpp>
#include <DumbFramework/file.hpp>
#include <DumbFramework/log.hpp>

#include <random>

// Some Constants.
static const glm::vec3 COLOR_WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
static const glm::vec3 COLOR_RED   = glm::vec3(1.0f, 0.3f, 0.3f);
static const glm::vec3 COLOR_GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 COLOR_BLUE  = glm::vec3(0.5f, 0.5f, 1.0f);


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
            delete _cache;
            std::vector<const Dumb::Font::Cache *>::const_iterator it;
            for(it = _collection.begin(); it != _collection.end(); ++it) {
                Dumb::Font::Cache *cache = const_cast<Dumb::Font::Cache *>(*it);
                delete cache;
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

        /**
         * 'Normal' font.
         */
        const Dumb::Font::Wrapper *_normal;

        /**
         * 'Italic' font.
         */
        const Dumb::Font::Wrapper *_italic;

        /**
         * 'Big' font.
         */
        const Dumb::Font::Wrapper *_big;

        /**
         * Single cached text.
         */
        Dumb::Font::Cache *_cache;

        /**
         * Text cache collection.
         */
        std::vector<const Dumb::Font::Cache *> _collection;
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

    _engine = new Engine(resource, 1048576, 4096);
    _engine->setViewport(_screenSize.x, _screenSize.y);
    _normal = _engine->getFont("Vera-16-ovr");
    _italic = _engine->getFont("Vera-Italic");
    _big = _engine->getFont("Vera-24-ovr");

    icu::UnicodeString decorated("This is a decorated text with a big font here.");
    _cache = new Dumb::Font::Cache(_normal, glm::vec2(50, 150), decorated, COLOR_WHITE,
            { Dumb::Font::Decoration(glm::vec2(0, 4), 0, &COLOR_GREEN, false, false),
            Dumb::Font::Decoration(glm::vec2(10, 9), _italic, 0, false, false),
            Dumb::Font::Decoration(glm::vec2(32, 13), _big, &COLOR_RED, false, false) },
            _engine->size());

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distX(0,_screenSize.x - 100);
    std::uniform_int_distribution<int> distY(32,_screenSize.y);
    for(int i = 0; i < 20; ++i) {
        Dumb::Font::Cache *newCache = new Dumb::Font::Cache(*_cache);
        newCache->moveTo(glm::vec2(distX(generator), distY(generator)));
        _collection.push_back(newCache);
    }

    Dumb::Font::Cache *changeText = new Dumb::Font::Cache(*_cache);
    changeText->moveTo(glm::vec2(0, 240));
    changeText->setText(icu::UnicodeString("It's a new text with the same decoration."), true);
    // Append some text.
    changeText->append(icu::UnicodeString(" And appending some new text."));
    // Remove 5 glyphs.
    changeText->remove(5);
    // Append a new text.
    changeText->append(icu::UnicodeString("stuff."));
    _collection.push_back(changeText);

    glViewport(0, 0, _screenSize.x, _screenSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
    renderer.depthTest(false);
    renderer.culling(false);
    renderer.texture2D(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::SRC_ALPHA,
            Framework::Render::BlendFunc::ONE_MINUS_SRC_ALPHA);

    _start = glfwGetTime();
}

int Example::render() {
    double startTime = glfwGetTime();

    _engine->print(_collection);

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
