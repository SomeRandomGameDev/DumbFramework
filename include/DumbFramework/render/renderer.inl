/*
 * Copyright 2015 MooZ
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
namespace Dumb   {
namespace Render {

/** Convert to OpenGL compliant value. **/
inline GLenum TestFunc::to() const
{
    switch(value)
    {
        case NEVER:
            return GL_NEVER;
        case LESS:
            return GL_LESS;
        case EQUAL:
            return GL_EQUAL;
        case LESS_EQUAL: 
            return GL_LEQUAL;
        case GREATER:
            return GL_GREATER;
        case NOT_EQUAL:
            return GL_NOTEQUAL;
        case GREATER_EQUAL:
            return GL_GEQUAL;
        case ALWAYS:
            return GL_ALWAYS;
        default:
            return GL_ALWAYS;
    }
}

/** Create from OpenGL value. **/
inline TestFunc TestFunc::from(GLenum test)
{
    switch(test)
    {
        case GL_NEVER:
            return TestFunc(NEVER);
        case GL_LESS:
            return TestFunc(LESS);
        case GL_EQUAL:
            return TestFunc(EQUAL);
        case GL_LEQUAL:
            return TestFunc(LESS_EQUAL);
        case GL_GREATER:
            return TestFunc(GREATER);
        case GL_NOTEQUAL:
            return TestFunc(NOT_EQUAL);
        case GL_GEQUAL:
            return TestFunc(GREATER_EQUAL);
        case GL_ALWAYS:
            return TestFunc(ALWAYS);
        default:
            return TestFunc(ALWAYS);
    }
}

/** Convert to OpenGL compliant value. **/
inline GLenum CullFace::to() const
{
    switch(value)
    {
        case BACK:
            return GL_BACK;
        case FRONT:
            return GL_FRONT;
        case FRONT_AND_BACK:
            return GL_FRONT_AND_BACK;
        default:
            return GL_BACK;
    }
}

/** Create from OpenGL value. **/
inline CullFace CullFace::from(GLenum mode)
{
    switch(mode)
    {
        case GL_BACK:
            return CullFace(BACK);
        case GL_FRONT:
            return CullFace(FRONT);
        case GL_FRONT_AND_BACK:
            return CullFace(FRONT_AND_BACK);
        default:
            return CullFace(FRONT);
    }
}

/** Convert to OpenGL compliant value. **/
inline GLenum BlendFunc::to() const
{
    switch(value)
    {
        case ZERO:
            return GL_ZERO;
        case ONE:
            return GL_ONE;
        case SRC_COLOR:
            return GL_SRC_COLOR;
        case ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case DST_COLOR:
            return GL_DST_COLOR;
        case ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case SRC_ALPHA:
            return GL_SRC_ALPHA;
        case ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case DST_ALPHA:
            return GL_DST_ALPHA;
        case ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case SRC_ALPHA_SATURATE:
            return GL_SRC_ALPHA_SATURATE;
        default:
            return GL_ONE;
    }
}

/** Create from OpenGL value. **/
inline BlendFunc BlendFunc::from(GLenum mode)
{
    BlendFunc blend;
    switch(mode)
    {
        case GL_ZERO:
            return BlendFunc(ZERO);
        case GL_ONE:
            return BlendFunc(ONE);
        case GL_SRC_COLOR:
            return BlendFunc(SRC_COLOR);
        case GL_ONE_MINUS_SRC_COLOR:
            return BlendFunc(ONE_MINUS_SRC_COLOR);
        case GL_DST_COLOR:
            return BlendFunc(DST_COLOR);
        case GL_ONE_MINUS_DST_COLOR:
            return BlendFunc(ONE_MINUS_DST_COLOR);
        case GL_SRC_ALPHA:
            return BlendFunc(SRC_ALPHA);
        case GL_ONE_MINUS_SRC_ALPHA:
            return BlendFunc(ONE_MINUS_SRC_ALPHA);
        case GL_DST_ALPHA:
            return BlendFunc(DST_ALPHA);
        case GL_ONE_MINUS_DST_ALPHA:
            return BlendFunc(ONE_MINUS_DST_ALPHA);
        case GL_CONSTANT_COLOR:
            return BlendFunc(CONSTANT_COLOR);
        case GL_ONE_MINUS_CONSTANT_COLOR:
            return BlendFunc(ONE_MINUS_CONSTANT_COLOR);
        case GL_CONSTANT_ALPHA:
            return BlendFunc(CONSTANT_ALPHA);
        case GL_ONE_MINUS_CONSTANT_ALPHA:
            return BlendFunc(ONE_MINUS_CONSTANT_ALPHA);
        case GL_SRC_ALPHA_SATURATE:
            return BlendFunc(SRC_ALPHA_SATURATE);
            break;
        default:
            return BlendFunc(ONE);
    }
}

/** Convert to OpenGL compliant value. **/
GLenum Operation::to() const
{
    switch(value)
    {
        case KEEP:
            return GL_KEEP;
        case ZERO:
            return GL_ZERO;
        case REPLACE:
            return GL_REPLACE;
        case INCREMENT:
            return GL_INCR;
        case INCREMENT_WRAP:
            return GL_INCR_WRAP;
        case DECREMENT:
            return GL_DECR;
        case DECREMENT_WRAP:
            return GL_DECR_WRAP;
        case INVERT:
            return GL_INVERT;
        default:
            return GL_KEEP;
    }
}

/** Create from OpenGL value. **/
Operation Operation::from(GLenum test)
{
    switch(test)
    {
        case GL_KEEP:
            return Operation(KEEP);
        case GL_ZERO:
            return Operation(ZERO);
        case GL_REPLACE:
            return Operation(REPLACE);
        case GL_INCR:
            return Operation(INCREMENT);
        case GL_INCR_WRAP:
            return Operation(INCREMENT_WRAP);
        case GL_DECR:
            return Operation(DECREMENT);
        case GL_DECR_WRAP:
            return Operation(DECREMENT_WRAP);
        case GL_INVERT:
            return Operation(INVERT);
        default:
            return Operation(KEEP);

    }
}

} // Render
} // Dumb
