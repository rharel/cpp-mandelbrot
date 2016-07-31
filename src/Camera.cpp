#include <mandelbrot/Camera.h>


using namespace mandelbrot;


Camera::Camera()
    : Camera(1.0, 1.0) {}
Camera::Camera
(
    const double movement_speed,
    const double zoom_speed
)
    : movement_speed_(movement_speed),
      zoom_speed_(zoom_speed) {}

void Camera::Move(const Vector2d& direction, const double t)
{
    position_ += t * direction * movement_speed_ * zoom_factor_;
}
void Camera::Zoom(const int direction, const double t)
{
    zoom_factor_ *= 1 + t * direction * zoom_speed_;
}

const Vector2d& Camera::position() const
{
    return position_;
}
void Camera::set_position(const Vector2d& value)
{
    position_ = value;
}

double Camera::zoom_factor() const
{
    return zoom_factor_;
}
void Camera::set_zoom_factor(const double value)
{
    zoom_factor_ = value;
}

double Camera::movement_speed() const
{
    return movement_speed_;
}
void Camera::set_movement_speed(const double value)
{
    movement_speed_ = value;
}

double Camera::zoom_speed() const
{
    return zoom_speed_;
}
void Camera::set_zoom_speed(const double value)
{
    zoom_speed_ = value;
}
