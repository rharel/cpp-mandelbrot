/**
 * Evaluates the mandelbrot fractal on the GPU.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <memory>

#include <mandelbrot/ProcessingStage.h>
#include <mandelbrot/Vector2.h>
#include <mandelbrot/Box2.h>

#include <oogl/Program.hpp>
#include <oogl/Texture.hpp>
#include <oogl/FrameBuffer.hpp>
#include <oogl/RenderBuffer.hpp>


namespace mandelbrot
{
    /**
     * Evaluates the mandelbrot fractal and saves evaluated values and
     * escape times to textures.
     */
    class ComputationStage : public ProcessingStage
    {
        public:
        static const char* VERTEX_SHADER_SOURCE_PATH;
        static const char* FRAGMENT_SHADER_SOURCE_PATH;

        static const GLint VALUE_TEXTURE_UNIT_INDEX;
        static const GLint LIFETIME_TEXTURE_UNIT_INDEX;

        /**
         * Creates a new stage.
         */
        ComputationStage();

        /**
         * Initializes stage.
         */
        bool Initialize() override;

        /**
         * Resets current rendering.
         */
        void Reset();
        /**
         * Executes one rendering step.
         */
        void Execute() override;

        /**
         * Gets resolution.
         */
        unsigned int resolution() const;
        /**
         * Sets resolution power-of-two.
         */
        void set_resolution_power(unsigned int value);

        /**
         * Gets viewport.
         */
        const Box2d& viewport() const;
        /**
         * Sets viewport position.
         */
        void set_viewport_position(const Vector2d& value);
        /**
         * Sets viewport size.
         */
        void set_viewport_size(const double value);

        /**
         * Gets the number of iterations evaluated per execution step.
         */
        unsigned int iterations_per_step() const;
        /**
         * Sets the number of iterations evaluated per execution step.
         */
        void set_iterations_per_step(unsigned int value);

        /**
         * Gets value texture.
         */
        oogl::Texture& value_texture();
        /**
         * Gets lifetime texture.
         */
        oogl::Texture& lifetime_texture();

        private:
        bool InitializeTextures();
        bool InitializeTexture
        (
            GLint internal_format, 
            GLenum data_format,
            GLenum data_type,
            std::unique_ptr<oogl::Texture>& texture
        );
        bool InitializeBuffers();
        bool InitializeUniforms();

        void UpdateUniforms();
        void UpdateResolution();
        void SwapBuffers();
        void ComputeStep();

        unsigned int resolution_ = 512;
        Box2d viewport_ = Box2d(-2, -1.5, 3);
        unsigned int iterations_per_step_ = 1;

        std::unique_ptr<oogl::Texture> in_value_texture_;
        std::unique_ptr<oogl::Texture> in_lifetime_texture_;
        std::unique_ptr<oogl::Texture> out_value_texture_;
        std::unique_ptr<oogl::Texture> out_lifetime_texture_;
        
        std::unique_ptr<oogl::FrameBuffer> frame_buffer_;
        std::unique_ptr<oogl::RenderBuffer> depth_render_buffer_;

        oogl::Uniform2d uniform_viewport_bottom_left_;
        oogl::Uniform1d uniform_viewport_size_;

        oogl::Uniform1i uniform_value_texture_;
        oogl::Uniform1i uniform_lifetime_texture_;

        oogl::Uniform1i uniform_iterations_per_step_;

        bool resolution_needs_update_ = true;
        bool viewport_position_needs_update_ = true;
        bool viewport_size_needs_update_ = true;
        bool iterations_per_step_needs_update_ = true;
    };
}
