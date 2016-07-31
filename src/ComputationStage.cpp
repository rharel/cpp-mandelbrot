#include <mandelbrot\ComputationStage.h>


using namespace mandelbrot;
using namespace oogl;


const char* ComputationStage::VERTEX_SHADER_SOURCE_PATH =
    "../src/shaders/computeVertexShader.glsl";
const char* ComputationStage::FRAGMENT_SHADER_SOURCE_PATH =
    "../src/shaders/computeFragmentShader.glsl";

const GLint ComputationStage::VALUE_TEXTURE_UNIT_INDEX = 0;
const GLint ComputationStage::LIFETIME_TEXTURE_UNIT_INDEX = 1;

ComputationStage::ComputationStage()
    : ProcessingStage
      (
        VERTEX_SHADER_SOURCE_PATH,
        FRAGMENT_SHADER_SOURCE_PATH
      ) {}

bool ComputationStage::Initialize()
{
    return ProcessingStage::Initialize() &&
           InitializeTextures() &&
           InitializeBuffers() && 
           InitializeUniforms();
}
bool ComputationStage::InitializeTextures()
{
    return InitializeTexture
    (
        GL_RG32F, GL_RG, GL_FLOAT, 
        in_value_texture_
    ) &&
    InitializeTexture
    (
        GL_R32I, GL_RED_INTEGER, GL_INT, 
        in_lifetime_texture_
    ) &&
    InitializeTexture(
        GL_RG32F, GL_RG, GL_FLOAT,
        out_value_texture_
    ) &&
    InitializeTexture
    (
        GL_R32I, GL_RED_INTEGER, GL_INT, 
        out_lifetime_texture_
    );
}
bool ComputationStage::InitializeTexture
(
    const GLint internal_format,
    const GLenum data_format,
    const GLenum data_type,
    std::unique_ptr<Texture>& texture
)
{
    texture = std::make_unique<Texture>
    (
        Texture::Binding::Texture2D,
        internal_format,
        resolution_, resolution_
    );
    
    texture->Bind();
    texture->UploadData(0, data_format, data_type, nullptr);

    texture->set_filters(Texture::Filter::Nearest);

    return true;
}
bool ComputationStage::InitializeBuffers()
{
    frame_buffer_ = std::make_unique<FrameBuffer>();
    frame_buffer_->Bind();

    depth_render_buffer_ = std::make_unique<RenderBuffer>();
    depth_render_buffer_->Bind();
    RenderBuffer::DefineStorage
    (
        GL_DEPTH_COMPONENT, 
        resolution_, resolution_
    );

    frame_buffer_->AttachRenderBuffer
    (
        *depth_render_buffer_, 
        GL_DEPTH_ATTACHMENT
    );
    frame_buffer_->AttachTexture
    (
        *out_value_texture_, 
        GL_COLOR_ATTACHMENT0
    );
    frame_buffer_->AttachTexture
    (
        *out_lifetime_texture_, 
        GL_COLOR_ATTACHMENT1
    );
    
    std::string message;
    if (!frame_buffer_->is_complete(message))
    {
        status_message_ = "Frame buffer incomplete: " + message;
        return false;
    }
    return true;
}
bool ComputationStage::InitializeUniforms()
{
    program_->Use();

    uniform_viewport_bottom_left_ = 
        program_->GetVectorUniform<GLdouble, 2>("viewport_bottom_left");
    uniform_viewport_size_ =
        program_->GetVectorUniform<GLdouble, 1>("viewport_size");

    uniform_value_texture_ =
        program_->GetVectorUniform<GLint, 1>("value_texture");
    uniform_value_texture_.set(VALUE_TEXTURE_UNIT_INDEX);

    uniform_lifetime_texture_ = 
        program_->GetVectorUniform<GLint, 1>("lifetime_texture");
    uniform_lifetime_texture_.set(LIFETIME_TEXTURE_UNIT_INDEX);

    uniform_iterations_per_step_ = program_->GetVectorUniform<GLint, 1>("dt");

    return uniform_viewport_bottom_left_.is_valid() &&
           uniform_viewport_size_.is_valid() &&
           uniform_value_texture_.is_valid() &&
           uniform_lifetime_texture_.is_valid() &&
           uniform_iterations_per_step_.is_valid();
}

