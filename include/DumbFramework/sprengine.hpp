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
#ifndef _DUMB_FW_SPRENGINE_
#define _DUMB_FW_SPRENGINE_

#include <initializer_list>

//#include <DumbFramework/sprite.hpp>
#include <DumbFramework/engine.hpp>
#include <DumbFramework/render/texture2d.hpp>

namespace Dumb {
    namespace Sprite {
        /**
         * Sprite definition (Atlas Wise).
         */
        class Sprite {
            public:
                /**
                 * Default constructor.
                 */
                Sprite() : _coordinates(0.0, 0.0, 0.0, 0.0),
                    _size(0.0, 0.0),
                    _anchor(0.0, 0.0), _layer(0) {}
                /**
                 * Bulk setter.
                 * @param [in] coord Texture coordinates.
                 * @param [in] size Sprite size in pixels.
                 * @param [in] anchor Anchor coordinates.
                 * @param [in] layer Texture layer.
                 */
                inline void set(glm::vec4 coord, glm::vec2 size,
                        glm::vec2 anchor, unsigned int layer) {
                    _coordinates = coord;
                    _size = size;
                    _anchor = anchor;
                    _layer = layer;
                }
                /**
                 * Provides textures coordinates.
                 * @return Texture coordinates (top-left, bottom-right, [0..1]).
                 */
                inline glm::vec4 getCoordinates() const { return _coordinates; }
                /**
                 * Provides sprite size.
                 * @return Sprite size (in pixels).
                 */
                inline glm::vec2 getSize() const { return _size; }
                /**
                 * Provides anchor position.
                 * @return Relative anchor position (in pixels).
                 */
                inline glm::vec2 getAnchor() const { return _anchor; }
                /**
                 * Provides texture layer.
                 * @return Layer in the texture array.
                 */
                inline unsigned int getLayer() const { return _layer; }
            private:
                /**
                 * Texture coordinates.
                 */
                glm::vec4 _coordinates;

                /**
                 * Sprite size (in pixels).
                 */
                glm::vec2 _size;

                /**
                 * Relative anchor position (from top-left).
                 */
                glm::vec2 _anchor;

                /**
                 * Texture layer in the Atlas.
                 */
                unsigned int _layer;
        };

        /**
         * Sprite Atlas.
         */
        class Atlas {
            public:
                /**
                 * Build Sprite Atlas from multiple textures.
                 * @param [in] paths List of file path.
                 */
                Atlas(const std::vector<std::string> &paths, unsigned int capacity);

                /**
                 * Destructor.
                 */
                ~Atlas();

                /**
                 * Define a sprite.
                 * @param [in] id Sprite definition identifier.
                 * @param [in] coord Texture coordinates ([0..1]).
                 * @param [in] size Sprite size in pixels.
                 * @param [in] anchor Anchor coordinates.
                 * @param [in] layer Texture layer.
                 * @return Sprite definition or null if not in capacity range.
                 */
                const Sprite *define(unsigned int id, glm::vec4 coord,
                        glm::vec2 size, glm::vec2 anchor, unsigned int layer);

                /**
                 * Define a sprite with pixel coordinates.
                 * @param [in] id Sprite definition identifier.
                 * @param [in] coord Sprite coordinates (x, y, width, height).
                 * @param [in] anchor Anchor coordinates.
                 * @param [in] layer Texture layer.
                 * @return Sprite definition or null if not in capacity range.
                 */
                const Sprite *define(unsigned int id, glm::vec4 coord,
                        glm::vec2 anchor, unsigned int layer);

                /**
                 * Retrieve sprite definition.
                 * @param [in] id Sprite definition identifier.
                 * @return Sprite definition or null if not in capacity range.
                 */
                const Sprite *get(unsigned int id) const;

                /**
                 * Bind atlas texture.
                 */
                inline void bind() const {
                    _texture.bind();
                }

                /**
                 * Unbind atlas texture.
                 */
                inline void unbind() const {
                    _texture.unbind();
                }

                /**
                 * @return Atlas size.
                 */
                inline glm::vec2 size() {
                    return _size;
                }
            private:
                /**
                 * Texture array identifier.
                 */
                Framework::Render::Texture2D _texture;

