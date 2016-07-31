#include <mandelbrot/Application.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>

#include <soil/soil.h>

#include <oogl/info.hpp>


using namespace mandelbrot;


const char* Application::WINDOW_TITLE = "Mandelbrot Explorer";
std::unique_ptr<Application> Application::instance_;

void Application::Initialize
(
    const unsigned int window_width,
    const unsigned int window_height
)
{
    instance_.reset(new Application
    (
        window_width, 
        window_height
    ));
}
Application& Application::instance()
{
    return *instance_;
}

void Application::WindowSizeCallback
(
    GLFWwindow* window,
    const int width,
    const int height
)
{
    auto application = 
        static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (application != nullptr)
    {
        application->WindowSizeCallback(width, height);
    }
}
void Application::WindowSizeCallback
(
    const int width,
    const int height
)
{
    window_size_ = Vector2u(width, height);

    renderer_.set_display_size(Vector2u(width, height));
    renderer_.Reset();
    Step();
}

void Application::KeyCallback
(
    GLFWwindow* window,
    const int key, 
    const int scancode,
    const int action,
    const int mods
)
{
    auto application = 
        static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (application != nullptr)
    {
        application->KeyCallback(key, scancode, action, mods);
    }
}
void Application::KeyCallback
(
    const int key, 
    const int,
    const int action,
    const int modifiers
)
{
    keyboard_controller_.Process(key, action, modifiers);
}

Application::Application
(
    const unsigned int window_width,
    const unsigned int window_height
)
    : window_size_(window_width, window_height),
      keyboard_controller_(*this) {}

bool Application::Launch()
{
    if (!Initialize()) { return false; }
    
    Play();
    EnterMainLoop();
    Dispose();

    return true;
}
void Application::Close()
{
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

bool Application::Initialize()
{
    return InitializeWindow() &&
           InitializeOpenGL() &&
           InitializeRenderer();
}
bool Application::InitializeWindow()
{
    if (!glfwInit()) { return false; }

    window_ = glfwCreateWindow
    (
        window_size_.x,
        window_size_.y,
        WINDOW_TITLE, 
        nullptr, nullptr
    );
    if (window_ == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(window_, this);
    glfwSetWindowSizeCallback(window_, Application::WindowSizeCallback);
    glfwSetKeyCallback(window_, Application::KeyCallback);

    return true;
}
bool Application::InitializeOpenGL()
{
    glfwMakeContextCurrent(window_);
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return false;
    }

    std::cout << "OpenGL version: " + oogl::version_string() 
              << std::endl;

    return true;
}
bool Application::InitializeRenderer()
{
    if (!renderer_.Initialize())
    {
        std::cout << renderer_.status_message() << std::endl;
        return false;
    }
    renderer_.set_display_size(window_size_);
    UpdateViewport();

    return true;
}

void Application::Dispose()
{
    glfwTerminate();
}

void Application::Pause()
{
    is_paused_ = true;
}
void Application::Play()
{
    is_paused_ = false;
}
void Application::TogglePause()
{
    is_paused_ = !is_paused_;
}
void Application::Step()
{
    is_stepping_ = true;
}

void Application::IncreasePrecision()
{
    renderer_.set_max_step_count(renderer_.max_step_count() + 1);
    renderer_.Reset();
    Step();
}
void Application::DecreasePrecision()
{
    renderer_.set_max_step_count(renderer_.max_step_count() - 1);
    renderer_.Reset();
    Step();
}

bool Application::SaveSnapshot(const char* name)
{
    std::string path;

    if (name == nullptr)
    {
        path = std::string
        (
            std::string("mandelbrot_snapshot_") + 
            std::to_string(session_saved_snapshots_count_)
        );
    }
    else { path = name; }

    const std::string image_path = path + ".bmp";
    const std::string state_path = path + ".txt";

    const bool success = 
        SaveImage(image_path.c_str()) &&
        SaveState(state_path.c_str());

    if (success) { ++ session_saved_snapshots_count_; }

    return success;
}
bool Application::SaveImage(const char* path)
{
    const unsigned int width = renderer_.resolution();
    const unsigned int height = renderer_.resolution();
    const unsigned int channel_count = 3;

    std::vector<unsigned char> pixels(channel_count * width * height);
    renderer_.GetImagePixels(pixels.data());

    const bool success = SOIL_save_image
    (
        path, SOIL_SAVE_TYPE_BMP,
        width, height, channel_count,
        pixels.data()
    ) != 0;

    if (success)
    {
        std::cout << "Saved image: " 
                  << path 
                  << std::endl;
    }
    else
    {
        std::cout << "Error saving image: " 
                  << path 
                  << std::endl;
    }
    return success;
}
bool Application::SaveState(const char* path)
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        std::cout << "Error saving state: " 
                  << path 
                  << std::endl;
        return false;
    }
    
    const int double_precision = 
        std::numeric_limits<double>::max_digits10;
    
    file << std::setprecision(double_precision)
         << std::fixed
         << renderer_.display_viewport().position.x
         << std::endl
         << renderer_.display_viewport().position.y
         << std::endl
         << renderer_.display_viewport().size
         << std::endl;
    
    file.close();

    std::cout << "Saved state: " 
              << path 
              << std::endl;

    return true;
}

