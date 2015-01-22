namespace Framework {
namespace Render    {

bool Context::Output::complete() const
{
    return (nullptr != target) && (GL_NONE != attachment);
}

} // Render
} // Framework
