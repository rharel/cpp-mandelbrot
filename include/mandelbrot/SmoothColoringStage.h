/**
 * Colors based on escape time.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <mandelbrot/ProcessingStage.h>
#include <mandelbrot/ColorArray.h>
#include <mandelbrot/Vector2.h>

#include <oogl/Program.hpp>
#include <oogl/Texture.hpp>
#include <oogl/FrameBuffer.hpp>
#include <oogl/RenderBuffer.hpp>


namespace mandelbrot
{
    /**
     * Renders colored mandelbrot image based on given value- and escape
     * time textures.
     */
    class SmoothColoringStage : public ProcessingStage
    {
        public:
        static const char* VERTEX_SHADER_SOURCE_PATH;
        static const char* FRAGMENT_SHADER_SOURCE_PATH;

        static const GLint VALUE_TEXTURE_UNIT_INDEX;
        static const GLint LIFETIME_TEXTURE_UNIT_INDEX;
        static const GLint LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX;

        /**
         * Creates a new stage.
         */
        SmoothColoringStage();

        /**
         * Initializes stage.
         */
        bool Initialize() override;

        /**
         * Executes stage.
         */
        void Execute() override;

        /**
         * Sets expected input/output texture size.
         */
        void set_texture_size(GLint value);
        /**
         * Sets value texture source.
         */
        void set_value_texture(oogl::Texture& value);
        /**
         * Sets escape time texture source.
         */
        void set_lifetime_texture(oogl::Texture& value);

        /**
         * Gets color map.
         */
        const ColorArray& color_map() const;
        /**
         * Sets color map.
         */
        void set_color_map(const ColorArray& value);

        /**
         * Sets maximum escape time.
         */
        void set_max_lifetime(GLint value);

        /**
         * Gets output colored texture.
         */
        const oogl::Texture& colored_texture() const;

        private:
        static const unsigned char DEFAULT_COLOR_MAP_DATA[];

        bool InitializeTextures();
        bool InitializeBuffers();
        bool InitializeUniforms();

        void UpdateTextureSize();
        void UpdateColorMap();
        void UpdateUniforms();

        GLint texture_size_;
        ColorArray color_map_;
        GLint max_lifetime_;

        oogl::Texture* in_value_texture_;
        oogl::Texture* in_lifetime_texture_;
        std::unique_ptr<oogl::Texture> in_lifetime_color_map_texture_;
        std::unique_ptr<oogl::Texture> out_colored_texture_;

        std::unique_ptr<oogl::FrameBuffer> frame_buffer_;
        std::unique_ptr<oogl::RenderBuffer> depth_render_buffer_;

        oogl::Uniform1i uniform_value_texture_;
        oogl::Uniform1i uniform_lifetime_texture_;
        oogl::Uniform1i uniform_lifetime_color_map_texture_;
        oogl::Uniform1i uniform_max_lifetime_;

        bool texture_size_needs_update_ = true;
        bool color_map_needs_update_ = true;
        bool max_lifetime_needs_update_ = true;
    };
}
