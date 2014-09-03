namespace Framework {
namespace Input     {

/** Mouse position listener. **/
class Mouse::PositionListener
{
    public:
        /** Destructor. **/
        virtual ~PositionListener();
        /**
         * Cursor movement callback.
         * @param [in] cursor Current cursor position.
         */
        virtual bool operator() (glm::dvec2 const& cursor) = 0;
};
/** Mouse button listener. **/
class Mouse::ButtonListener
{
    public:
        /** Destructor. **/
        virtual ~ButtonListener();
        /**
         * Button state change callback.
         * @param [in] button   Button id.
         * @param [in] pressed  Button state (true: pressed, false: released).
         * @param [in] modifier Key modifier [todo]
         */
        virtual bool operator() (Mouse::Button button, bool pressed, int modifier) = 0;
};
/** Scroll listener. **/
class Mouse::ScrollListener
{
    public:
        /** Destructor. **/
        virtual ~ScrollListener();
        /**
         * [todo]
         * @param [in] offset [todo]
         */
        virtual bool operator() (glm::dvec2 const& offset) = 0;
};

template <class T>
class MousePositionListenerHelper : public Mouse::PositionListener
{
    typedef bool (T::*TProc)(glm::dvec2 const& cursor);
    public:
        MousePositionListenerHelper(T* instance, TProc method);
        virtual bool operator() (glm::dvec2 const& cursor) ;
    protected:
        T*    _instance;
        TProc _method;
};

template <class T>
class MouseButtonListenerHelper : public Mouse::ButtonListener
{
    typedef bool (T::*TProc)(Mouse::Button button, bool pressed, int modifier);
    public:
        MouseButtonListenerHelper(T* instance, TProc method);
        virtual bool operator() (Mouse::Button button, bool pressed, int modifier) ;
    protected:
        T*    _instance;
        TProc _method;
};

template <class T>
class MouseScrollListenerHelper : public Mouse::ScrollListener
{
    typedef bool (T::*TProc)(glm::dvec2 const& offset);
    public:
        MouseScrollListenerHelper(T* instance, TProc method);
        virtual bool operator() (glm::dvec2 const& offset) ;
    protected:
        T*    _instance;
        TProc _method;
};

template <class T>
MousePositionListenerHelper<T>::MousePositionListenerHelper(T* instance, MousePositionListenerHelper<T>::TProc method)
    : _instance(instance)
    , _method(method)
{}

template <class T>
bool MousePositionListenerHelper<T>::operator() (glm::dvec2 const& cursor) 
{
    if((NULL == _instance) || (NULL == _method))
    {
        return false;
    }
    return (_instance->*_method)(cursor);
}

template <class T>
MouseButtonListenerHelper<T>::MouseButtonListenerHelper(T* instance, MouseButtonListenerHelper<T>::TProc method)
    : _instance(instance)
    , _method(method)
{}

template <class T>
bool MouseButtonListenerHelper<T>::operator() (Mouse::Button button, bool pressed, int modifier) 
{
    if((NULL == _instance) || (NULL == _method))
    {
        return false;
    }
    return (_instance->*_method)(button, pressed, modifier);
}

template <class T>
MouseScrollListenerHelper<T>::MouseScrollListenerHelper(T* instance, MouseScrollListenerHelper<T>::TProc method)
    : _instance(instance)
    , _method(method)
{}

template <class T>
bool MouseScrollListenerHelper<T>::operator() (glm::dvec2 const& offset) 
{
    if((NULL == _instance) || (NULL == _method))
    {
        return false;
    }
    return (_instance->*_method)(offset);
}

template<class T>
bool Mouse::addPositionListener(T* instance, bool (T::*method)(glm::dvec2 const& cursor))
{
    Mouse::PositionListener* listener = new MousePositionListenerHelper<T>(instance, method);
    return addPositionListener(listener);
}

template<class T>
bool Mouse::addButtonListener(T* instance, bool (T::*method)(Mouse::Button button, bool pressed, int modifier))
{
    Mouse::ButtonListener* listener = new MouseButtonListenerHelper<T>(instance, method);
    return addButtonListener(listener);
}

template<class T>
bool Mouse::addScrollListener(T* instance, bool (T::*method)(glm::dvec2 const& offset))
{
    Mouse::ScrollListener* listener = new MouseScrollListenerHelper<T>(instance, method);
    return addScrollListener(listener);
}

} // Input
} // Framework