                /**
                 * Atlas Size in pixels.
                 */
                glm::vec2 _size;

                /**
                 * Sprite definitions.
                 */
                Sprite *_sprites;

                /**
                 * Capacity (in sprites).
                 */
                unsigned int _capacity;
        };

        /**
         * Sprite instance.
         * Not so many information here. Only sprite definition.
         * The rest of the information is in the associated cell.
         */
        class Instance {
            public:
                /**
                 * Default useless constructor.
                 */
                Instance() : _sprite(0), _reverse(0) {}

                /**
                 * Provide the instanciated sprite.
                 * @return Sprite definition.
                 */
                inline const Sprite *getSprite() const { return _sprite; }

                /**
                 * Provide reverse id.
                 * @return Reverse id in lookup table.
                 */
                inline unsigned int getReverse() const { return _reverse; }

                /**
                 * Bulk setter.
                 * @param [in] sprite Sprite definition.
                 * @param [in] reverse Reverse identification in the lookup table.
                 * @param [in] layer Layer.
                 */
                inline void set(const Sprite *sprite, unsigned int reverse,
                        unsigned int layer) {
                    _sprite = sprite;
                    _reverse = reverse;
                    _layer = layer;
                }

                /**
                 * Set the reverse identifier.
                 * @param [in] reverse Reverse identifier in the lookup table.
                 */
                inline void setReverse(unsigned int reverse) { _reverse = reverse; }

                /**
                 * Set the sprite definition.
                 * @param [in] sprite Sprite definition.
                 */
                inline void setSprite(const Sprite *sprite) { _sprite = sprite; }

                /**
                 * Retrieve layer.
                 * @return Layer.
                 */
                inline unsigned int getLayer() const { return _layer; }

                /**
                 * Set Layer.
                 * @param [in] layer Layer.
                 */
                void setLayer(unsigned int layer) { _layer = layer; }
            private:
                /**
                 * Sprite Definition.
                 */
                const Sprite *_sprite;

                /**
                 * Reserve identification in lookup table.
                 */
                unsigned int _reverse;

                /**
                 * Layer.
                 */
                unsigned int _layer;
        };

        /**
         * Sprite instance identifier.
         * Negative values are for invalid identifiers.
         */
        typedef int Identifier;

        /**
         * Lookup table item.
         */
        typedef struct {
            /**
             * Previous item (free or used).
             */
            int _previous;
            /**
             * Next item (free or used).
             */
            int _next;
            /**
             * Targeted sprite slot.
             */
            Identifier _target;
            /**
             * Helper flag for quick status retrieval.
             */
            bool _free;
        } LookupItem;

        /**
         * Cell to copy into 'V-Ram'.
         */
        typedef struct {
            /**
             * Position.
             */
            GLfloat _posX;
            GLfloat _posY;

            /**
             * Offset.
             */
            GLfloat _offsetX;
            GLfloat _offsetY;

            /**
             * Size.
             */
            GLfloat _sizeX;
            GLfloat _sizeY;

            /**
             * Top texel coordinate.
             */
            GLfloat _topU;
            GLfloat _topV;

            /**
             * Bottom texel coordinate.
             */
            GLfloat _bottomU;
            GLfloat _bottomV;

            /**
             * Rotation angle.
             */
            GLfloat _angle;

            /**
             * Sprite scale.
             */
            GLfloat _scale;

            /**
             * Texture layer in which the sprite actually resides in the array.
             */
            GLuint _layer;
        } Cell;

        /**
         * Sprite Cache class.
         */
        class Cache {
            public:
                /**
                 * Constructor.
                 * @param [in] atlas Sprite Atlas.
                 * @param [in] capacity Instance capacity.
                 */
                Cache(const Atlas *atlas, unsigned int capacity);

                /**
                 * Destructor.
                 */
                ~Cache();

                /**
                 * @return The number of defined instance.
                 */
                inline unsigned int count() const { return _count; }

                /**
                 * Copy the buffer content.
                 * @param [in] ptr Mapped buffer.
                 * @param [in] size Number of cell to copy.
                 */
                inline void copy(void *ptr, GLsizei size) const {
                    memcpy(ptr, _cell, size * sizeof(Cell));
                }

