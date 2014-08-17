#ifndef DECLARE_ENUM

#define BEGIN_ENUM(name) \
struct name { \
    enum Value { \

        
#define DEFINE_ENUM(value) value, \


#define END_ENUM(name) \
        Count \
    } value; \
    inline name() {} \
    inline name(Value v) : value(v) {}  \
    inline operator Value() { return value; }  \
    inline operator const Value() const { return value; }  \
    const char* toString() const; \
}; \

#else

#ifdef BEGIN_ENUM
#undef BEGIN_ENUM
#endif /* BEGIN_ENUM */

#ifdef DEFINE_ENUM
#undef DEFINE_ENUM
#endif /* DEFINE_ENUM */

#ifdef END_ENUM
#undef END_ENUM
#endif /* END_ENUM */

#define BEGIN_ENUM(name) \
static const char* name##_string[name::Count] = { \
    

#define DEFINE_ENUM(value) #value , \

        
#define END_ENUM(name) \
}; \
const char* name::toString() const { \
    if(value < name::Count) { return name##_string[value]; } \
    else { return ""; } \
} \
    

#endif /* DECLARE_ENUM */
