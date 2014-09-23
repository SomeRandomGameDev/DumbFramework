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
glm::mat3 glm::local() const
{
    float cs = scale * cos(angle);
    float sn = scale * sin(angle);
    return glm::mat3(  cs,  -sn, position.x,
                       sn,   cs, position.y,
                     O.0f, 0.0f, 1.0f );
}

} // Framework