                /**
                 * Create a new sprite instance.
                 * @param definitionId Sprite definition identifier.
                 * @patam pos Initial position.
                 * @param angle Initial sprite rotation angle.
                 * @param scale Initial sprite scale factor.
                 * @param layer Sprite layer.
                 * @return An identifier to sprite instance or a negative value in case of
                 * invalid parameters.
                 */
                Identifier create(unsigned int definitionId, glm::vec2 pos,
                        float angle=0.0f, float scale=1.0f, unsigned int layer=0.0f);

                /**
                 * Destroy/Remove a sprite instance for the engine.
                 * @param id Identifier of the instance to remove.
                 */
                void destroy(Identifier id);

                /**
                 * Copy a sprite state into another.
                 * @param destination Destination sprite slot.
                 * @param source Source sprite slot.
                 */
                void copy(Identifier destination, Identifier source);

                /**
                 * Create a copy/clone of an existing sprite instance.
                 * @param source Source sprite instance.
                 * @return An identifier is the operation is legit.
                 */
                Identifier clone(Identifier source);

                /**
                 * Set the sprite for a given identifier.
                 * @param Id Sprite identifier.
                 * @param pos Position.
                 * @param spriteId Sprite definition identifier.
                 * @param angle Sprite rotation angle.
                 * @param [in] scale Scaling factor.
                 * @param [in] layer Layer.
                 * @return <code>false</code> in case of invalid parameters.
                 */
                bool set(Identifier id, glm::vec2 pos, unsigned int spriteId,
                        float angle=0.0f, float scale=1.0f, unsigned int layer=0.0f);

                /**
                 * Set the sprite for a given identifier.
                 * @param Id Sprite identifier.
                 * @param pos Position.
                 * @param sprite Sprite definition.
                 * @param angle Sprite rotation angle.
                 * @param [in] scale Scaling factor.
                 * @param [in] layer Layer.
                 * @return <code>false</code> in case of invalid parameters.
                 */
                bool set(Identifier id, glm::vec2 pos, const Sprite *sprite,
                        float angle=0.0f, float scale=1.0f, unsigned int layer=0.0f);


                /**
                 * Move a sprite to the specified position.
                 * @param pos Position in pixel.
                 */
                void move(Identifier id, glm::vec2 pos);

                /**
                 * Rotate a sprite to the specified angle.
                 * @param id Sprite instance identifier.
                 * @param angle Angle.
                 */
                void rotate(Identifier id, float angle);

                /**
                 * Set the sprite layer.
                 * @param id Sprite instance identifier.
                 * @param layer Sprite depth.
                 */
                void setLayer(Identifier id, unsigned int layer);

                /**
                 * Apply a scale factor to the specified sprite instance.
                 * @param id Sprite instance identifier.
                 * @param scale Scaling ratio.
                 */
                void scale(Identifier id, float scale);
            private:
                /**
                 * Copy sprite content to a cell.
                 * @param [in] sprite Source frame.
                 * @param [in] cell Destination cell.
                 * @param [in] x Display position of the frame (on x axis).
                 * @param [in] y Display position of the frame (on y axis)
                 * @param [in] angle Sprite angle.
                 * @param [in] scale Sprite scaling factor.
                 * @param [in] layer Sprite layer.
                 */
                inline void assignSpriteToCell(const Sprite *sprite, Cell *cell,
                        double x, double y, float angle, float scale);

                /**
                 * Swap instances.
                 * @param id Look-up table identifier.
                 * @param a First instance.
                 * @param b Second instance.
                 */
                void swapInstances(Identifier id, Identifier a, Identifier b);

            private:
                /**
                 * Attached Sprite Atlas.
                 */
                const Atlas *_atlas;

                /**
                 * Sprite instance capacity.
                 */
                unsigned int _capacity;

                /**
                 * Number of managed sprites.
                 */
                unsigned int _count;

                /**
                 * Sprites instances.
                 */
                Instance *_instance;

                /**
                 * Sprites data.
                 */
                Cell *_cell;

