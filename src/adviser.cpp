#include <DumbFramework/adviser.hpp>

namespace Dumb {
    namespace Core {
        namespace Application {
            namespace Video {

                Mode::Mode(const GLFWvidmode *mode) {
                    resolution = glm::vec2(mode->width, mode->height);
                    depth = glm::vec3(mode->redBits, mode->greenBits, mode->blueBits);
                    rate = mode->refreshRate;
                }

                Monitor::Monitor(GLFWmonitor *monitor) {
                    if(0 != monitor) {
                        const char *nm = glfwGetMonitorName(monitor);
                        name = nm;
                        int width;
                        int height;
                        glfwGetMonitorPhysicalSize(monitor, &width, &height);
                        size = glm::vec2(width, height);
                        current = Mode(glfwGetVideoMode(monitor));
                        int modeCount;
                        const GLFWvidmode *vidmods = glfwGetVideoModes(monitor, &modeCount);
                        for(int i = 0; i < modeCount; ++i) {
                            modes.push_back(Mode(vidmods + i));
                        }
                    }
                    descriptor = monitor;
                }

                void Monitor::fetchModes(std::vector<Mode> *buffer) const {
                    if(0 != buffer) {
                        buffer->clear();
                        *buffer = modes;
                    }
                }
            } // 'Video' namespace.

            Video::Monitor Adviser::getPrimaryMonitor() const {
                return Video::Monitor(glfwGetPrimaryMonitor());
            }

            void Adviser::fetchMonitors(std::vector<Video::Monitor> *buffer) const {
                if(0 != buffer) {
                    buffer->clear();
                    int monitorCount;
                    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
                    for(int i = 0; i < monitorCount; ++i) {
                        buffer->push_back(Video::Monitor(monitors[i]));
                    }
                }
            }

        } // 'Application' namespace.
    } // 'Core' namespace.
} // 'Dumb' namespace.
