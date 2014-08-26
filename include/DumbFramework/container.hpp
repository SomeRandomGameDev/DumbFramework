#ifndef _DUMB_FW_CONTAINER_
#define _DUMB_FW_CONTAINER_

namespace Framework {

    /**
     * Oversimple overridable container.
     */
    template <typename T> class Container {
        public:
            /**
             * Base constructor.
             */
            Container(unsigned int capacity);
            
            /**
             * Destructor.
             */
            virtual ~Container();

            /**
             * @return Number of storable element.
             */
            unsigned int capacity() const;

            /**
             * @param index Element position.
             * @return Targeted element or 0 if out of range.
             */
            const T *data(unsigned int index) const;
            
            /**
             * @param index Element position.
             * @return Targeted element or 0 if out of range.
             */
            T *data(unsigned int index);

            /**
             * Unsecure accessor. (const version)
             * @param index Element position.
             * @return Copy of the targeted element.
             */
            const T& get(unsigned int index) const;
            /**
             * Unsecure accessor.
             * @param index Element position.
             * @return Copy of the targeted element.
             */
            T get(unsigned int index);
            
        private:
            unsigned int _capacity; /**< Capacity. **/
            T * _data;             /**< Data. **/
    };

    /**
     * Base constructor.
     */
    template <typename T>
    Container<T>::Container(unsigned int capacity) {
        _capacity = capacity;
        _data = new T[_capacity];
    }

    /**
     * Destructor.
     */
    template <typename T>
    Container<T>::~Container() {
        delete [] _data;
    }

    /**
     * @return Number of storable element.
     */
    template <typename T>
    unsigned int Container<T>::capacity() const {
        return _capacity;
    }

    /**
     * @param index Element position.
     * @return Targeted element or 0 if out of range.
     */
    template <typename T>
    T *Container<T>::data(unsigned int index) {
        T* result = 0;
        if(index < _capacity) {
            result = _data + index;
        }
        return result;
    }

    /**
     * Unsecure accessor.
     * @param index Element position.
     * @return Copy of the targeted element.
     */
    template <typename T>
    T Container<T>::get(unsigned int index) {
        return _data[index];
    }
            
    /**
     * Unsecure accessor. (const version)
     * @param index Element position.
     * @return Copy of the targeted element.
     */
    template <typename T>
    const T& Container<T>::get(unsigned int index) const {
        return _data[index];
    }
}

#endif /* _DUMB_FW_CONTAINER_ */
