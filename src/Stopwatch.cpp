#include <mandelbrot/Stopwatch.h>


using namespace mandelbrot;


void Stopwatch::Start()
{
    begin_ = Clock::now();
}
void Stopwatch::Stop()
{
    const auto end = Clock::now();
    elapsed_nanoseconds_ = end - begin_;
}

const Stopwatch::Duration& Stopwatch::nanoseconds() const
{
    return elapsed_nanoseconds_;
}
