#include <mandelbrot/DisplayStage.h>


using namespace mandelbrot;
using namespace oogl;


const char* DisplayStage::VERTEX_SHADER_SOURCE_PATH =
    "../src/shaders/simpleTextureVertexShader.glsl";
const char* DisplayStage::FRAGMENT_SHADER_SOURCE_PATH =
    "../src/shaders/simpleTextureFragmentShader.glsl";

const GLint DisplayStage::IMAGE_TEXTURE_UNIT_INDEX = 0;

DisplayStage::DisplayStage()
    : ProcessingStage
     (
         VERTEX_SHADER_SOURCE_PATH,
         FRAGMENT_SHADER_SOURCE_PATH
     ) {}

bool DisplayStage::Initialize()
{
    return ProcessingStage::Initialize() &&
           InitializeTextures() &&
           InitializeUniforms();
}
bool DisplayStage::InitializeTextures()
{
    image_texture_ = std::make_unique<Texture>
    (
        Texture::Binding::Texture2D,
        GL_RGB,
        1, 1
    );

    image_texture_->Bind();
    image_texture_->UploadData(0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    image_texture_->set_filters(Texture::Filter::Nearest);
    image_texture_->set_wrappers(Texture::Wrapper::ClampToBorder);

    return image_texture_->is_valid();
}
bool DisplayStage::InitializeUniforms()
{
    program_->Use();

    uniform_image_ = 
        program_->GetVectorUniform<GLint, 1>("image_texture");
    uniform_image_
        .set(IMAGE_TEXTURE_UNIT_INDEX);

    uniform_relative_position_offset_ =
        program_->GetVectorUniform<GLdouble, 2>("relative_position_offset");
    uniform_relative_size_factor_ =
        program_->GetVectorUniform<GLdouble, 1>("relative_size_factor");

    return uniform_image_.is_valid() &&
           uniform_relative_position_offset_.is_valid() &&
           uniform_relative_size_factor_.is_valid();
}

void DisplayStage::Execute()
{
    program_->Use();
    
    UpdateUniforms();
    
    image_texture_->BindToUnit(IMAGE_TEXTURE_UNIT_INDEX);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    const GLint min = Min(display_size_);
    const GLint max = Max(display_size_);
    const GLint offset = (min - max) / 2;
    GLint x, y;
    if (display_size_.x > display_size_.y)
    {
        x = 0;
        y = offset;
    }
    else
    {
        x = offset;
        y = 0;
    }
    
    glViewport(x, y, max, max);

    DrawScreenQuad();
}

void DisplayStage::UpdateUniforms()
{
    if (relative_position_offset_needs_update_)
    {
        uniform_relative_position_offset_.set
        (
            relative_position_offset_.x,
            relative_position_offset_.y
        );
        relative_position_offset_needs_update_ = false;
    }
    if (relative_size_factor_needs_update_)
    {
        uniform_relative_size_factor_.set(relative_size_factor_);
        relative_size_factor_needs_update_ = false;
    }
}

void DisplayStage::UpdateCache
(
    const Texture& texture,
    const Vector2d& position,
    const double size
)
{
    image_texture_->Bind();
    image_texture_->CopyData(texture);
    
    viewport_.position = position;
    viewport_.size = size;
}

const Vector2u& DisplayStage::display_size() const
{
    return display_size_;
}
void DisplayStage::set_display_size(const Vector2u& value)
{
    display_size_ = value;
}

const Box2d& DisplayStage::viewport() const
{
    return viewport_;
}

void DisplayStage::set_relative_position_offset(const Vector2d& value)
{
    relative_position_offset_ = value;
    relative_position_offset_needs_update_ = true;
}
void DisplayStage::set_relative_size_offset(const double value)
{
    relative_size_factor_ = value;
    relative_size_factor_needs_update_ = true;
}

Texture& DisplayStage::image()
{
    return *image_texture_;
}
