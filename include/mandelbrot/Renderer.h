/**
 * Renders the mandelbrot fractal on the GPU.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <string>

#include <mandelbrot/ColorArray.h>
#include <mandelbrot/ComputationStage.h>
#include <mandelbrot/SmoothColoringStage.h>
#include <mandelbrot/DisplayStage.h>
#include <mandelbrot/Box2.h>
#include <mandelbrot/Vector2.h>


namespace mandelbrot
{
    /**
     * This class is responsible for rendering the fractal.
     */
    class Renderer
    {
        public:
        enum class Resolution : unsigned int
        {
            Size_2    = 1,  // Power-of-two
            Size_4    = 2,
            Size_8    = 3,
            Size_16   = 4,
            Size_32   = 5,
            Size_64   = 6,
            Size_128  = 7,
            Size_256  = 8,
            Size_512  = 9,
            Size_1024 = 10,
            Size_2048 = 11,
            Size_4096 = 12
        };

        /**
         * Initializes resources. 
         * Should be called once before attempting to render.
         */
        bool Initialize();

        /**
         * Resets any rendering that is currently in progress.
         */
        void Reset();
        /**
         * Performs one rendering step - i.e. evaluates the mandelbrot
         * function for a fixed amount of iterations.
         */
        void RenderStep();
        /**
         * Finalizes the current rendering operation, colors the image and
         * saves it to the cache.
         */
        void Flush();
        /**
         * Renders image from cache.
         */
        void Render();

        /**
         * Gets rendering's image data.
         */
        void GetImagePixels(unsigned char* buffer);

        /**
         * Checks whether the renderer is properly initialized.
         */
        bool is_ready() const;
        /**
         * Gets string containing status information.
         */
        const char* status_message() const;

        /**
         * Checks whether rendering is complete.
         */
        bool is_done() const;

        /**
         * Gets image size.
         */
        const Vector2u& display_size() const;
        /**
         * Sets image size.
         */
        void set_display_size(const Vector2u& value);

        /**
         * Gets resolution.
         */
        unsigned int resolution() const;
        /**
         * Sets resolution.
         *
         * @note 
         *      Given value will be ceil'd towards the nearest 
         *      power-of-two that contains it.
         */
        void set_resolution(unsigned int value);
        /**
         * Sets resolution.
         */
        void set_resolution(Resolution value);

        /**
         * Gets color map.
         */
        const ColorArray& color_map() const;
        /**
         * Sets color map.
         */
        void set_color_map(const ColorArray& value);

        /**
         * Gets the number of evaluated iterations of the mandelbrot sequence
         * per rendering step.
         */
        unsigned int iterations_per_step() const;
        /**
         * Sets the number of evaluated iterations of the mandelbrot sequence
         * per rendering step.
         */
        void set_iterations_per_step(unsigned int value);

        /**
         * Gets the maximum number of rendering steps.
         */
        unsigned int max_step_count() const;
        /**
         * Sets the maximum number of rendering steps.
         */
        void set_max_step_count(unsigned int value);

        /**
         * Gets viewport.
         */
        const Box2d& viewport() const;
        /**
         * Sets viewport center position.
         */
        void set_viewport_position(const Vector2d& value);
        /**
         * Sets viewport size.
         */
        void set_viewport_size(double value);
        /**
         * Gets display viewport.
         */
        const Box2d& display_viewport() const;

        private:
        unsigned int step_count_ = 0;
        unsigned int max_step_count_ = 1;

        std::string status_message_;

        ComputationStage computation_stage_;
        SmoothColoringStage coloring_stage_;
        DisplayStage display_stage_;
    };
}
