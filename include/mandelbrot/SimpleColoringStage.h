/**
 * Colors based on survived iteration counts.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <mandelbrot/ProcessingStage.h>
#include <mandelbrot/Vector2.h>
#include <mandelbrot/ColorArray.h>

#include <oogl/Program.hpp>
#include <oogl/Texture.hpp>
#include <oogl/FrameBuffer.hpp>
#include <oogl/RenderBuffer.hpp>


namespace mandelbrot
{
    class SimpleColoringStage : public ProcessingStage
    {
        public:
        static const char* VERTEX_SHADER_SOURCE_PATH;
        static const char* FRAGMENT_SHADER_SOURCE_PATH;

        static const GLint LIFETIME_TEXTURE_UNIT_INDEX;
        static const GLint LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX;

        SimpleColoringStage();

        bool Initialize() override;

        void Execute() override;

        void set_texture_size(GLint width, GLint height);
        void set_lifetime_texture(oogl::Texture& value);

        const ColorArray& color_map() const;
        void set_color_map(const ColorArray& value);

        const oogl::Texture& colored_texture() const;

        private:
        static const unsigned char DEFAULT_COLOR_MAP_DATA[];

        bool InitializeTextures();
        bool InitializeBuffers();
        bool InitializeUniforms();

        ColorArray color_map_;

        oogl::Texture* in_lifetime_texture_;
        std::unique_ptr<oogl::Texture> in_lifetime_color_map_texture_;
        std::unique_ptr<oogl::Texture> out_colored_texture_;

        std::unique_ptr<oogl::FrameBuffer> frame_buffer_;
        std::unique_ptr<oogl::RenderBuffer> depth_render_buffer_;

        oogl::Uniform1i uniform_lifetime_texture_;
        oogl::Uniform1i uniform_lifetime_color_map_texture_;
    };
}
