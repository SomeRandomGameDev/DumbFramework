#include <DumbFramework/entity.hpp>

namespace Framework {
/** Default constructor. **/
Entity::Entity()
    : name("")
    , _enable(false)
    , _globalEnable(false)
    , _visible(false)
    , _globalVisible(false)
    , _transform()
    , _parent(nullptr)
    , _children()
{}
/** 
 * Copy constructor
 * @param [in] e Source entity.
 */
Entity::Entity(Entity const& e)
    :  name(e.name)
    , _enable(e._enable)
    , _globalEnable(e._globalEnable)
    , _visible(e._visible)
    , _globalVisible(e._globalVisible)
    , _transform(e._transform)
    , _parent(e._parent)
    , _children(e._children)
{}
/** Destructor. **/
Entity::~Entity()
{}
/**
 * Attach new child.
 * @param [in] e Entity to be added.
 */
void Entity::addChild(Entity* e)
{
    e->detach();
    _children.push_back(e);
    e->_parent = this;
}
/**
 * Remove child.
 * @param [in] e Child entity to remove.
 */
void Entity::removeChild(Entity* e)
{
    for(size_t i=0; i<_children.size(); i++)
    {
        if(_children[i] == e)
        {
            e->_parent = nullptr;
            _children.erase(_children.begin()+i);
            return;
        }
    }
}
/**
 * Detach entity from tree.
 * @return
 */
void Entity::detach()
{
    if(nullptr != _parent)
    {
        _parent->removeChild(this);
    }
}
/** Get parent entity. **/
Entity const* Entity::parent() const
{
    return _parent;
}
/**
 * Get child at the specified index.
 * @param [in] index Child index.
 * @return A pointer to the child at the specified index or 
 *         nullptr if the index is out of bound.
 */
Entity* Entity::child(size_t index)
{
    if(index >= _children.size())
    {
        return nullptr;
    }
    return _children[index];
}
/**
 * Get child at the specified index.
 * @param [in] index Child index.
 * @return A pointer to the child at the specified index or 
 *         nullptr if the index is out of bound.
 */
Entity const* Entity::child(size_t index) const
{
    if(index >= _children.size())
    {
        return nullptr;
    }
    return _children[index];
}
/** Return child cound. **/
size_t Entity::childrenCount() const
{
    return _children.size();
}
/**
 * Clone entity without its children.
 * @return Pointer to the cloned entity.
 */
Entity* Entity::clone() const
{
    return new Entity(*this);
}
/**
 * Clone entity and its children recursively.
 * @return Pointer to the cloned entity.
 */
Entity* Entity::deepClone() const
{
    Entity* e = clone();
    if(nullptr == e)
    {
        return nullptr;
    }
    
    if(!_children.empty())
    {
        for(size_t i=0; i<e->_children.size(); i++)
        {
            e->_children[i] = _children[i]->deepClone();
            if(nullptr != e->_children[i])
            {
                e->_children[i]->_parent = e;
            }
        }
    }
    return e;
}
/** Recursively update activity flag. **/
void Entity::_updateActivity()
{
    _globalEnable = _enable;
    if(nullptr != _parent)
    {
        _globalEnable = _globalEnable && _parent->_globalEnable;
    }
    for(size_t i=0; i<_children.size(); i++)
    {
        _children[i]->_updateActivity();
    }
}
/** Recursively update visibility flag. **/
void Entity::_updateVisibility()
{
    _globalVisible = _enable;
    if(nullptr != _parent)
    {
        _globalVisible = _globalVisible && _parent->_globalVisible;
    }
    for(size_t i=0; i<_children.size(); i++)
    {
        _children[i]->_updateVisibility();
    }
}
/**
 * Enable entity.
 * Note that even if an entity is enabled, if one of its parents
 * is disabled, the entity will remain disabled.
 */
void Entity::enable()
{
    _enable = true;
    _updateActivity();
}
/**
 * Disable entity.
 * The entity and all its subtree (even if the child is enabled)
 * will be disabled.
 */
void Entity::disable()
{
    _enable = false;
    _updateActivity();
}
/**
 * Show entity.
 * Note that even if an entity is set to be shown, if one of its
 * parents is hidden, the entity will remain hidden.
 */
void Entity::show()
{
    _visible = true;
    _updateVisibility();
}
/**
 * Hide entity.
 * The entity and all its subtree (even if the child is set to 
 * be displayed) will be hidden.
 */
void Entity::hide()
{
    _visible = false;
    _updateVisibility();
}
/** Is the entity enabled ? **/
bool Entity::isEnabled()
{
    return _enable;
}
/**
 * Is the entity globally enabled?
 * If one parent is disabled, the entity will be globally
 * disabled.
 */
bool Entity::isGloballyEnabled()
{
    return _globalEnable;
}
/** Is the entity visible ? **/
bool Entity::isVisible()
{
    return _visible;
}
/**
 * Is the entity globally visible?
 * If one parent is not visible, the entity will be globally
 * invisible.
 */
bool Entity::isGloballyVisible()
{
    return _globalVisible;
}
/**
 * Main render method.
 * @note Override it to add your own rendering.
 */
void Entity::render()
{}
/**
 * Main update method.
 * This method is called at each render frame. The delay between
 * each call may vary. Use fixedUpdate if you want to have a
 * method called at a fixed frequency (60Hz) use fixedUpdate.
 */
void Entity::update()
{}
/**
 * Call update method at a fixed frequency (60Hz)
 */
void Entity::fixedUpdate()
{}
/** Render current entity and all its children. **/
void Entity::_render()
{
    if(_globalVisible)
    {
        render();
        for(size_t i=0; i<_children.size(); i++)
        {
            _children[i]->_render();
        }
    }
}
/** Update current entity and all its children. **/
void Entity::_update()
{
    if(_globalEnable)
    {
        update();
        for(size_t i=0; i<_children.size(); i++)
        {
            _children[i]->_update();
        }
    }
}
/** Update current entity and all its children. **/
void Entity::_fixedUpdate()
{
    if(_globalEnable)
    {
        fixedUpdate();
        for(size_t i=0; i<_children.size(); i++)
        {
            _children[i]->_fixedUpdate();
        }
    }
}

/** Set local transform. **/
void Entity::transform(Transform2D const& t)
{
    _transform = t;
    // [todo] set dirty flag
}
/** Get local transform. **/
Transform2D const& Entity::transform() const
{
    return _transform;
}
/** Get global transform. **/
Transform2D const& Entity::globalTransform() const
{
    // [todo]
    static Transform2D dummy;
    return dummy;
}

} // Framework
