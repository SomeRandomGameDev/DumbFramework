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
#ifndef _DUMB_FW_SPRITE_
#define _DUMB_FW_SPRITE_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <unordered_map>

#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/texture2d.hpp>

namespace Dumb {
    namespace Sprite {

        /**
         * An artifact is a non-mutable definition of a graphic object in
         * the sprite atlas.
         */
        class Artifact {
            public:
                /**
                 * Default constructor.
                 * @param [in] anchor Anchor Position.
                 * @param [in] coordinates Texture coordinates.
                 * @param [in] layer Texture index.
                 */
                Artifact(glm::vec2 anchor, glm::vec4 coordinates, unsigned int layer) :
                    _anchor(anchor), _coordinates(coordinates), _layer(layer) { }

                /**
                 * @return The anchor.
                 */
                const glm::vec2 &anchor() { return _anchor; }

                /**
                 * @return The texture coordinates.
                 */
                const glm::vec4 &coordinates() { return _coordinates; }

                /**
                 * @return Texture layer.
                 */
                unsigned int layer() { return _layer; }
            private:
                /**
                 * Anchor position.
                 */
                glm::vec2 _anchor;

                /**
                 * Texture coordinates.
                 * These coordinates also serves as size. There expressed in texels
                 * instead of the classic [0..1] range.
                 */
                glm::vec4 _coordinates;

                /**
                 * Texture index in the texture array.
                 */
                unsigned int _layer;
        };

        /**
         * A sprite state consists in a relative position, a rotation angle
         * and a scale factor associated to a sprite identifier.
         */
        class State {
            public:
                State(Identifier id, glm::vec2 offset, double angle = 0.0, double factor = 1.0) :
                    _identifier(id), _offset(offset), _angle(angle), _factor(factor) {}
                inline Identifier getId() const { return _id; }
                inline void setId(Identifier id) { _id = id; }
                inline glm::vec2 getOffset() const { return _offset; }
                inline void setOffset(glm::vec2 offset) { _offset = offset; }
                inline double getRotation() const { return _angle; }
                inline void setRotation(double angle) { _angle = angle }
                inline double getScale() const { return _factor; }
                inline void setScale(double factor) { _factor = factor };
                inline int getLayer() const { return _layer; }
                inline void setLayer(int layer) { _layer = layer; }
            private:
                /**
                 * Sprite identifier.
                 */
                Identifier _id;

                /**
                 * Relative position.
                 */
                glm::vec2 _offset;

                /**
                 * Rotation angle.
                 */
                double _angle;

                /**
                 * Scale factor.
                 */
                double _factor;

                /**
                 * Layer.
                 */
                int _layer;
        };

        class Keyframe {
            public:
                /**
                 * Constructor.
                 * @param [in] size Number of components.
                 */
                Keyframe(unsigned int size) : _states(size) {}

                /**
                 * @return Relative timestamp (in seconds).
                 */
                inline double getTime() const { return _time; }
                /**
                 * Set relative timestamp.
                 * @param [in] time Time in seconds.
                 */
                inline void setTime(double time) { _time = time; }
                /**
                 * @return The component states.
                 */
                inline const std::vector<const State *> &getStates() const { return _states; }
                /**
                 * Set a component states.
                 * @param [in] id Component identifier (slot index).
                 * @param [in] st Pointer to the state object.
                 */
                inline void setState(int id, const State *st) { _states[id] = st; }
            private:
                /**
                 * Relative time at which the keyframe occurs.
                 */
                double _time;

                /**
                 * States. Can be null for a certain slot, meaning
                 * that there's no modification.
                 */
                std::vector<const State *> _states; // TODO There's a problem here : Who's in charge of this object lifecycle ?
        };

        /**
         * Composite animation definition.
         */
        class Animation {
            // TODO Make keyframe, Fixed size component list.
            public:
            private:
                /**
                 * Keyframes (sorted in time).
                 */
                std::vector<const Keyframe> _frames;
        };

        /**
         * Animation Instanciation.
         */
        class Instance {
            public:
            private:
                /**
                 * Current animation.
                 */
                Animation *_animation;

                /**
                 * Relative time progression.
                 */
                double _time;

                /**
                 * Current/Last keyframe.
                 */
                unsigned int _lastFrame;

                // TODO Add a listener/callback for animation end.
        };

        // TODO Make a sprite management layer ON TOP of the Dumb Sprite Engine.
        // It must manage still images and animation instances. It also must be
        // totally invisible from the Dumb Sprite Engine point of view.

        // ### OLD STYLE ###

        /**
         * An animation frame consists in a texture quad coordinate, an offset,
         * a size and a timestamp.
         * It is a non-mutable object.
         */
        struct Frame
        {
            double       time;    /**< Time of appearance in seconds. **/
            glm::ivec2   offset;  /**< Position offset. **/
            glm::ivec2   size;    /**< Size. **/
            glm::dvec2   top;     /**< Texture coordinate top (upper left). **/
            glm::dvec2   bottom;  /**< Texture coordinate bottom (lower right). **/
            unsigned int layer;   /**< Texture layer in the texture array. **/
        };

        /**
         * An animation consists in an ordered list of frame.
         */
        class Animation
        {
            friend class XMLAnimationReader;

