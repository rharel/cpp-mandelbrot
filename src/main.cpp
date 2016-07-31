/**
 * Application entry point.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#include <string>

#include <tclap/CmdLine.h>

#include <mandelbrot/Application.h>
#include <mandelbrot/ColorArray.h>
#include <mandelbrot/Vector2.h>


using namespace mandelbrot;


typedef int Status;
const Status SUCCESS =  0;
const Status FAILURE = -1;

Status main(const int argc, char** argv)
{
    static const unsigned int WINDOW_WIDTH = 720;
    static const unsigned int WINDOW_HEIGHT = 720;
    
    static const unsigned int DEFAULT_RESOLUTION = 512; 
    
    static const double CAMERA_MOVEMENT_SPEED = 0.5;
    static const double CAMERA_ZOOM_SPEED = 1.0;
    
    static const Vector2d DEFAULT_VIEWPORT_CENTER = Vector2d(-0.5, 0);
    static const double DEFAULT_VIEWPORT_SIZE = 3;

    try
    {
        TCLAP::CmdLine command_line
        (
            "Mandelbrot explorer helps you navigate and extract nifty "
            "images from the mandelbrot set.",
            ' ',
            "1.0"
        );
        TCLAP::ValueArg<unsigned int> resolution_arg
        (
            "r", "resolution", "Rendering resolution",
            false, DEFAULT_RESOLUTION, "positive integer"
        );
        TCLAP::ValueArg<std::string> color_map_path_arg
        (
            "c", "color-map-path", 
            "Path to color map image (accepts RGB .bmp, .png, .jpg)",
            false, "", "string"
        );
        TCLAP::ValueArg<double> camera_x_arg
        (
            "x", "viewport-x", "Viewport center along the real axis",
            false, DEFAULT_VIEWPORT_CENTER.x, "double"
        );
        TCLAP::ValueArg<double> camera_y_arg
        (
            "y", "viewport-y", "Viewport center along the imaginary axis",
            false, DEFAULT_VIEWPORT_CENTER.y, "double"
        );
        TCLAP::ValueArg<double> camera_zoom_arg
        (
            "z", "viewport-size", "Viewport size",
            false, DEFAULT_VIEWPORT_SIZE, "positive double"
        );

        command_line.add(resolution_arg);
        command_line.add(color_map_path_arg);
        command_line.add(camera_x_arg);
        command_line.add(camera_y_arg);
        command_line.add(camera_zoom_arg);

        command_line.parse(argc, argv);

        Application::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
        Application& application = Application::instance();

        ColorArray color_map = 
            ColorArray::FromPath(color_map_path_arg.getValue().c_str());
        color_map = 
            color_map.is_empty() ?
            application.renderer().color_map() :
            color_map;
        
        const Vector2d camera_position
        (
            camera_x_arg.getValue(),
            camera_y_arg.getValue()
        );
        application.camera().set_position(camera_position);
        application.camera().set_zoom_factor(camera_zoom_arg.getValue());
        application.camera().set_movement_speed(CAMERA_MOVEMENT_SPEED);
        application.camera().set_zoom_speed(CAMERA_ZOOM_SPEED);
    
        application.renderer().set_resolution(resolution_arg.getValue());
        application.renderer().set_color_map(color_map);
        application.renderer().set_iterations_per_step(color_map.size());

        return application.Launch() ? SUCCESS : FAILURE;
    }
    catch (TCLAP::ArgException& exception)
    {
        std::cerr << "Error: " << exception.error()
                  << "in argument" << exception.argId()
                  << std::endl;
    }
    return FAILURE;
}
