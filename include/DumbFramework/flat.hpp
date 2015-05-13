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
#ifndef _DUMBFRAMEWORK_CORE_FLAT_ENGINE_
#define _DUMBFRAMEWORK_CORE_FLAT_ENGINE_

#include <vector>
#include <initializer_list>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/program.hpp>

namespace Dumb {
    namespace Core {
        namespace Engine {

            // TODO Primitive setting, the engine itself.

            /**
             * Layer. Defines the memory structure (hence the stride),
             * the shaders and so on.
             */
            class Layer {
                public:
                    /**
                     * Default constructor.
                     */
                    Layer();

                    /**
                     * Full constructor.
                     * @param [in] capacity Maximum number of manageable artifacts.
                     * @param [in] stride Buffer objects stride (in bytes).
                     * @param [in] descriptor Description of the data to be stored.
                     * @param [in] shaders List of shaders.
                     * @param [in] priority Layer Priority. The higher, the more priority allocated.
                     */
                    Layer(unsigned int capacity,
                            unsigned int stride,
                            std::initializer_list<std::pair<unsigned int,
                            Framework::Render::Geometry::Attribute>> const& descriptor,
                            std::initializer_list<std::pair<unsigned int,
                            const char *>> const& shaders,
                            unsigned int prority = 0);

                    /**
                     * Render.
                     */
                    void render();

                protected:
                    // FIXME Gosh, I hate this ! Find a way to fix that with template mechanism.

                    /**
                     * Pre-processing. Invoked just before the rendering itself.
                     * @return 'true' if a buffer update is needed.
                     */
                    virtual bool process() = 0;

                    /**
                     * Prepare the rendering with the help of the shader program.
                     * @param [in] program Program.
                     */
                    virtual void prepare(Framework::Render::Program &program) = 0;

                    /**
                     * Update the buffer.
                     * @param [in] buffer Buffer to update.
                     */
                    virtual void update(void *buffer) = 0;

                protected:
                    /**
                     * Number of artifacts to draw at render time.
                     */
                    GLsizei _artifacts;

                private:
                    /**
                     * Vertex Stream.
                     */
                    Framework::Render::VertexStream _stream;
                    /**
                     * Vertex Buffer Object.
                     */
                    Framework::Render::VertexBuffer _buffer;
                    /**
                     * GLSL Program identifier.
                     */
                    Framework::Render::Program _program;
                    /**
                     * Instanciation flag.
                     */
                    bool _instanced;
                    /**
                     * Instance count (if needed).
                     */
                    GLsizei _count;
                    /**
                     * Kind of primitive to draw.
                     */
                    GLenum _primitive;
                    /**
                     * Delegated processing.
                     */
                    Processing *_delegate;
            };

            /**
             * The 'Flat' engine manages the display of 2D
             * images layers. It shares the projection matrix/viewport,
             * shaders and VAO utilities between other engines such as
             * Sprengine or the Dumb Font Engine.
             */
            class Flat {
                public:
                    /**
                     * Set engine drawing viewport.
                     * @param x Center origin coordinate on X axis in pixel.
                     * @param y Center origin coordinate on Y axis in pixel.
                     * @param width Width in pixel.
                     * @param height Height in pixel.
                     * @param scale Scale.
                     */
                    void viewport(float x, float y,
                            unsigned int width, unsigned int height,
                            float scale = 1.0f);

                    /**
                     * Translate the viewport given a logical displacement.
                     * @param relX Displacement in world coordinate (X coordinate).
                     * @param relY Displacement in world coordinate (Y coordinate).
                     */
                    void translate(float relX, float relY);

                    /**
                     * Translate the viewport given a pixel displacement.
                     * @param relX Number of pixel in the X axis.
                     * @param relY Number of pixel in the Y axis.
                     */
                    void translate(int relX, int relY);

                    /**
                     * Adjust the viewport to the specified zooming scale at the
                     * targetd position.
                     * @param x Zoom center of interest (in pixels).
                     * @param y Zoom center of interest (in pixels).
                     * @param scale Scaling factor.
                     */
                    void zoom(int x, int y, float scale);

                    /**
                     * Render the delegated content.
                     */
                    void render();
            };

        } // 'Engine' namespace.
    } // 'Core' namespace.
} // 'Dumb' namespace.

#endif
