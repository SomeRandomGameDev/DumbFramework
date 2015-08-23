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
#ifndef _DUMBFRAMEWORK_ENGINE_
#define _DUMBFRAMEWORK_ENGINE_

#include <vector>
#include <map>
#include <tuple>
#include <initializer_list>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/renderer.hpp>

#define DUMB_CORE_ENGINE_CAPACITY_DEFAULT 1024576

/* Dumb Core Engine definition.
 * A simple rendering engine implementing the 'Engine' concept.
 */
namespace Dumb {
    namespace Core {

        /**
         * Dumb Core Engine.
         * The processing delegation must implement the following concept:
         * Constructor(...);
         * std::vector<std::pair<Framework::Render::Shader::Type, const char *> > shaders() const;
         * std::vector<std::pair<unsigned int, Framework::Render::Geometry::Attribute> > attributes() const;
         * GLenum primitive() const;
         * bool isInstanced() const;
         * void viewport(GLfloat, GLfloat, GLfloat, GLfloat);
         * GLsizei instanceCardinality() const;
         * void init(Framework::Render::Program &);
         * void update(Framework::Render::Program &);
         * GLsizei update(void *, GLsizei, ...);
         * void postRender();
         * This last method is in fact a set of methods which first argument
         * is a pointer to the vertex buffer mapping and other arguments are
         * variadic. Hence, it's up to the delegation to implement whatever
         * is wanted. It will reflect what is passed to the render method.
         */
        template <typename T> class Engine {
            public:

                /**
                 * Default constructor.
                 * @param [in] capacity Engine capacity in number of managed objects.
                 * @param [in] args Variable number of argument.
                 * A Delegate Class constructor must match.
                 */
                template <typename... A> Engine(unsigned int capacity, A... args) : _count(0) {
                    // First, create the delegate object.
                    _delegate = new T(args...);
                    std::vector< std::pair<Framework::Render::Shader::Type, const char*> > shaders;
                    std::vector< std::pair<unsigned int, Framework::Render::Geometry::Attribute> > attributes;
                    shaders = _delegate->shaders();
                    attributes = _delegate->attributes();
                    _mode = _delegate->primitive();
                    _instanced = _delegate->isInstanced();
                    if(_instanced) {
                        _cardinality = _delegate->instanceCardinality();
                    }
                    // Then, determine the stride.
                    unsigned int stride = 0;
                    for(auto &i : attributes) {
                        Framework::Render::Geometry::Attribute &attribute = i.second;
                        if(attribute.stride > stride) {
                            stride = attribute.stride; // It should not differ from one attribute to another.
                        }
                    }
                    // Initialize buffer.
                    _capacity = capacity;
                    _buffer.create(stride*capacity);
                    // Create and initialize stream.
                    _stream.create();
                    for(auto &i : attributes) {
                        _stream.add(&_buffer, i.first, i.second);
                    }
                    _stream.compile();
                    // Create program and initialize program.
                    _program.create();
                    _program.infoLog(Dumb::Severity::Info);
                    for(auto &i : shaders) {
                        Framework::Render::Shader shader;
                        shader.create(i.first, i.second);
                        shader.infoLog(Dumb::Severity::Info);
                        _program.attach(shader);
                    }
                    _program.link();
                    _delegate->init(_program);
                }

                /**
                 * Interface method for viewport setting.
                 * @param [in] startX Viewport starting point on X-axis.
                 * @param [in] startY Viewport starting point on Y-axis.
                 * @param [in] width Width.
                 * @param [in] height Height.
                 */
                void viewport(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
                    _delegate->viewport(x, y, width, height);
                }

                /**
                 * Destructor.
                 */
                ~Engine() {
                    delete _delegate;
                }

                /**
                 * @return The delegation object.
                 */
                inline T &delegate() { return *_delegate; }

                /**
                 * Variadic update (no render).
                 * @param [in] args Variadic arguments.
                 */
                template <typename... A> void update(A... args) {
                    _buffer.bind();
                    void *ptr = _buffer.map(Framework::Render::BufferObject::Access::Policy::WRITE_ONLY);
                    GLsizei count = _delegate->update(ptr, _capacity, args...);
                    _count = count;
                    _buffer.unmap();
                    _buffer.unbind();
                }

                /**
                 * Variadic render.
                 * @param [in] args Variadic arguments.
                 */
                template <typename... A> void render(A... args) {
                    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
                    renderer.depthBufferWrite(false);
                    _program.begin();
                    renderer.setActiveTextureUnit(0);
                    // Delegate the program update.
                    _delegate->update(_program);
                    _buffer.bind();
                    void *ptr = _buffer.map(Framework::Render::BufferObject::Access::Policy::WRITE_ONLY);
                    // Delegate the buffer update.
                    GLsizei count = _delegate->update(ptr, _capacity, args...);
                    _count = count;
                    _buffer.unmap();
                    _buffer.unbind();
                    _stream.bind();
                    if(_instanced) {
                        glDrawArraysInstanced (_mode, 0, _cardinality, count);
                    } else {
                        glDrawArrays(_mode, 0, count);
                    }
                    _stream.unbind();
                    _program.end();
                    renderer.depthBufferWrite(true);
                    _delegate->postRender();
                }

                /**
                 * Simple render.
                 * Only trigger GPU for buffer rendering without update.
                 */
                void render() {
                    if(_count > 0) {
                        Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
                        renderer.depthBufferWrite(false);
                        _program.begin();
                        renderer.setActiveTextureUnit(0);
                        _delegate->update(_program); // Delegate the program update.
                        _stream.bind();
                        if(_instanced) {
                            glDrawArraysInstanced (_mode, 0, _cardinality, _count);
                        } else {
                            glDrawArrays(_mode, 0, _count);
                        }
                        _stream.unbind();
                        _program.end();
                        renderer.depthBufferWrite(true);
                        _delegate->postRender();
                    }
                }

                /**
                 * @return The engine capacity.
                 */
                inline unsigned int capacity() const { return _capacity; }
            private:
                /**
                 * Delegation object.
                 */
                T *_delegate;

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
                 * Primitive.
                 */
                GLenum _mode;

                /**
                 * 'true' if the associated program is instanced.
                 */
                bool _instanced;

                /**
                 * Instanciation cardinality.
                 */
                GLsizei _cardinality;

                /**
                 * Engine capacity.
                 */
                GLsizei _capacity;

                /**
                 * Last update count.
                 */
                GLsizei _count;
        };
    } // 'Core' namespace.
} // 'Dumb' namespace.

#endif
