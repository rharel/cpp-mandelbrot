/**
 * 2D square box.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <type_traits>

#include <mandelbrot/Vector2.h>


namespace mandelbrot
{
    /**
     * Two-dimensional square box.
     */
    template <typename T>
    class Box2
    {
        static_assert(std::is_arithmetic<T>::value, "is arithmetic");

        public:
        Box2() 
            : Box2(Vector2<T>(0, 0), 1) {}
        Box2(T x, T y, T size)
            : Box2(Vector2<T>(x, y), size) {}
        Box2
        (
            const Vector2<T>& position,
            const T size
        )
            : position(position), size(size) {}

        Vector2<T> position;
        T size;
    };
    typedef Box2<int> Box2i;
    typedef Box2<unsigned int> Box2u;
    typedef Box2<float> Box2f;
    typedef Box2<double> Box2d;
}
