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
#include <flat.hpp>

using namespace Dumb::Core::Engine;

Layer::Layer(unsigned int capacity,
        unsigned int stride,
        std::initializer_list<std::pair<unsigned int,
        Framework::Render::Geometry::Attribute>> const& descriptor,
        std::initializer_list<std::pair<unsigned int,
        const char *>> const& shaders,
        unsigned int prority = 0) {
    // FIXME Compute stride from descriptor.
    _buffer.create(stride*capacity);

    _stream.create();
    _stream.add(&_buffer, descriptor);
    _stream.compile();

    _time = glfwGetTime();

    // Create program.
    _program.create();
    for(auto &i : shaders) {
        Render::Shader shader;
        shader.create(i->first, i->second);
        shader.infoLog(Severity::Info);
        _program.attach(shader);
    }

    _program.link();
    _program.infoLog(Severity::Info);
    _uniformMatrix = _program.getUniformLocation("pMatrix");
    _delegate->init(_program);
}

void Layer::render() {
    Render::Renderer& renderer = Render::Renderer::instance();

    // Render pre-processing.
    bool updateNeeded = process();

    // Retrieve buffer and memcpy.
    renderer.depthBufferWrite(false);

    _program.begin();
    _program.uniform(_uniformMatrix, false, _matrix);
    prepare(_program);

    // Optional buffer update.
    if(updateNeeded) {
        _buffer.bind();
        void *ptr = _buffer.map(Render::BufferObject::Access::Policy::WRITE_ONLY);
        update(ptr);
        _buffer.unmap();
        _buffer.unbind();
    }

    // Send VAO.
    _stream.bind();
    if(_instanced) {
        glDrawArraysInstanced(_primitive, 0, _count, _artifacts);
    } else {
        glDrawArrays(_primitive, 0, _artifacts);
    }
    _stream.unbind();
    _program.end();
    renderer.depthBufferWrite(true);
}

