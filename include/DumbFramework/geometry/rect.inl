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
 * Compute minimum point.
 */
glm::vec2 Rect::min() const
{
    return position;
}
/**
 * Compute maximum point.
 */
glm::vec2 Rect::max() const
{
    return position + size;
}
/**
 * Compute center.
 */
glm::vec2 Rect::center() const
{
    return position + (size / 2.0f);
}
/**
 * Compute extent.
 */
glm::vec2 Rect::extent() const
{
    return size / 2.0f;
}

} // Geometry
} // Core
} // Dumb
