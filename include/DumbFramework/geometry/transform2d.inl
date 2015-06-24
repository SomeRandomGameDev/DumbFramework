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
namespace Dumb     {
namespace Core     {
namespace Geometry {

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

} // Geometry
} // Core
} // Dumb