void ComputationStage::Reset()
{
    GLfloat null_value_data[2] { 0, 0 };
    out_value_texture_->ClearData
    (
        0, 
        GL_RG, GL_FLOAT, 
        null_value_data
    );
    GLint null_lifetime_data[1] { 0 };
    out_lifetime_texture_->ClearData
    (
        0, 
        GL_RED_INTEGER, GL_INT, 
        null_lifetime_data
    );
}
void ComputationStage::Execute()
{
    program_->Use();
    frame_buffer_->Bind();

    UpdateUniforms();
    UpdateResolution();
    SwapBuffers();
    ComputeStep();
}

void ComputationStage::UpdateUniforms()
{
    if (viewport_position_needs_update_)
    {
        const Vector2d bottom_left =
            viewport_.position - 0.5 * viewport_.size;
        uniform_viewport_bottom_left_.set
        (
            bottom_left.x, 
            bottom_left.y
        );
        viewport_position_needs_update_ = false;
    }
    if (viewport_size_needs_update_)
    {
        uniform_viewport_size_.set(viewport_.size);
        viewport_size_needs_update_ = false;
    }
    if (iterations_per_step_needs_update_)
    {
        uniform_iterations_per_step_.set(iterations_per_step_);
        iterations_per_step_needs_update_ = false;
    }
}
void ComputationStage::UpdateResolution()
{
    if (!resolution_needs_update_) { return; }

    in_value_texture_->Bind();
    in_value_texture_->Resize(resolution_, resolution_);
    
    in_lifetime_texture_->Bind();
    in_lifetime_texture_->Resize(resolution_, resolution_);
    
    out_value_texture_->Bind();
    out_value_texture_->Resize(resolution_, resolution_);
    
    out_lifetime_texture_->Bind();
    out_lifetime_texture_->Resize(resolution_, resolution_);

    depth_render_buffer_->Bind();
    RenderBuffer::DefineStorage
    (
        GL_DEPTH_COMPONENT, 
        resolution_, resolution_
    );

    Reset();

    resolution_needs_update_ = false;
}
void ComputationStage::SwapBuffers()
{
    std::swap(in_value_texture_, out_value_texture_);
    std::swap(in_lifetime_texture_, out_lifetime_texture_);

    frame_buffer_->AttachTexture
    (
        *out_value_texture_, 
        GL_COLOR_ATTACHMENT0
    );
    frame_buffer_->AttachTexture
    (
        *out_lifetime_texture_, 
        GL_COLOR_ATTACHMENT1
    );
}
void ComputationStage::ComputeStep()
{
    GLenum draw_buffers[2] = 
    {
        GL_COLOR_ATTACHMENT0, 
        GL_COLOR_ATTACHMENT1
    };
    glDrawBuffers(2, draw_buffers);

    in_value_texture_->BindToUnit(VALUE_TEXTURE_UNIT_INDEX);
    in_lifetime_texture_->BindToUnit(LIFETIME_TEXTURE_UNIT_INDEX);

    glViewport(0, 0, resolution_, resolution_);

    DrawScreenQuad();
}

unsigned int ComputationStage::resolution() const
{ 
    return resolution_;
}
void ComputationStage::set_resolution_power(const unsigned int value)
{
    resolution_ = static_cast<unsigned int>(std::pow(2, value));
    resolution_needs_update_ = true;
}

const Box2d& ComputationStage::viewport() const
{ 
    return viewport_; 
}
void ComputationStage::set_viewport_position(const Vector2d& value)
{
    viewport_.position = value;
    viewport_position_needs_update_ = true;
}
void ComputationStage::set_viewport_size(const double value)
{
    viewport_.size = value;
    viewport_size_needs_update_ = true;
}

unsigned int ComputationStage::iterations_per_step() const 
{
    return iterations_per_step_; 
}
void ComputationStage::set_iterations_per_step(const unsigned int value)
{
    iterations_per_step_ = std::max(value, 1U);
    iterations_per_step_needs_update_ = true;
}

Texture& ComputationStage::value_texture()
{
    return *out_value_texture_;
}
Texture& ComputationStage::lifetime_texture()
{
    return *out_lifetime_texture_;
}
