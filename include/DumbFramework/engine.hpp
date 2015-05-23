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
         * void init(Framework::Render::Program &);
         * void update(Framework::Render::Program &);
         * GLsizei update(void *, ...);
         * This last method is in fact a set of methods which first argument
         * is a pointer to the vertex buffer mapping and other arguments are
         * variadic. Hence, it's up to the delegation to implement whatever
         * is wanted. It will reflect what is passed to the render method.
         */
        template <typename T> class Engine {
            public:

                /**
                 * Default constructor.
                 * @param [in] delegate Delegation object.
                 * @param [in] shaders Set of shaders (pair of shader type and definition).
                 * @param [in] attributes Vertex stream attributes.
                 * @param [in] capacity Engine capacity (in buffer objects).
                 */
                Engine(T *delegate,
                        std::initializer_list< std::pair<Framework::Render::Shader::Type, const char*> > const& shaders,
                        std::initializer_list< std::pair<unsigned int, Framework::Render::Geometry::Attribute> > const& attributes,
                        GLenum primitive,
                        unsigned int capacity = DUMB_CORE_ENGINE_CAPACITY_DEFAULT,
                        bool instance = false,
                        GLsizei count = 4) : _delegate(delegate), _mode(primitive), _instanced(instance), _cardinality(count) {
                    // First, determine the stride.
                    unsigned int stride = 0;
                    for(auto &i : attributes) {
                        Framework::Render::Geometry::Attribute &attribute = i.second;
                        if(attribute.stride > stride) {
                            stride = attribute.stride; // It should not differ from one attribute to another.
                        }
                    }
                    // Initialize buffer.
                    _buffer.create(stride*capacity);
                    // Create and initialize stream.
                    _stream.create();
                    _stream.add(&_buffer, attributes);
                    _stream.compile();
                    // Create program and initialize program.
                    _program.create();
                    _program.infoLog(Severity::Info);
                    for(auto &i : shaders) {
                        Framework::Render::Shader shader;
                        shader.create(i.first, i.second);
                        shader.infoLog(Severity::Info);
                        _program.attach(shader);
                    }
                    _program.link();
                    _delegate->init(_program);
                }

                /**
                 * Variadic render.
                 * @param [in] args Variadic arguments.
                 */
                template <typename... A> void render(A... args) {
                    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
                    renderer.depthBufferWrite(false);
                    _program.begin();
                    _delegate->update(_program); // Delegate the program update.
                    _buffer.bind();
                    void *ptr = _buffer.map(Framework::Render::BufferObject::Access::Policy::WRITE_ONLY);
                    count = _delegate->update(ptr, args...); // Delegate the buffer update.
                    _buffer.unmap();
                    _buffer.unbind();
                    renderer.setActiveTextureUnit(0);
                    glBindTexture(GL_TEXTURE_2D, _atlas);
                    _stream.bind();
                    if(_instanced) {
                        glDrawArraysInstanced (_mode, 0, _cardinality, count);
                    } else {
                        glDrawArrays(_mode, 0, count);
                    }
                    _stream.unbind();
                    _program.end();
                    glBindTexture(GL_TEXTURE_2D, 0);
                    renderer.depthBufferWrite(true);
                }

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
        };
    } // 'Core' namespace.
} // 'Dumb' namespace.

#endif
