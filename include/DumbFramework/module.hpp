#ifndef _DUMB_FW_MODULE_
#define _DUMB_FW_MODULE_

namespace Framework {

/**
 * @brief Application module.
 * Identifier of the current application module.
 */
struct Module
{
    /** Module ids. **/
    enum Value
    {
        Base,       /**< Base module (low level or system). **/
        Render,     /**< Rendering module. **/
        App,        /**< User application. **/
    };
    Value value; /**< Module identifier. **/
    /** Default constructor. **/
    inline Module() {}
    /** 
     * Constructor.
     * @param [in] v Module id.
     */
    inline Module(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
    /**
     * Convert module id to string.
     * Module   | String
     * -------- | -------
     * Base     | Base
     * Render   | Render
     * App      | App
     * 
     * @return Module id as string. 
     */
    char const* toString() const;
};

} // Framework

#endif /* _DUMB_FW_MODULE_ */
