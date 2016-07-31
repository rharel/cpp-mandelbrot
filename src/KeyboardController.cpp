#include <mandelbrot/KeyboardController.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <mandelbrot/Application.h>


using namespace mandelbrot;


const int KeyboardController::KEY_EXIT = GLFW_KEY_ESCAPE;
const int KeyboardController::KEY_LEFT = GLFW_KEY_A;
const int KeyboardController::KEY_RIGHT = GLFW_KEY_D;
const int KeyboardController::KEY_UP = GLFW_KEY_W;
const int KeyboardController::KEY_DOWN = GLFW_KEY_S;
const int KeyboardController::KEY_ZOOM_IN = GLFW_KEY_R;
const int KeyboardController::KEY_ZOOM_OUT = GLFW_KEY_F;
const int KeyboardController::KEY_INCREASE_PRECISION = GLFW_KEY_T;
const int KeyboardController::KEY_DECREASE_PRECISION = GLFW_KEY_G;
const int KeyboardController::KEY_TOGGLE_PAUSE = GLFW_KEY_TAB;
const int KeyboardController::KEY_STEP = GLFW_KEY_SPACE;
const int KeyboardController::KEY_SAVE_SNAPSHOT = GLFW_KEY_S;
const int KeyboardController::KEY_DEBUG = GLFW_KEY_SEMICOLON;

void KeyboardController::ControlAxis::PressUp()
{
    is_signaling_up_ = true;
    most_recent_signal_ = Signal::Up;
}
void KeyboardController::ControlAxis::ReleaseUp()
{
    is_signaling_up_ = false;
}
void KeyboardController::ControlAxis::PressDown()
{
    is_signaling_down_ = true;
    most_recent_signal_ = Signal::Down;
}
void KeyboardController::ControlAxis::ReleaseDown()
{
    is_signaling_down_ = false;
}

int KeyboardController::ControlAxis::signal() const
{
    Signal signal;
    if (!is_signaling_up_ && !is_signaling_down_)
    {
        signal = Signal::None;
    }
    else if (is_signaling_up_ && !is_signaling_down_)
    {
        signal = Signal::Up;
    }
    else if (!is_signaling_up_ && is_signaling_down_)
    {
        signal = Signal::Down;
    }
    else // if (is_signaling_up && is_signaling_down)
    {
        signal = most_recent_signal_;
    }
    return static_cast<int>(signal);
}

KeyboardController::KeyboardController()
    : application_(nullptr) {}
KeyboardController::KeyboardController(Application& application)
    : application_(&application) {}

KeyboardController::ControlAxis& 
KeyboardController::horizontal_axis() 
{
    return horizontal_axis_; 
}
KeyboardController::ControlAxis& 
KeyboardController::vertical_axis() 
{
    return vertical_axis_; 
}
KeyboardController::ControlAxis& 
KeyboardController::zoom_axis() 
{
    return zoom_axis_; 
}

void KeyboardController::Process
(
    const int key,
    const int action,
    const int modifiers
)
{
    ProcessAxis
    (
        horizontal_axis_, 
        KEY_RIGHT, KEY_LEFT, 
        key, action
    );
    ProcessAxis
    (
        vertical_axis_, 
        KEY_UP, KEY_DOWN, 
        key, action
    );
    ProcessAxis
    (
        zoom_axis_, 
        KEY_ZOOM_OUT, KEY_ZOOM_IN, 
        key, action
    );

    if (application_ == nullptr) { return; }

    if (action == GLFW_PRESS)
    {
        if (key == KEY_EXIT)
        {
            application_->Close();
        }
        else if (key == KEY_INCREASE_PRECISION)
        {
            application_->IncreasePrecision();
        }
        else if (key == KEY_DECREASE_PRECISION)
        {
            application_->DecreasePrecision();
        }
        else if (key == KEY_TOGGLE_PAUSE)
        {
            application_->TogglePause();
        }
        else if (key == KEY_STEP)
        {
            application_->Step();
        }
        else if (key == KEY_SAVE_SNAPSHOT &&
                 modifiers & GLFW_MOD_CONTROL)
        {
            application_->SaveSnapshot();
        }
        else if (key == KEY_DEBUG)
        {
            application_->PrintDebugInformation();
        }
    }
}
void KeyboardController::ProcessAxis
(
    KeyboardController::ControlAxis& axis,
    const int key_up, const int key_down,
    const int key, const int action
)
{
    if (key == key_up)
    {
        if (action == GLFW_PRESS) { axis.PressUp(); }
        else if (action == GLFW_RELEASE) { axis.ReleaseUp(); }
    }
    else if (key == key_down)
    {
        if (action == GLFW_PRESS) { axis.PressDown(); }
        else if (action == GLFW_RELEASE) { axis.ReleaseDown(); }
    }
}
