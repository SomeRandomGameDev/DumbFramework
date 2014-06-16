#ifndef _DUMB_FW_HYPERPLANE_
#define _DUMB_FW_HYPERPLANE_

namespace Framework {

struct Side
{
    enum Value
    {
        Front = 0,
        Back,
        On
    } value;
    inline Side() : value(Back) {}
    inline Side(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
};

}

#endif /* _DUMB_FW_HYPERPLANE_ */
