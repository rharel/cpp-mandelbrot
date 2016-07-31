/**
 * Application singleton.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <memory>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <mandelbrot/Camera.h>
#include <mandelbrot/ColorArray.h>
#include <mandelbrot/KeyboardController.h>
#include <mandelbrot/Renderer.h>
#include <mandelbrot/Stopwatch.h>
#include <mandelbrot/Vector2.h>


namespace mandelbrot
{
    /**
     * This class is the singleton representing the application, 
     * and is responsible for the linkage between the graphical user
     * interface and the rendering functionalities.
     */
    class Application
    {
        public:
        /**
         * Initializes the singleton instance.
         * Must be called before attempting to access the object
         * through instance().
         */
        static void Initialize
        (
            unsigned int window_width,
            unsigned int window_height
        );
        /**
         * Gets the singleton instance.
         */
        static Application& instance();

        static const char* WINDOW_TITLE;

        /**
         * Initializes OpenGL and launches the the graphical user interface.
         */
        bool Launch();
        /**
         * Exits the application.
         */
        void Close();

        /**
         * Pauses continuous rendering.
         */
        void Pause();
        /**
         * Resumes continuous rendering.
         */
        void Play();
        /**
         * Toggles between paused and unpaused states.
         */
        void TogglePause();
        /**
         * Produces a single rendering based on current camera position.
         */
        void Step();
        
        /**
         * Increases rendering precision.
         * Viable for deep zooms.
         */
        void IncreasePrecision();
        /**
         * Decreases rendering precision.
         * Viable for shallow zooms.
         */
        void DecreasePrecision();

        /**
         * Save rendering and camera state as files.
         */
        bool SaveSnapshot(const char* name = nullptr);
        /**
         * Save rendering as an image file.
         */
        bool SaveImage(const char* path = nullptr);
        /**
         * Save camera state as a text file.
         */
        bool SaveState(const char* path = nullptr);

        /**
         * Gets camera (read only).
         */
        const Camera& camera() const;
        /**
         * Gets camera.
         */
        Camera& camera();

        /**
         * Gets renderer (read only).
         */
        const Renderer& renderer() const;
        /**
         * Gets renderer.
         */
        Renderer& renderer();
        
        /**
         * Prints debug information to std::cout.
         */
        void PrintDebugInformation() const;

        private:
        static void WindowSizeCallback
        (
            GLFWwindow* window,
            int width, int height
        );
        static void KeyCallback
        (
            GLFWwindow* window,
            int key, 
            int scan_code, 
            int action,
            int modifiers
        );

        static std::unique_ptr<Application> instance_;

        Application
        (
            unsigned int window_width,
            unsigned int window_height
        );

        bool Initialize();
        bool InitializeWindow();
        bool InitializeOpenGL();
        bool InitializeRenderer();
        
        void Dispose();

        void EnterMainLoop();
        void HandleEvents();
        void RenderFrame();

        bool UpdateCamera();
        void UpdateViewport();

        void WindowSizeCallback(int width, int height);
        void KeyCallback
        (
            int key, 
            int scan_code, 
            int action,
            int modifiers
        );

        Vector2u window_size_;
        GLFWwindow* window_;

        KeyboardController keyboard_controller_;
      
        bool is_paused_ = false;
        bool is_stepping_ = false;

        Renderer renderer_;
        Camera camera_;

        Stopwatch stopwatch_;

        unsigned int session_saved_snapshots_count_ = 0;
    };
}
