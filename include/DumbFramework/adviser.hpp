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
#ifndef _DUMBFRAMEWORK_ADVISER_
#define _DUMBFRAMEWORK_ADVISER_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace Dumb {
    namespace Core {
        namespace Application {
            namespace Video {
                /**
                 * Video Mode.
                 */
                class Mode {
                    public:
                        /**
                         * Default constructor.
                         */
                        Mode() : resolution(0, 0), depth(0, 0, 0), rate(0) {
                            // Nothing to be done.
                        }

                        /**
                         * Constructor.
                         * @param [in] res Resolution (in pixels).
                         * @param [in] dep Channels depth (in bits).
                         * @param [in] rt Rate (in Hz).
                         */
                        Mode(glm::vec2 res, glm::vec3 dep, int rt) :
                            resolution(res),
                            depth(dep),
                            rate(rt) {
                                // Nothing to be done.
                            }

                        /**
                         * Constructor.
                         * @param [in] mode Video mode descriptor (from GLFW).
                         */
                        Mode(const GLFWvidmode *mode);

                        /**
                         * Provides the resolution (in pixels).
                         * @return The resolution (in pixels).
                         */
                        inline glm::vec2 getResolution() const {
                            return resolution;
                        }

                        /**
                         * Provides the depth of the RGB channel (in bits).
                         * @return Channels depth (in bits).
                         */
                        inline glm::vec3 getDepth() const {
                            return depth;
                        }

                        /**
                         * Provides the refresh rate (in Hz).
                         * @return Refresh rate (in Hz).
                         */
                        inline int getRate() const {
                            return rate;
                        }

                    private:
                        /**
                         * Resolution (in pixels).
                         */
                        glm::vec2 resolution;

                        /**
                         * Channels depth (in bits).
                         */
                        glm::vec3 depth;

                        /**
                         * Refresh rate (in Hz).
                         */
                        int rate;
                };

                /**
                 * Monitor definition.
                 */
                class Monitor {
                    public:
                        /**
                         * Default constructor.
                         */
                        Monitor() : descriptor(0) {
                            // Nothing to be done.
                        }

                        /**
                         * Constructor.
                         * @param [in] monitor Monitor descriptor (from GLFW).
                         */
                        Monitor(GLFWmonitor *monitor);

                        /**
                         * Provides the physical size of the monitor in millimeters.
                         * @return Physical size in millimeters.
                         */
                        inline glm::vec2 getSize() const {
                            return size;
                        }

                        /**
                         * Provides the current video mode.
                         * @return Video mode.
                         */
                        inline const Mode getCurrentMode() const {
                            return current;
                        }

                        /**
                         * Provides the (possibly empty) name of the monitor.
                         * @return (A copy of the) Name of the monitor.
                         */
                        inline const std::string getName() const {
                            return name;
                        }

                        /**
                         * Fetches the available video modes.
                         * The specified container is emptied and populated with
                         * the available video modes (if any).
                         * @param [out] buffer Container in which to store the video modes.
                         */
                        void fetchModes(std::vector<Mode> *buffer) const;

                        /**
                         * Provides access to the monitor descriptor.
                         * @return Monitor descriptor as defined by GLFW.
                         */
                        inline GLFWmonitor *getMonitor() const {
                            return descriptor;
                        }

                    private:
                        /**
                         * Monitor name.
                         */
                        std::string name;

                        /**
                         * Monitor size (in mm).
                         */
                        glm::vec2 size;

                        /**
                         * Current video modes.
                         */
                        Mode current;

                        /**
                         * List of video modes.
                         */
                        std::vector<Mode> modes;

                        /**
                         * Attached monitor descriptor.
                         */
                        GLFWmonitor *descriptor;
                };

            } // 'Video' namespace.

            /**
             * Application Adviser.
             * Provides information about system capabilities.
             * Collects prefered configuration for application launch.
             */
            class Adviser {
                public:
                    /**
                     * Default constructor.
                     */
                    Adviser() : title("DumbFramework Powered") {
                        // Nothing to do.
                    }

                    /**
                     * Retrieves the primary monitor.
                     * @return Primary monitor descriptor.
                     */
                    Video::Monitor getPrimaryMonitor() const;

                    /**
                     * Fetches the available monitors.
                     * The provided container is emptied and populated with
                     * the available monitor descriptions.
                     * @param [out] buffer Container in witch to store
                     *                     monitor information.
                     */
                    void fetchMonitors(std::vector<Video::Monitor> *buffer) const;

                    /**
                     * Specifies the prefered video mode for the application window.
                     * @param [in] mode Prefered video mode.
                     */
                    inline void setVideoMode(Video::Mode mode) {
                        this->mode = mode;
                    }

                    /**
                     * Specifies the prefered monitor. If not specified, the application
                     * will open in the main monitor.
                     * @param [in] monitor Prefered monitor. Can be null to set
                     *                     preference to the default one.
                     */
                    inline void setMonitor(Video::Monitor monitor) {
                        this->monitor = monitor;
                    }

                    /**
                     * Provides the desired video mode.
                     * @return Video mode.
                     */
                    inline Video::Mode getVideoMode() const {
                        return mode;
                    }

                    /**
                     * Provides the desired monitor.
                     * @return Monitor.
                     */
                    inline Video::Monitor getMonitor() {
                        return monitor;
                    }

                    /**
                     * Set the title of the window.
                     * @param [in] ttl Desired window title.
                     */
                    void setTitle(const char *ttl) {
                        title = ttl;
                    }

                    /**
                     * Get the title of the window.
                     * @return The title.
                     */
                    const std::string getTitle() const {
                        return title;
                    }

                private:
                    /**
                     * Prefered monitor.
                     */
                    Video::Monitor monitor;

                    /**
                     * Prefered video mode.
                     */
                    Video::Mode mode;

                    /**
                     * Window title.
                     */
                    std::string title;
            };
        } // 'Application' namespace.
    } // 'Core' namespace.
} // 'Dumb' namespace.

#endif
