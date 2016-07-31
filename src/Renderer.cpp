#include <mandelbrot\Renderer.h>


using namespace mandelbrot;
using namespace oogl;


bool Renderer::Initialize()
{
    if (!computation_stage_.Initialize())
    {
        status_message_ = std::string("Computation stage:\n") + 
                          computation_stage_.status_message();
        return false;
    }
    if (!coloring_stage_.Initialize())
    {
        status_message_ = std::string("Coloring stage:\n") + 
                          coloring_stage_.status_message();
        return false;
    }
    if (!display_stage_.Initialize())
    {
        status_message_ = std::string("Caching stage:\n") +
                          display_stage_.status_message();
        return false;
    }

    const unsigned int max_iteration_count = 
        max_step_count_ * iterations_per_step();

    coloring_stage_.set_max_lifetime(max_iteration_count);

    return true;
}

void Renderer::Reset()
{
    computation_stage_.Reset();
    step_count_ = 0;
}
void Renderer::RenderStep()
{
    computation_stage_.Execute();
    ++ step_count_;
}
void Renderer::Flush()
{
    coloring_stage_.set_value_texture
    (
        computation_stage_.value_texture()
    );
    coloring_stage_.set_lifetime_texture
    (
        computation_stage_.lifetime_texture()
    );
    coloring_stage_.Execute();

    display_stage_.UpdateCache
    (
        coloring_stage_.colored_texture(),
        viewport().position,
        viewport().size
    );
}
void Renderer::Render()
{
    display_stage_.set_relative_position_offset
    (
        (
            viewport().position - 
            display_stage_.viewport().position
        ) / 
        display_stage_.viewport().size
    );
    display_stage_.set_relative_size_offset
    (
        viewport().size /
        display_stage_.viewport().size
    );
    display_stage_.Execute();
}

void Renderer::GetImagePixels(unsigned char* buffer)
{
    Texture& image = display_stage_.image();
    image.Bind();
    image.DownloadData
    (
        0,
        GL_RGB, GL_UNSIGNED_BYTE,
        buffer
    );
}

bool Renderer::is_ready() const
{
    return computation_stage_.is_ready() &&
           coloring_stage_.is_ready() &&
           display_stage_.is_ready();
}
const char* Renderer::status_message() const
{ 
    return status_message_.c_str(); 
}

bool Renderer::is_done() const
{
    return step_count_ >= max_step_count_;
}

const Vector2u& Renderer::display_size() const
{
    return display_stage_.display_size();
}
void Renderer::set_display_size(const Vector2u& value)
{
    display_stage_.set_display_size(value);
}

unsigned int Renderer::resolution() const
{
    return computation_stage_.resolution();
}
void Renderer::set_resolution(unsigned int value)
{
    unsigned int resolution = 2;
    unsigned int resolution_power = 1;
    while (2 * resolution <= value)
    {
        ++ resolution_power;
        resolution *= 2;
    }
    computation_stage_.set_resolution_power(resolution_power);
    coloring_stage_.set_texture_size(resolution);
}
void Renderer::set_resolution(const Resolution power)
{
    const auto value = static_cast<unsigned int>
    (
        std::pow(2, static_cast<unsigned int>(power))
    );
    set_resolution(value);
}

const ColorArray& Renderer::color_map() const
{
    return coloring_stage_.color_map();
}
void Renderer::set_color_map(const ColorArray& value)
{
    coloring_stage_.set_color_map(value);
}

unsigned int Renderer::iterations_per_step() const
{
    return computation_stage_.iterations_per_step();
}
void Renderer::set_iterations_per_step(const unsigned int value)
{
    computation_stage_.set_iterations_per_step(value);
}

unsigned int Renderer::max_step_count() const
{
    return max_step_count_;
}
void Renderer::set_max_step_count(const unsigned int value)
{
    max_step_count_ = std::max(value, 1U);

    const unsigned int max_iteration_count = 
        max_step_count_ * iterations_per_step();

    coloring_stage_.set_max_lifetime(max_iteration_count);
}

const Box2d& Renderer::viewport() const
{
    return computation_stage_.viewport();
}
void Renderer::set_viewport_position(const Vector2d& value)
{
    computation_stage_.set_viewport_position(value);
}
void Renderer::set_viewport_size(const double value)
{
    computation_stage_.set_viewport_size(value);
}
const Box2d& Renderer::display_viewport() const
{
    return display_stage_.viewport();
}
