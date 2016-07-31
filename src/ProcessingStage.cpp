#include <mandelbrot/ProcessingStage.h>


using namespace mandelbrot;
using namespace oogl;


const GLuint ProcessingStage::ATTRIBUTE_POSITION_INDEX = 0;
const GLfloat ProcessingStage::SCREEN_QUAD_TRIANGLES[2 * 3 * 3]
{
    -1, -1, 0,
     1, -1, 0,
     1,  1, 0,

    -1, -1, 0,
     1,  1, 0,
    -1,  1, 0
};

ProcessingStage::ProcessingStage
(
    const char* vertex_shader_source_path,
    const char* fragment_shader_source_path
)
    : vertex_shader_source_path_(vertex_shader_source_path),
      fragment_shader_source_path_(fragment_shader_source_path) {}

bool ProcessingStage::Initialize()
{
    CreateAttributes();
    return InitializeShaders() && InitializeProgram();
}
bool ProcessingStage::InitializeShaders()
{
    vertex_shader_ = std::unique_ptr<Shader>
    (
        Shader::BuildFromPath
        (
            Shader::Type::Vertex,
            vertex_shader_source_path_.c_str()
        )
    );
    if (!vertex_shader_->is_compiled())
    {
        status_message_ = "Error initializing vertex shader:\n";
        status_message_ += vertex_shader_->info_log();
        return false;
    }

    fragment_shader_ = std::unique_ptr<Shader>
    (
        Shader::BuildFromPath
        (
            Shader::Type::Fragment,
            fragment_shader_source_path_.c_str()
        )
    );
    if (!fragment_shader_->is_compiled())
    {
        status_message_ = "Error initializing fragment shader:\n";
        status_message_ += fragment_shader_->info_log();
        return false;
    }

    return true;
}
bool ProcessingStage::InitializeProgram()
{
    program_ = std::unique_ptr<Program>
    (
        Program::Build
        ({
            vertex_shader_.get(), 
            fragment_shader_.get()
        })
    );
    
    if (!program_->is_linked())
    {
        status_message_ = "Error initializing program:\n";
        status_message_ += program_->info_log();
        return false;
    }

    vertex_shader_.reset();
    fragment_shader_.reset();

    return true;
}

void ProcessingStage::CreateAttributes()
{
    vertex_array_ = std::make_unique<VertexArray>();
    vertex_array_->Bind();

    CreatePositionAttribute();
}
void ProcessingStage::CreatePositionAttribute()
{
    position_buffer_ = std::make_unique<VertexBuffer>
    (
        VertexBuffer::Binding::Array,
        VertexBuffer::Usage::StaticDraw
    );
    position_buffer_->Bind();
    position_buffer_->UploadData
    (
        sizeof(SCREEN_QUAD_TRIANGLES),
        SCREEN_QUAD_TRIANGLES
    );
}

void ProcessingStage::Execute()
{
    program_->Use();
    DrawScreenQuad();
}
void ProcessingStage::DrawScreenQuad()
{
    vertex_array_->Bind();
    VertexArray::EnableAttribute(ATTRIBUTE_POSITION_INDEX);
    glVertexAttribPointer
    (
        ATTRIBUTE_POSITION_INDEX,
        3, GL_FLOAT, GL_FALSE,
        0, 0
    );
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VertexArray::DisableAttribute(ATTRIBUTE_POSITION_INDEX);
}

bool ProcessingStage::is_ready() const
{
    return program_->is_linked();
}
const char* ProcessingStage::status_message() const
{ 
    return status_message_.c_str(); 
}