void Application::EnterMainLoop()
{
    while (!glfwWindowShouldClose(window_))
    {
        HandleEvents();
        RenderFrame(); 
    }
}
void Application::HandleEvents()
{
    stopwatch_.Stop();

    glfwPollEvents();
    if (UpdateCamera())
    {
        UpdateViewport();
        renderer_.Reset();
    }

    stopwatch_.Start();
}
void Application::RenderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | 
            GL_DEPTH_BUFFER_BIT);

    const bool is_rendering = !is_paused_ || is_stepping_;

    if (is_rendering && !renderer_.is_done())
    {
        renderer_.RenderStep();
    }
    if (renderer_.is_done())
    {
        renderer_.Flush();
        is_stepping_ = false;
    }
    renderer_.Render();

    glfwSwapBuffers(window_);
}

bool Application::UpdateCamera()
{
    bool camera_changed = false;

    const auto elapsed_microseconds = 
        std::chrono::duration_cast<std::chrono::microseconds>
    (
        stopwatch_.nanoseconds()
    ).count();
    const double elapsed_seconds = elapsed_microseconds / 1000000.0;

    const Vector2d movement_signal
    (
        keyboard_controller_.horizontal_axis().signal(),
        keyboard_controller_.vertical_axis().signal()
    );
    if (LengthSquared(movement_signal) != 0)
    {
        camera_.Move(Normal(movement_signal), elapsed_seconds);
        camera_changed = true;
    }
    
    const int zoom_signal = keyboard_controller_.zoom_axis().signal();
    if (zoom_signal != 0)
    {
        camera_.Zoom(zoom_signal, elapsed_seconds);
        camera_changed = true;
    }

    return camera_changed;
}
void Application::UpdateViewport()
{
    renderer_.set_viewport_position(camera_.position());
    renderer_.set_viewport_size(camera_.zoom_factor());
}

const Camera& Application::camera() const
{
    return camera_;
}
Camera& Application::camera()
{
    return camera_;
}

const Renderer& Application::renderer() const
{
    return renderer_;
}
Renderer& Application::renderer()
{
    return renderer_;
}

void Application::PrintDebugInformation() const
{
    const int double_precision = 
        std::numeric_limits<double>::max_digits10;

    std::cout << std::setprecision(double_precision)
              << std::fixed
              << "X: "
              << renderer_.viewport().position.x
              << std::endl
              << "Y: "
              << renderer_.viewport().position.y
              << std::endl
              << "Size: "
              << renderer_.viewport().size
              << std::endl;
}
