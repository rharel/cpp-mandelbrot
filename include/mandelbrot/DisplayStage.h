/**
 * Renders cached texture with offset.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <memory>

#include <mandelbrot/ProcessingStage.h>
#include <mandelbrot/Box2.h>
#include <mandelbrot/Vector2.h>

#include <oogl/Program.hpp>
#include <oogl/Texture.hpp>


namespace mandelbrot
{
    class DisplayStage : public ProcessingStage
    {
        public:
        static const char* VERTEX_SHADER_SOURCE_PATH;
        static const char* FRAGMENT_SHADER_SOURCE_PATH;

        static const GLint IMAGE_TEXTURE_UNIT_INDEX;
        
        /**
         * Creates stage.
         */
        DisplayStage();

        /**
         * Initializes stage.
         */
        bool Initialize() override;

        /**
         * Executes stage.
         */
        void Execute() override;

        /**
         * Saves current state as a cached copy.
         */
        void UpdateCache
        (
            const oogl::Texture& texture,
            const Vector2d& position,
            double size
        );

        /**
         * Gets display window size.
         */
        const Vector2u& display_size() const;
        /**
         * Sets display window size.
         */
        void set_display_size(const Vector2u& value);

        /**
         * Gets saved viewport.
         */
        const Box2d& viewport() const;

        /**
         * Sets relative viewport center rposition offset.
         */
        void set_relative_position_offset(const Vector2d& value);
        /**
         * Sets relative viewport size offset.
         */
        void set_relative_size_offset(double value);

        /**
         * Gets cached image.
         */
        oogl::Texture& image();

        private:
        bool InitializeTextures();
        bool InitializeUniforms();

        void UpdateUniforms();

        Vector2u display_size_;

        Box2d viewport_;

        Vector2d relative_position_offset_ = Vector2d(0, 0);
        double relative_size_factor_ = 1.0f;

        std::unique_ptr<oogl::Texture> image_texture_;

        oogl::Uniform1i uniform_image_;
        oogl::Uniform2d uniform_relative_position_offset_;
        oogl::Uniform1d uniform_relative_size_factor_;

        bool relative_position_offset_needs_update_ = true;
        bool relative_size_factor_needs_update_ = true;
    };
}