                /**
                 * Lookup table.
                 */
                LookupItem *_table;

                /**
                 * Pointer to first used slot.
                 */
                int _used;

                /**
                 * Pointer to first free slot.
                 */
                int _free;

                /**
                 * Pointer to lastly allocated slot.
                 */
                int _last;
        };

        /**
         * Sprite engine delegation class.
         * By itself, the sprite engine doesn't do much.
         * It only stores an Atlas from which can be extracted 'Artifacts'.
         * Artifacts are the base for sprites and sprite animations.
         * The computation of what must be drawn is done out of the box.
         * A 'Cache' structure is passed to the delegation class for any update.
         */
        class Delegate {
            public:
                /**
                 * Create a delegation object attached to an Atlas.
                 * @param [in] atlas Sprite atlas.
                 */
                Delegate(Atlas *atlas) : _atlas(atlas) {}

                /**
                 * Amical destructor.
                 */
                ~Delegate() {}

                // And now, the methods satisfying the 'Delegate' Concept.

                /**
                 * @return List of shaders used for rendering.
                 */
                std::vector<std::pair<Framework::Render::Shader::Type, const char *> >
                    shaders() const;

                /**
                 * @return Vertex stream composition.
                 */
                std::vector<std::pair<unsigned int, Framework::Render::Geometry::Attribute> >
                    attributes() const;

                /**
                 * @return Drawing mode.
                 */
                inline GLenum primitive() const { return GL_TRIANGLE_STRIP; }

                /**
                 * @return 'true' if the drawing is instanced.
                 */
                inline bool isInstanced() const { return true; }

                /**
                 * @return The instanciation cardinality.
                 */
                inline GLsizei instanceCardinality() const { return 4; }

                /**
                 * Initialisation.
                 * @param [in] program Compiled program that will be used for rendering.
                 */
                void init(Framework::Render::Program &program);

                /**
                 * Program update at rendering time.
                 * @param [in] program Program used for rendering.
                 */
                void update(Framework::Render::Program &program);

                /**
                 * Post-render context cleaning.
                 */
                void postRender();

                /**
                 * Directly print the content of a precomputed buffer.
                 * @param [in] ptr Buffer to update.
                 * @param [in] capacity Buffer capacity (number of elements).
                 * @param [in] cache Vertex buffer data.
                 * @param [in] size Number of artifacts to draw.
                 * @return The number of updated elements.
                 */
                GLsizei update(void *ptr, GLsizei capacity,
                        const void *cache, unsigned int size);

                /**
                 * Update using an Atlas cache.
                 * @param [in] ptr Buffer to update.
                 * @param [in] capacity Buffer capacity (number of elements).
                 * @param [in] cache Artifacts cache.
                 * @return The number of updated elements.
                 */
                GLsizei update(void *ptr, GLsizei capacity,
                        const Cache *cache);

                /**
                 * Set the viewport.
                 * @param [in] startX Viewport starting point on X-axis.
                 * @param [in] startY Viewport starting point on Y-axis.
                 * @param [in] width Width.
                 * @param [in] height Height.
                 */
                void viewport(GLfloat startX, GLfloat startY, GLfloat width, GLfloat height);

                /**
                 * @return The projection matrix.
                 */
                inline glm::mat4 viewport() { return _matrix; }

                /**
                 * Provide access to the Atlas.
                 * @return Sprite atlas.
                 */
                Atlas *getAtlas() const { return _atlas; }

            private:
                // Some protections.

                /**
                 * Private copy operator.
                 */
                Delegate &operator=(const Delegate &) { return *this; }

            private:
                /**
                 * Atlas.
                 */
                Atlas *_atlas;

                /**
                 * Texture uniform binding.
                 */
                GLint _uniformTexture;

                /**
                 * Projection Matrix uniform binding.
                 */
                GLint _uniformMatrix;

                /**
                 * Projection Matrix.
                 */
                glm::mat4 _matrix;
        };

        /**
         * Sprite Engine Declaration/Definition.
         */
        typedef Dumb::Core::Engine<Delegate> Engine;
    } // 'Sprite' namespace.
} // 'Dumb' namespace.

#endif
