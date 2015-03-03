namespace Framework {
namespace Render    {

/** Convert to OpenGL compliant value. **/
inline GLenum TestFunc::to() const
{
    switch(value)
    {
        case TestFunc::NEVER:
            return GL_NEVER;
        case TestFunc::LESS:
            return GL_LESS;
        case TestFunc::EQUAL:
            return GL_EQUAL;
        case TestFunc::LESS_EQUAL: 
            return GL_LEQUAL;
        case TestFunc::GREATER:
            return GL_GREATER;
        case TestFunc::NOT_EQUAL:
            return GL_NOTEQUAL;
        case TestFunc::GREATER_EQUAL:
            return GL_GEQUAL;
        case TestFunc::ALWAYS:
            return GL_ALWAYS;
        default:
            return GL_ALWAYS;
    }
}

/** Create from OpenGL value. **/
inline TestFunc TestFunc::from(GLenum test)
{
    TestFunc func;
    switch(test)
    {
        case GL_NEVER:
            func.value = TestFunc::NEVER;
            break;
        case GL_LESS:
            func.value = TestFunc::LESS;
            break;
        case GL_EQUAL:
            func.value = TestFunc::EQUAL;
            break;
        case GL_LEQUAL:
            func.value = TestFunc::LESS_EQUAL;
            break;
        case GL_GREATER:
            func.value = TestFunc::GREATER;
            break;
        case GL_NOTEQUAL:
            func.value = TestFunc::NOT_EQUAL;
            break;
        case GL_GEQUAL:
            func.value = TestFunc::GREATER_EQUAL;
            break;
        case GL_ALWAYS:
            func.value = TestFunc::ALWAYS;
            break;
        default:
            func.value = TestFunc::ALWAYS;
            break;
    }
    return func;
}

/** Convert to OpenGL compliant value. **/
inline GLenum CullFace::to() const
{
    switch(value)
    {
        case CullFace::BACK:
            return GL_BACK;
        case CullFace::FRONT:
            return GL_FRONT;
        case CullFace::FRONT_AND_BACK:
            return GL_FRONT_AND_BACK;
        default:
            return GL_BACK;
    }
}

/** Create from OpenGL value. **/
inline CullFace CullFace::from(GLenum mode)
{
    CullFace face;
    switch(mode)
    {
        case GL_BACK:
            face.value = CullFace::BACK;
            break;
        case GL_FRONT:
            face.value = CullFace::FRONT;
            break;
        case GL_FRONT_AND_BACK:
            face.value = CullFace::FRONT_AND_BACK;
            break;
        default:
            face.value = CullFace::FRONT;
            break;
    }
    return face;
}

/** Convert to OpenGL compliant value. **/
inline GLenum BlendFunc::to() const
{
    switch(value)
    {
        case BlendFunc::ZERO:
            return GL_ZERO;
        case BlendFunc::ONE:
            return GL_ONE;
        case BlendFunc::SRC_COLOR:
            return GL_SRC_COLOR;
        case BlendFunc::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendFunc::DST_COLOR:
            return GL_DST_COLOR;
        case BlendFunc::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendFunc::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case BlendFunc::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFunc::DST_ALPHA:
            return GL_DST_ALPHA;
        case BlendFunc::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendFunc::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case BlendFunc::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendFunc::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case BlendFunc::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendFunc::SRC_ALPHA_SATURATE:
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
            blend.value = BlendFunc::ZERO;
            break;
        case GL_ONE:
            blend.value = BlendFunc::ONE;
            break;
        case GL_SRC_COLOR:
            blend.value = BlendFunc::SRC_COLOR;
            break;
        case GL_ONE_MINUS_SRC_COLOR:
            blend.value = BlendFunc::ONE_MINUS_SRC_COLOR;
            break;
        case GL_DST_COLOR:
            blend.value = BlendFunc::DST_COLOR;
            break;
        case GL_ONE_MINUS_DST_COLOR:
            blend.value = BlendFunc::ONE_MINUS_DST_COLOR;
            break;
        case GL_SRC_ALPHA:
            blend.value = BlendFunc::SRC_ALPHA;
            break;
        case GL_ONE_MINUS_SRC_ALPHA:
            blend.value = BlendFunc::ONE_MINUS_SRC_ALPHA;
            break;
        case GL_DST_ALPHA:
            blend.value = BlendFunc::DST_ALPHA;
            break;
        case GL_ONE_MINUS_DST_ALPHA:
            blend.value = BlendFunc::ONE_MINUS_DST_ALPHA;
            break;
        case GL_CONSTANT_COLOR:
            blend.value = BlendFunc::CONSTANT_COLOR;
            break;
        case GL_ONE_MINUS_CONSTANT_COLOR:
            blend.value = BlendFunc::ONE_MINUS_CONSTANT_COLOR;
            break;
        case GL_CONSTANT_ALPHA:
            blend.value = BlendFunc::CONSTANT_ALPHA;
            break;
        case GL_ONE_MINUS_CONSTANT_ALPHA:
            blend.value = BlendFunc::ONE_MINUS_CONSTANT_ALPHA;
            break;
        case GL_SRC_ALPHA_SATURATE:
            blend.value = BlendFunc::SRC_ALPHA_SATURATE;
            break;
        default:
            blend.value = BlendFunc::ONE;
            break;
    }
    return blend;
}

} // Render
} // Framework
