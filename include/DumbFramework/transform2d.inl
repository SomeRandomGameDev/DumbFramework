namespace Framework {

/**
 * Retrieve up vector.
 */
glm::vec2 Transform2D::up() const
{
    return glm::vec2(cos(angle), sin(angle));
}
/**
 * Retrieve right vector.
 */
glm::vec2 Transform2D::right() const
{
    return glm::vec2(-sin(angle), cos(angle));
}
/**
 * Compute transform matrix.
 */
glm::mat3 Transform2D::local() const
{
    return _local;
}
/**
 * Compute transform matrix.
 */
void Transform2D::update()
{
    float cs = scale.x * cos(angle);
    float sn = scale.y * sin(angle);
    _local = glm::mat3(  cs,  -sn, position.x,
                         sn,   cs, position.y,
                       0.0f, 0.0f, 1.0f );
}

} // Framework

