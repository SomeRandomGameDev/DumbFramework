#ifndef _DUMB_FW_ENTITY_
#define _DUMB_FW_ENTITY_

#include <string>
#include <vector>
#include <cstddef>

#include <DumbFramework/transform2d.hpp>

namespace Framework {

class Entity
{
    public:
        /** Default constructor. **/
        Entity();
        /** 
         * Copy constructor
         * @param [in] e Source entity.
         */
        Entity(Entity const& e);
        /** Destructor. **/
        ~Entity();
       
        /**
         * Attach new child.
         * @param [in] e Entity to be added.
         */
        void addChild(Entity* e);
        /**
         * Remove child.
         * @param [in] e Child entity to remove.
         */
        void removeChild(Entity* e);
        /**
         * Detach entity from tree.
         * @return
         */
        void detach();
        /** Get parent entity. **/
        Entity const* parent() const;
        /**
         * Get child at the specified index.
         * @param [in] index Child index.
         * @return A pointer to the child at the specified index or 
         *         nullptr if the index is out of bound.
         */
        Entity* child(size_t index);
        /**
         * Get child at the specified index.
         * @param [in] index Child index.
         * @return A pointer to the child at the specified index or 
         *         nullptr if the index is out of bound.
         */
        Entity const* child(size_t index) const;
        /** Return child cound. **/
        size_t childrenCount() const;
        /**
         * Clone entity without its children.
         * @return Pointer to the cloned entity.
         */
        virtual Entity* clone() const;
        /**
         * Clone entity and its children recursively.
         * @return Pointer to the cloned entity.
         */
        Entity* deepClone() const;
        
        /**
         * Enable entity.
         * Note that even if an entity is enabled, if one of its parents
         * is disabled, the entity will remain disabled.
         */
        void enable();
        /**
         * Disable entity.
         * The entity and all its subtree (even if the child is enabled)
         * will be disabled.
         */
        void disable();
        /**
         * Show entity.
         * Note that even if an entity is set to be shown, if one of its
         * parents is hidden, the entity will remain hidden.
         */
        void show();
        /**
         * Hide entity.
         * The entity and all its subtree (even if the child is set to 
         * be displayed) will be hidden.
         */
        void hide();        
        /** Is the entity enabled ? **/
        bool isEnabled();
        /**
         * Is the entity globally enabled?
         * If one parent is disabled, the entity will be globally
         * disabled.
         */
        bool isGloballyEnabled();
        /** Is the entity visible ? **/
        bool isVisible();
        /**
         * Is the entity globally visible?
         * If one parent is not visible, the entity will be globally
         * invisible.
         */
        bool isGloballyVisible();
        
        /**
         * Main render method.
         * @note Override it to add your own rendering.
         */
        virtual void render();
        /**
         * Main update method.
         * This method is called at each render frame. The delay between
         * each call may vary. Use fixedUpdate if you want to have a
         * method called at a fixed frequency (60Hz) use fixedUpdate.
         */
        virtual void update();
        /**
         * Call update method at a fixed frequency (60Hz)
         */
        virtual void fixedUpdate();
        
        /** Set local transform. **/
        void transform(Transform2D const& t);
        /** Get local transform. **/
        Transform2D const& transform() const;
        /** Get global transform. **/
        Transform2D const& globalTransform() const;
        
    protected:
        /** Render current entity and all its children. **/
        void _render();
        /** Update current entity and all its children. **/
        void _update();
        /** Update current entity and all its children. **/
        void _fixedUpdate();
        /** Recursively update activity flag. **/
        void _updateActivity();
        /** Recursively update visibility flag. **/
        void _updateVisibility();
        
    public:
        /** Name. **/
        std::string name;
        
    protected:
        /** 
         * If true the entity is considered active. Note that if its 
         * parent is disable, this entity will be considered globally
         * disabled.
         */
        bool _enable;
        /**
         * Global activity flag.
         * This can be viewed as a logical "and" of the current entity 
         * and all its parents flag.
         */
        bool _globalEnable;
        /**
         * If true the entity will be displayed unless one of its 
         * parents is not visible.
         */
        bool _visible;
        /**
         * Global visibility flag.
         * This can be viewed as a logical "and" of the current entity 
         * and all its parents flag.
         */
        bool _globalVisible;
        /** Local transform. **/
        Transform2D _transform;
        // [todo] global transform + dirty flag?
        /** Parent entity. **/
        Entity *_parent;
        /** Child entities. **/
        std::vector<Entity*> _children;
        // callbacks...
        // bounding object (collider?)
        // animation
        // materials, etc...
};

} // Framework

#endif /* _DUMB_FW_ENTITY_ */
