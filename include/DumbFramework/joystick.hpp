#ifndef _DUMB_FW_JOYSTICK_
#define _DUMB_FW_JOYSTICK_

#include <DumbFramework/config.hpp>

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

namespace Framework {

/**
 * Simple joystick class.
 */
class Joystick
{
    public:
        /** 
         * Constructor. 
         * @param [in] id Joystick identifier. 
         */
        Joystick(int id);
        /** 
         * Destructor.
         */
        ~Joystick();
        /**
         * Is the joystick present?
         * @return true if the joystick is present.
         */
        bool isPresent() const;
        /**
         * Update joystick states.
         */
        void update();
        /**
         * Get joystick name.
         * @return Joystick name in UTF-8.
         */
        std::string const& name() const;
         /**
          * Get axes count.
          * @return axes count.
          */
         size_t getAxesCount() const;
         /**
          * Get axes.
          * @return const pointer to axes.
          */
         float const* getAxes() const;
         /**
          * Get key count.
          * @return key count.
          */
         size_t getKeyCount() const;
         /**
          * Get keys.
          * @return const pointer ot keys.
          */
         uint8_t const* getKeys() const;

    private:
        /** Joystick number. **/
        int _id;
        /** Joystick name. **/
        std::string _name;
        /** Axes values. **/
        std::vector<float> _axes;
        /** Keys. **/
        std::vector<uint8_t> _keys;
};
    
} // Framework

#endif /* _DUMB_FW_JOYSTICK_ */
