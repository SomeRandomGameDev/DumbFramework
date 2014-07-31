#ifndef _DUMB_FW_SEVERITY_
#define _DUMB_FW_SEVERITY_

namespace Framework {
/**
 * @brief Severity
 * Indicates the severity of the information.
 */
struct Severity
{
    /** Severity values. **/
    enum Value
    {
        Info,       /**< Purely informative. **/
        Warning,    /**< Something wrong or unexpected happened but nothing irremediable. **/
        Error       /**< Something failed. **/
    };
    Value value; /**< Severity value. **/
    /** Default constructor. **/
    inline Severity() {}
    /** 
     * Constructor.
     * @param [in] v Severity value.
     */
    inline Severity(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
    /**
     * Convert severity value to string.
     * Severity | String
     * -------- | -------
     * Info     | info
     * Warning  | warning
     * Error    | error
     * 
     * @return Severity as string. 
     */
    char const* toString() const;
};

} // Framework

#endif /* _DUMB_FW_SEVERITY_ */
