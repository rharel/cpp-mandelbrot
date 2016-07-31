/**
 * 2D vector.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <algorithm>
#include <type_traits>


namespace mandelbrot
{
    /**
     * Rudimentary two-dimensional vector.
     */
    template <typename T>
    class Vector2
    {
        static_assert(std::is_arithmetic<T>::value, "is arithmetic");

        public:
        Vector2() : Vector2(0, 0) {}
        Vector2(T x, T y) : x(x), y(y) {};
        
        T x, y;

        template <typename T2>
        Vector2<T2> Cast() const
        {
            static_assert(std::is_arithmetic<T2>::value, "is arithmetic");

            return Vector2<T2>
            (
                static_cast<T2>(x), 
                static_cast<T2>(y)
            );
        }

        // unary //

        Vector2<T> operator-() const
        {
            return Vector2<T>(-x, -y);
        }

        // operator + //

        Vector2<T>& operator+=(const Vector2<T>& other)
        {
            this->x += other.x;
            this->y += other.y;
            return *this;
        }
        Vector2<T>& operator+=(double scalar)
        {
            this->x += scalar;
            this->y += scalar;
            return *this;
        }
        friend Vector2<T> operator+
        (
            Vector2<T> lhs, 
            const Vector2<T>& rhs
        )
        {
            return lhs += rhs;
        }
        friend Vector2<T> operator+
        (
            Vector2<T> vector, 
            double scalar
        )
        {
            return vector += scalar;
        }
        friend Vector2<T> operator+
        (
            double scalar, 
            const Vector2<T>& vector
        )
        {
            return vector + scalar;
        }

        // operator - //

        Vector2<T>& operator-=(const Vector2<T>& other)
        {
            return *this += -other;
        }
        Vector2<T>& operator-=(double scalar)
        {
            return *this += -scalar;
        }
        friend Vector2<T> operator-
        (
            Vector2<T> lhs, 
            const Vector2<T>& rhs
        )
        {
            return lhs -= rhs;
        }
        friend Vector2<T> operator-
        (
            Vector2<T> vector, 
            double scalar
        )
        {
            return vector -= scalar;
        }
        friend Vector2<T> operator-
        (
            double scalar, 
            const Vector2<T>& vector
        )
        {
            return vector - scalar;
        }
        
        // operator * //

        Vector2<T>& operator*=(const Vector2<T>& other)
        {
            this->x *= other.x;
            this->y *= other.y;
            return *this;
        }
        Vector2<T>& operator*=(double scalar)
        {
            this->x *= scalar;
            this->y *= scalar;
            return *this;
        }
        friend Vector2<T> operator*
        (
            Vector2<T> lhs, 
            const Vector2<T>& rhs
        )
        {
            return lhs *= rhs;
        }
        friend Vector2<T> operator*
        (
            Vector2<T> vector, 
            double scalar
        )
        {
            return vector *= scalar;
        }
        friend Vector2<T> operator*
        (
            double scalar, 
            const Vector2<T>& vector
        )
        {
            return vector * scalar;
        }

        // operator / //

        Vector2<T>& operator/=(const Vector2<T>& other)
        {
            return *this *= Inverse(other);
        }
        Vector2<T>& operator/=(double scalar)
        {
            return *this *= (1.0 / scalar);
        }
        friend Vector2<T> operator/
        (
            Vector2<T> lhs, 
            const Vector2<T>& rhs
        )
        {
            return lhs /= rhs;
        }
        friend Vector2<T> operator/
        (
            Vector2<T> vector, 
            double scalar
        )
        {
            return vector /= scalar;
        }
    };
    typedef Vector2<bool> Vector2b;
    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2u;
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;

    template <typename T>
    double Dot
    (
        const Vector2<T>& lhs, 
        const Vector2<T>& rhs
    )
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    T Min(const Vector2<T>& vector)
    {
        return std::min(vector.x, vector.y);
    }
    template <typename T>
    T Max(const Vector2<T>& vector)
    {
        return std::max(vector.x, vector.y);
    }
    
    template <typename T>
    double LengthSquared(const Vector2<T>& vector)
    {
        return Dot(vector, vector);
    }
    template <typename T>
    double Length(const Vector2<T>& vector)
    {
        return std::sqrt(LengthSquared(vector));
    }

    template <typename T>
    Vector2<T> Normal(const Vector2<T>& vector)
    {
        return vector / Length(vector);
    }

    template <typename T>
    Vector2<T> Inverse(const Vector2<T>& vector)
    {
        return Vector2<T>
        (
            1.0 / vector.x, 
            1.0 / vector.y
        );
    }
}
