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
#ifndef _DUMBFRAMEWORK_RUNNER_
#define _DUMBFRAMEWORK_RUNNER_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <DumbFramework/adviser.hpp>
#include <iostream>

#include <DumbFramework/log.hpp>

namespace Dumb {
    namespace Core {
        namespace Application {

            /**
             * Application Runner.
             * @param <T> A type implementing the wrapper concept.
             */
            template <typename T> class Runner {
                public:
                    /**
                     * Constructor.
                     * @param [in] wrapper An instance of the wrapper.
                     */
                    Runner(T *wrapper) : delegate(wrapper) {}

                    /**
                     * Start the application.
                     * @return An error code or 0 if the application exited correctly.
                     */
                    int start() {
                        // Tons of work to do.
                        // First, initialise all the libraries.
                        glfwSetErrorCallback(Runner::handleErrorMessage);
                        int returnCode = 0;

                        if(0 != delegate) {
                            returnCode = glfwInit();
                            if(0 != returnCode) {
                                Core::Application::Adviser adviser;

                                // Initialize the wrapper.
                                delegate->init(&adviser);

                                // Create the window.
                                Video::Monitor preferedMonitor = adviser.getMonitor();
                                Video::Mode mode = adviser.getVideoMode();
                                glm::vec2 size = mode.getResolution();
                                GLFWmonitor *monitor = preferedMonitor.getMonitor();
                                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                                GLFWwindow *window = glfwCreateWindow(size.x, size.y,
                                        adviser.getTitle().c_str(),
                                        monitor, 0);
                                if(0 != window) {
                                    // Set the window user pointer.
                                    glfwSetWindowUserPointer(window,
                                            static_cast<void *>(delegate));
                                    // Setting up the callbacks.
                                    glfwSetCharCallback(window, Runner::handleUnicodeCharacter);
                                    glfwSetKeyCallback(window, Runner::handleKey);
                                    glfwSetMouseButtonCallback(window,
                                            Runner::handleMouseButton);
                                    glfwSetCursorPosCallback(window,
                                            Runner::handleMousePosition);
                                    glfwSetScrollCallback(window,
                                            Runner::handleMouseScroll);
                                    glfwSetWindowSizeCallback(window,
                                            Runner::handleWindowSize);
                                    glfwSetWindowCloseCallback(window,
                                            Runner::handleWindowClose);
                                    glfwSetWindowIconifyCallback(window,
                                            Runner::handleWindowIconify);
                                    // Setting up app hints.
                                    glfwMakeContextCurrent(window);
                                    // Setting up GLEW.
                                    GLenum glewError = glewInit();
                                    if(GLEW_OK != glewError) {
                                        Log_Error(Framework::Module::App,
                                                "Can't initialize GLEW : %s",
                                                glewGetErrorString(glewError));
                                        glfwTerminate();
                                        return -1;
                                    }
                                    glfwSwapInterval(1);
                                    // Application main loop.
                                    delegate->postInit();
                                    while(!glfwWindowShouldClose(window)) {
                                        if(!delegate->render()) {
                                            glfwSetWindowShouldClose(window, GL_TRUE);
                                        }
                                        glfwSwapBuffers(window);
                                        glfwPollEvents();
                                    }
                                    glfwDestroyWindow(window);
                                } else {
                                    returnCode = -1;
                                }
                            }

                            glfwTerminate();
                        } else {
                            returnCode = -1;
                            handleErrorMessage(returnCode, "Undefined wrapper");
                        }

                        return returnCode;
                    }

                    /**
                     * Destructor.
                     */
                    ~Runner() {
                        if(0 != delegate) {
                            delegate->close();
                            delegate = 0;
                        }
                    }
                private:
                    /**
                     * Assignation operator made private to forbid it.
                     * @param [in] orig Don't care ...
                     * @return Don't care.
                     */
                    Runner &operator=(const Runner &orig) {
                        return *this;
                    }

                    /**
                     * GLFW Error message handling.
                     * @param [in] code Error code.
                     * @param [in] desc Error description.
                     */
                    static void handleErrorMessage(int code, const char *desc) {
                        Log_Error(Framework::Module::App, "%s (errcode : %d)", desc, code);
                    }

                    // Basic GLFW Callbacks.
                    static void handleUnicodeCharacter(GLFWwindow *win,
                            unsigned int codepoint) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleUnicodeCharacter(codepoint);
                    }
                    static void handleUnicodeModifierCharacter(GLFWwindow *win,
                            unsigned int codepoint,
                            int mods) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleUnicodeModifierCharacter(codepoint, mods);
                    }
                    static void handleKey(GLFWwindow *win,
                            int key, int scancode,
                            int action, int mods) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleKey(key, scancode, action, mods);
                    }
                    static void handleMousePosition(GLFWwindow *win,
                            double xpos,double ypos) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleMousePosition(xpos, ypos);
                    }
                    static void handleMouseButton(GLFWwindow *win,
                            int button,int action,int mods) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleMouseButton(button, action, mods);
                    } 
                    static void handleMouseScroll(GLFWwindow *win,
                            double xpos,double ypos) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleMouseScroll(xpos, ypos);
                    }
                    static void handleWindowClose(GLFWwindow *win) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleWindowClose();
                    }
                    static void handleWindowSize(GLFWwindow *win,
                            int width,int height) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleWindowSize(width, height);
                    }
                    static void handleWindowIconify(GLFWwindow *win,int flag) {
                        T *delegate = static_cast<T *>(glfwGetWindowUserPointer(win));
                        delegate->handleWindowIconify(flag);
                    }

                private:
                    /**
                     * Delegated object.
                     */
                    T *delegate;
            };

        } // 'Application' namespace.
    } // 'Core' namespace.
} // 'Dumb' namespace.

// Create a simple application.
#define SIMPLE_APP(W) \
    int main(void){ \
        Framework::Log::LogBuilder<Framework::Log::AllPassFilter,\
        Framework::Log::SimpleMessageFormat> msgBuilder;\
        Framework::Log::ConsoleOutputPolicy output;\
        Framework::Log::LogProcessor& processor = Framework::Log::LogProcessor::instance();\
        processor.start(&msgBuilder, &output);\
        W *w = new W();Dumb::Core::Application::Runner<W> runner(w);\
        int result = runner.start();delete w;\
        Log_Info(Framework::Module::App, "End ...");\
        processor.stop();\
        return result;\
    }

// Declare all wrapper methods.
#define DECLARE_WRAPPER_METHODS \
    public:\
void init(Dumb::Core::Application::Adviser *);\
void postInit();\
bool render();\
void handleUnicodeCharacter(unsigned int);\
void handleUnicodeModifierCharacter(unsigned int,int);\
void handleKey(int,int,int,int);\
void handleMousePosition(double,double);\
void handleMouseButton(int,int,int);\
void handleMouseScroll(double,double);\
void handleWindowClose();\
void handleWindowSize(int, int);\
void handleWindowIconify(int);\
void close();


#endif
