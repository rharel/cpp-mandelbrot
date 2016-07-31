#include <mandelbrot/SmoothColoringStage.h>
#include <mandelbrot/ComputationStage.h>


using namespace mandelbrot;
using namespace oogl;


const char* SmoothColoringStage::VERTEX_SHADER_SOURCE_PATH =
    "../src/shaders/smoothColoringVertexShader.glsl";
const char* SmoothColoringStage::FRAGMENT_SHADER_SOURCE_PATH =
    "../src/shaders/smoothColoringFragmentShader.glsl";

const GLint SmoothColoringStage::VALUE_TEXTURE_UNIT_INDEX = 0;
const GLint SmoothColoringStage::LIFETIME_TEXTURE_UNIT_INDEX = 1;
const GLint SmoothColoringStage::LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX = 2;

const unsigned char SmoothColoringStage::DEFAULT_COLOR_MAP_DATA[3 * 8] = 
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

SmoothColoringStage::SmoothColoringStage()
    : ProcessingStage
      (
          VERTEX_SHADER_SOURCE_PATH,
          FRAGMENT_SHADER_SOURCE_PATH
      ),
      color_map_(8, 3, DEFAULT_COLOR_MAP_DATA) {}

bool SmoothColoringStage::Initialize()
{
    return ProcessingStage::Initialize() &&
           InitializeTextures() &&
           InitializeBuffers() &&
           InitializeUniforms();
}
bool SmoothColoringStage::InitializeTextures()
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
bool SmoothColoringStage::InitializeBuffers()
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
bool SmoothColoringStage::InitializeUniforms()
{
    program_->Use();

    uniform_value_texture_ =
        program_->GetVectorUniform<GLint, 1>("value_texture");
    uniform_value_texture_
        .set(VALUE_TEXTURE_UNIT_INDEX);

    uniform_lifetime_texture_ = 
        program_->GetVectorUniform<GLint, 1>("lifetime_texture");
    uniform_lifetime_texture_
        .set(LIFETIME_TEXTURE_UNIT_INDEX);

    uniform_lifetime_color_map_texture_ =
        program_->GetVectorUniform<GLint, 1>("lifetime_color_map_texture");
    uniform_lifetime_color_map_texture_
        .set(LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX);

    uniform_max_lifetime_ = 
        program_->GetVectorUniform<GLint, 1>("max_lifetime");
    uniform_max_lifetime_.set(max_lifetime_);

    return uniform_value_texture_.is_valid() &&
           uniform_lifetime_texture_.is_valid() &&
           uniform_lifetime_color_map_texture_.is_valid() &&
           uniform_max_lifetime_.is_valid();
}

void SmoothColoringStage::Execute()
{
    program_->Use();
    frame_buffer_->Bind();
    
    UpdateTextureSize();
    UpdateColorMap();
    UpdateUniforms();

    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers);

    in_value_texture_
        ->BindToUnit(VALUE_TEXTURE_UNIT_INDEX);
    in_lifetime_texture_
        ->BindToUnit(LIFETIME_TEXTURE_UNIT_INDEX);
    in_lifetime_color_map_texture_
        ->BindToUnit(LIFETIME_COLOR_MAP_TEXTURE_UNIT_INDEX);

    glViewport(0, 0, texture_size_, texture_size_);

    DrawScreenQuad();
}

void SmoothColoringStage::UpdateTextureSize()
{
    if (!texture_size_needs_update_) { return; }

    out_colored_texture_->Bind();
    out_colored_texture_->Resize(texture_size_, texture_size_);

    depth_render_buffer_->Bind();
    RenderBuffer::DefineStorage
    (
        GL_DEPTH_COMPONENT, 
        texture_size_, texture_size_
    );

    texture_size_needs_update_ = false;
}
void SmoothColoringStage::UpdateColorMap()
{
    if (!color_map_needs_update_) { return; }

    in_lifetime_color_map_texture_->Bind();
    in_lifetime_color_map_texture_->Resize(color_map_.size(), 1);
    in_lifetime_color_map_texture_->UploadData
    (
        0, 
        GL_RGB, GL_UNSIGNED_BYTE, 
        color_map_.data()
    );

    color_map_needs_update_ = false;
}
void SmoothColoringStage::UpdateUniforms()
{
    if (max_lifetime_needs_update_)
    {
        uniform_max_lifetime_.set(max_lifetime_);
        max_lifetime_needs_update_ = false;
    }
}

void SmoothColoringStage::set_texture_size(const GLint value)
{
    texture_size_ = value;
    texture_size_needs_update_ = true;
}

void SmoothColoringStage::set_value_texture(Texture& value)
{
    in_value_texture_ = &value;
}
void SmoothColoringStage::set_lifetime_texture(Texture& value)
{
    in_lifetime_texture_ = &value;
}

const ColorArray& SmoothColoringStage::color_map() const
{
    return color_map_;
}
void SmoothColoringStage::set_color_map(const ColorArray& value)
{
    color_map_ = 
        value.is_empty() ?
        ColorArray(8, 3, DEFAULT_COLOR_MAP_DATA) :
        value;
    color_map_needs_update_ = true;
}

void SmoothColoringStage::set_max_lifetime(const GLint value)
{
    max_lifetime_ = value;
    max_lifetime_needs_update_ = true;
}

const Texture& SmoothColoringStage::colored_texture() const
{
    return *out_colored_texture_;
}
