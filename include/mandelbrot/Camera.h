/**
 * 2D Camera.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <mandelbrot/Vector2.h>


namespace mandelbrot
{
    /**
     * This class represents a two-dimensional camera capable of movement
     * and zooming.
     */
    class Camera
    {
        public:
        /**
         * Creates a new camera.
         */
        Camera();
        /**
         * Creates a new camera with given movement- and zoom-speed.
         */
        Camera(double movement_speed, double zoom_speed);

        /**
         * Moves camera along given direction for given amount of time.
         */
        void Move(const Vector2d& direction, const double duration);
        /**
         * Zooms camera either in or out for given amount of time.
         */
        void Zoom(int direction, const double duration);

        /**
         * Gets position.
         */
        const Vector2d& position() const;
        /**
         * Sets position.
         */
        void set_position(const Vector2d& value);

        /**
         * Gets zoom factor.
         */
        double zoom_factor() const;
        /**
         * Sets zoom factor.
         */
        void set_zoom_factor(double value);

        /**
         * Gets movement speed (per second).
         */
        double movement_speed() const;
        /**
         * Sets movement speed (per second).
         */
        void set_movement_speed(double value);

        /**
         * Gets zoom speed (per second).
         */
        double zoom_speed() const;
        /**
         * Sets zoom speed (per second).
         */
        void set_zoom_speed(double value);

        private:
        double movement_speed_;
        double zoom_speed_;

        Vector2d position_ = Vector2d(0, 0);
        double zoom_factor_ = 1.0;
    };
}
