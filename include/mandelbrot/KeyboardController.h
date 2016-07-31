/**
 * Application keyboard controller.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <mandelbrot/Vector2.h>


namespace mandelbrot
{
    class Application;

    /**
     * This class contains keyboard controller logic.
     */
    class KeyboardController
    {
        public:
        static const int KEY_EXIT;
        static const int KEY_LEFT, 
                         KEY_RIGHT;
        static const int KEY_UP, 
                         KEY_DOWN;
        static const int KEY_ZOOM_IN, 
                         KEY_ZOOM_OUT;
        static const int KEY_INCREASE_PRECISION,
                         KEY_DECREASE_PRECISION;
        static const int KEY_TOGGLE_PAUSE, 
                         KEY_STEP;
        static const int KEY_SAVE_SNAPSHOT;

        /**
         * Represents a one dimensional axis, controlled by two keys, 
         * that emits either a negative, positive, or neutral signal.
         */
        class ControlAxis
        {
            public:
            /**
             * Declare the key responsible for the 'up' signal is pressed.
             */
            void PressUp();
            /**
             * Declare the key responsible for the 'up' signal is released.
             */
            void ReleaseUp();

            /**
             * Declare the key responsible for the 'down' signal is pressed.
             */
            void PressDown();
            /**
             * Declare the key responsible for the 'down' signal is released.
             */
            void ReleaseDown();

            /**
             * Get signal.
             *
             * @returns One of {-1, 0, 1}
             */
            int signal() const;

            private:
            enum class Signal : int
            {
                None = 0,
                Up = 1,
                Down = -1
            };

            bool is_signaling_up_ = false;
            bool is_signaling_down_ = false;
            Signal most_recent_signal_ = Signal::None;
        };

        /**
         * Creates an orphaned controller.
         */
        KeyboardController();
        /**
         * Creates a controller under given application.
         */
        KeyboardController(Application& application);

        /**
         * Processes key event.
         */
        void Process(int key, int action, int modifiers);

        /**
         * Gets horizontal axis.
         */
        ControlAxis& horizontal_axis();
        /**
         * Gets vertical axis.
         */
        ControlAxis& vertical_axis();
        /**
         * Gets zoom axis.
         */
        ControlAxis& zoom_axis();

        private:
        static const int KEY_DEBUG;

        static void ProcessAxis
        (
            ControlAxis& axis, 
            int key_up, int key_down,
            int key, int action
        );

        Application* application_;

        ControlAxis horizontal_axis_;
        ControlAxis vertical_axis_;
        ControlAxis zoom_axis_;
    };
}
