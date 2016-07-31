/**
 * Color array.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <vector>


namespace mandelbrot
{
    /**
     * This class represents a one dimensional array of colors.
     */
    class ColorArray
    {
        public:
        /**
         * Creates an array from file.
         *
         * @note 
         *      Uses the SOIL library (http://www.lonesock.net/soil.html).
         *      Expects RGB data.
         */
        static ColorArray FromPath(const char* path);

        /**
         * Creates an empty array.
         */
        ColorArray();
        /**
         * Creates a new array.
         *
         * @param size          Number of color elements.
         * @param channel_count Number of components per element.
         * @param data          Pointer to data.
         */
        ColorArray
        (
            unsigned int size, 
            unsigned int channel_count,
            const unsigned char* data
        );

        /**
         * Checks whether this array contains no usable data.
         */
        bool is_empty() const;
        /**
         * Gets number of color elements.
         */
        unsigned int size() const;
        /**
         * Gets number of components per element.
         */
        unsigned int channel_count() const;
        /**
         * Gets pointer to data.
         */
        const unsigned char* data() const;

        private:
        unsigned int size_;
        unsigned int channel_count_;
        std::vector<unsigned char> data_;
    };
}
