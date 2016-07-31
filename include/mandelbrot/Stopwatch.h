/**
 * Elapsed time measurement.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#pragma once

#include <chrono>


namespace mandelbrot
{
    /**
     * This class is used to measure elapsed time.
     */
    class Stopwatch
    {
        public:
        typedef std::chrono::high_resolution_clock Clock;
        typedef Clock::time_point TimePoint;
        typedef Clock::duration Duration;

        /**
         * Mark current time as starting point of measurement.
         */
        void Start();
        /**
         * Mark current time as end point of measurement.
         */
        void Stop();

        /**
         * Gets elapsed nanoseconds.
         */
        const Duration& nanoseconds() const;

        private:
        TimePoint begin_;
        Duration elapsed_nanoseconds_;
    };
}
