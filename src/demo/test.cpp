#include <runner.hpp>
#include <iostream>

/**
 * Sample application : Minimal example.
 */
class Example {
    DECLARE_WRAPPER_METHODS
    public:
        /**
         * Default Constructor.
         */
        Example() : closeFlag(1) {
            // Nothing special to do.
        }
    private:
        /**
         * If 0, close the app.
         */
        int closeFlag;
};

void Example::init(Dumb::Core::Application::Adviser *adviser) {
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode current = monitor.getCurrentMode();
    adviser->setVideoMode(current);
    adviser->setTitle("My App");
}
void Example::postInit() {}
int Example::render() { return closeFlag; }
void Example::handleUnicodeCharacter(unsigned int) {}
void Example::handleUnicodeModifierCharacter(unsigned int,int) {}
void Example::handleKey(int,int,int,int) {
    closeFlag = 0;
}
void Example::handleMousePosition(double xp,double yp) {
    std::cout << "(" << xp << ", " << yp << ")" << std::endl;
}
void Example::handleMouseButton(int,int,int) {}
void Example::handleMouseScroll(double,double) {}
void Example::handleWindowClose() {}
void Example::handleWindowSize(int, int) {}
void Example::handleWindowIconify(int) {}
void Example::close() {}



SIMPLE_APP(Example)
