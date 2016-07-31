/**
 * Processing stage abstract class.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <memory>
#include <string>

#include <oogl/Shader.hpp>
#include <oogl/Program.hpp>
#include <oogl/VertexArray.hpp>
#include <oogl/VertexBuffer.hpp>


namespace mandelbrot
{
    /**
     * Compiles program object and renders a full-screen quad.
     */
    class ProcessingStage
    {
        public:
        /**
         * Creates a new stage from given shader source files.
         */
        ProcessingStage
        (
            const char* vertex_shader_source_path,
            const char* fragment_shader_source_path
        );

        /**
         * Initializes stage.
         */
        virtual bool Initialize();

        /**
         * Executes stage.
         */
        virtual void Execute();

        /**
         * Checks whether stage is properly initialized.
         */
        virtual bool is_ready() const;
        /**
         * Gets status string.
         */
        const char* status_message() const;

        protected:
        /**
         * Draws full-screen quad.
         */
        void DrawScreenQuad();

        std::unique_ptr<oogl::Program> program_;
        std::string status_message_;

        private:
        static const GLuint ATTRIBUTE_POSITION_INDEX;
        static const GLfloat SCREEN_QUAD_TRIANGLES[2 * 3 * 3];

        bool InitializeShaders();
        bool InitializeProgram();

        void CreateAttributes();
        void CreatePositionAttribute();

        std::string vertex_shader_source_path_;
        std::string fragment_shader_source_path_;

        std::unique_ptr<oogl::Shader> vertex_shader_;
        std::unique_ptr<oogl::Shader> fragment_shader_;

        std::unique_ptr<oogl::VertexArray> vertex_array_;
        std::unique_ptr<oogl::VertexBuffer> position_buffer_;
    };
}
