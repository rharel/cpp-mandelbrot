#include <mandelbrot/SimpleColoringStage.h>
#include <mandelbrot/ComputationStage.h>


using namespace mandelbrot;
using namespace oogl;


const char* SimpleColoringStage::VERTEX_SHADER_SOURCE_PATH =
    "../src/shaders/simpleColoringVertexShader.glsl";
const char* SimpleColoringStage::FRAGMENT_SHADER_SOURCE_PATH =
    "../src/shaders/simpleColoringFragmentShader.glsl";

const GLint SimpleColoringStage::LIFETIME_TEXTURE_UNIT_INDEX = 0;
const GLint SimpleColoringStage::LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX = 1;

const unsigned char SimpleColoringStage::DEFAULT_COLOR_MAP_DATA[3 * 8] = 
{
    0 * 32, 0 * 32, 0 * 32,
    1 * 32, 1 * 32, 1 * 32,
    2 * 32, 2 * 32, 2 * 32,
    3 * 32, 3 * 32, 3 * 32,
    4 * 32, 4 * 32, 4 * 32,
    5 * 32, 5 * 32, 5 * 32,
    6 * 32, 6 * 32, 6 * 32,
    7 * 32, 7 * 32, 7 * 32
};

SimpleColoringStage::SimpleColoringStage()
    : ProcessingStage
      (
          VERTEX_SHADER_SOURCE_PATH,
          FRAGMENT_SHADER_SOURCE_PATH
      ),
      color_map_(8, 3, DEFAULT_COLOR_MAP_DATA) {}

bool SimpleColoringStage::Initialize()
{
    return ProcessingStage::Initialize() &&
           InitializeTextures() &&
           InitializeBuffers() &&
           InitializeUniforms();
}
bool SimpleColoringStage::InitializeTextures()
{
    in_lifetime_color_map_texture_ = std::make_unique<Texture>
    (
        Texture::Binding::Texture1D,
        GL_RGB, color_map_.size(), 1
    );
    in_lifetime_color_map_texture_->Bind();
    in_lifetime_color_map_texture_->Resize(color_map_.size(), 1);
    in_lifetime_color_map_texture_->UploadData
    (
        0, 
        GL_RGB, GL_UNSIGNED_BYTE, 
        color_map_.data()
    );
    in_lifetime_color_map_texture_->set_filters(Texture::Filter::Nearest);

    out_colored_texture_ = std::make_unique<Texture>
    (
        Texture::Binding::Texture2D,
        GL_RGB, 1, 1
    );
    out_colored_texture_->Bind();
    out_colored_texture_->UploadData
    (
        0,
        GL_RGB, GL_UNSIGNED_BYTE,
        nullptr
    );
    out_colored_texture_->set_filters(Texture::Filter::Nearest);

    return true;
}
bool SimpleColoringStage::InitializeBuffers()
{
    frame_buffer_ = std::make_unique<FrameBuffer>();
    frame_buffer_->Bind();

    depth_render_buffer_ = std::make_unique<RenderBuffer>();
    depth_render_buffer_->Bind();
    RenderBuffer::DefineStorage
    (
        GL_DEPTH_COMPONENT, 
        out_colored_texture_->width(), 
        out_colored_texture_->height()
    );

    frame_buffer_->AttachRenderBuffer
    (
        *depth_render_buffer_, 
        GL_DEPTH_ATTACHMENT
    );
    frame_buffer_->AttachTexture
    (
        *out_colored_texture_, 
        GL_COLOR_ATTACHMENT0
    );
    
    std::string message;
    if (!frame_buffer_->is_complete(message))
    {
        status_message_ = "Frame buffer incomplete: " + message;
        return false;
    }
    return true;
}
bool SimpleColoringStage::InitializeUniforms()
{
    program_->Use();

    uniform_lifetime_texture_ = 
        program_->GetVectorUniform<GLint, 1>("lifetime_texture");
    uniform_lifetime_texture_
        .set(LIFETIME_TEXTURE_UNIT_INDEX);

    uniform_lifetime_color_map_texture_ =
        program_->GetVectorUniform<GLint, 1>("lifetime_color_map_texture");
    uniform_lifetime_color_map_texture_
        .set(LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX);

    return uniform_lifetime_texture_.is_valid() &&
           uniform_lifetime_color_map_texture_.is_valid();
}

void SimpleColoringStage::Execute()
{
    program_->Use();
    frame_buffer_->Bind();
    
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    in_lifetime_texture_
        ->BindToUnit(LIFETIME_TEXTURE_UNIT_INDEX);
    in_lifetime_color_map_texture_
        ->BindToUnit(LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX);

    glViewport
    (
        0, 0, 
        out_colored_texture_->width(), 
        out_colored_texture_->height()
    );

    DrawScreenQuad();
}

void SimpleColoringStage::set_texture_size
(
    const GLint width,
    const GLint height
)
{
    out_colored_texture_->Bind();
    out_colored_texture_->Resize(width, height);

    depth_render_buffer_->Bind();
    RenderBuffer::DefineStorage
    (
        GL_DEPTH_COMPONENT, 
        width, height
    );
}
void SimpleColoringStage::set_lifetime_texture(Texture& value)
{
    in_lifetime_texture_ = &value;
}

const ColorArray& SimpleColoringStage::color_map() const
{
    return color_map_;
}
void SimpleColoringStage::set_color_map(const ColorArray& value)
{
    if (value.is_empty()) { return; }
    
    color_map_ = value;
}

const Texture& SimpleColoringStage::colored_texture() const
{
    return *out_colored_texture_;
}