            public:
            /** Default constructor. **/
            Animation();
            /** Destructor. **/
            ~Animation();
            /**
             * Create animation.
             * @param [in] name  Animation name.
             */
            void create(std::string const& name);
            /**
             * Get animation name.
             * @return Animation name.
             */
            std::string const& name() const;
            /**
             * Add frame to animation.
             * Frames are sorted by increasing time.
             * @param [in] frame  Frame to be added.
             */
            void add(Frame const& frame);
            /**
             * Get frame count.
             * @return Number of frames in animation.
             */
            size_t frameCount() const;
            /**
             * Retrieve a given frame.
             * @param [in] index  Frame number.
             * @return A const pointer to the specified frame or NULL if the
             *         index is out of bound.
             */
            Frame const* getFrame(size_t offset) const;

            private:
            std::string        _name;   /**< Animation name. **/
            std::vector<Frame> _frames; /**< Frames sorted by increasing time. **/
        };

        /**
         * Sprite definition.
         * Consist in a list of animation. All materials concerning a sprite
         * must be hosted in the same texture.
         */
        class Definition
        {
            friend class XMLDefinitionReader;
            public:
            /** Default constructor. **/
            Definition();
            /** Destructor. **/
            ~Definition();
            /**
             * Create sprite definition.
             * @param [in] name  Identifier.
             * @param [in] count Animation count. 
             */
            void create(std::string const& name, size_t count);
            /**
             * Get name.
             * @return name.
             */
            std::string const& name() const;
            /**
             * Get animation count.
             * @return Number of stored animations.
             */
            size_t animationCount() const;

            /**
             * Retrieve a given animation by its offset.
             * @param [in] index  Animation number.
             * @return A const pointer to the specified animation or NULL if the
             *         index is out of bound.
             */
            Animation const* getAnimation(size_t offset) const;
            /**
             * Retrieve a given animation by its offset.
             * @param [in] index  Animation number.
             * @return A pointer to the specified animation or NULL if the
             *         index is out of bound.
             */
            Animation* getAnimation(size_t offset);
            /**
             * Retrieve a given animation by its name.
             * @param [in] name  Animation name.
             * @return A const pointer to the animation with the corresponding
             *         name, or NULL if no matching animation was found.
             */
            Animation const* getAnimation(std::string const& name) const;
            /**
             * Retrieve a given animation by its name.
             * @param [in] name  Animation name.
             * @return A pointer to the animation with the corresponding
             *         name, or NULL if no matching animation was found.
             */
            Animation* getAnimation(std::string const& name);

            private:
            /** Sprite definition name. **/
            std::string _name;
            /** Animation list. **/
            std::vector<Animation> _animations;
            /** 
             * Animation dictionary. 
             * For a given name we have its index in the animation storage.
             */
            std::unordered_map<std::string, unsigned int > _dict;
        };

        /**
         * Sprite Atlas. Maps all definitions to a texture.
         * It is not mutable.
         */
        class Atlas
        {
            public:
                /** Constructor. **/
                Atlas();
                /** Destructor. **/
                ~Atlas();

                /**
                 * Read atlas from a XML file.
                 * @param [in] filename  XML filename.
                 * @return true if the atlas was succesfully read.
                 */
                bool read(std::string const& filename);

                /**
                 * Access texture.
                 * @return texture object.
                 */
                Framework::Render::Texture2D const& texture() const;

                /**
                 * @brief Access to definitions.
                 * Return a pointer to the definition stored at @a index, or
                 * NULL if the index is out of storage bound.
                 * @param index Definition number.
                 * @return Definition pointer or NULL if the index is out of
                 *         storage bound..
                 */
                Definition *get(unsigned int index);
                /**
                 * @brief Access to definitions (const version).
                 * Return a pointer to the definition stored at @a index, or
                 * NULL if the index is out of storage bound.
                 * @param index Definition number.
                 * @return Definition pointer or NULL if the index is out of
                 *         storage bound..
                 */
                Definition const* get(unsigned int index) const;
                /**
                 * Retrieve a given definition by its name.
                 * @param [in] name  Definition name.
                 * @return A pointer to the definition with the corresponding
                 *         name, or NULL if no matching definition was found.
                 */
                Definition* get(std::string const& name);
                /**
                 * Retrieve a given definition by its name.
                 * @param [in] name  Definition name.
                 * @return A const pointer to the definition with the corresponding
                 *         name, or NULL if no matching definition was found.
                 */
                Definition const* get(std::string const& name) const;

                /**
                 * Provide the number of definitions.
                 * @return Number of definitions.
                 */
                unsigned int count() const;

            private:

                /**
                 * Load images from files and create a multiple layered texture out of them.
                 * @param [in] filename  Image filenames list. Filenames are separated by a semi-colon.
                 * @return true if the texture was succesfully created.
                 */
                bool loadTextures(const char *filename);

            private:
                /** Sprite definitions. **/
                std::vector<Definition> _definitions;
                /** 
                 * Sprite definition dictionary. 
                 * For a given name we have its index in the sprite definition storage.
                 */
                std::unordered_map<std::string, unsigned int > _dict;
                /** Texture. **/
                Framework::Render::Texture2D _texture;
                /** Atlas size in pixels. **/
                glm::ivec2 _size;
        };

        // []
        // -> (position,
        //     definition pointer,
        //     animation identifier,
        //     current frame,
        //     current progression time) 

        // Each progression step seek to update sprite instance with correct frame.
        //

    } // 'Sprite' namespace.
} // 'Dumb' namespace.

#endif /* _DUMB_FW_SPRITE_ */
