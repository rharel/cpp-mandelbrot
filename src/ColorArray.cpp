#include <mandelbrot/ColorArray.h>

#include <soil/soil.h>


using namespace mandelbrot;


ColorArray ColorArray::FromPath(const char* path)
{
    int width, height, channels;
    unsigned char* data = SOIL_load_image
    (
        path, 
        &width, &height, &channels, 
        SOIL_LOAD_AUTO
    );
    
    ColorArray color_map = 
        data == 0  ? 
        ColorArray() :
        ColorArray(width * height, 3, data);
    
    SOIL_free_image_data(data);

    return color_map;
}

ColorArray::ColorArray()
    : ColorArray(0, 0, nullptr) {}
ColorArray::ColorArray
(
    const unsigned int size,
    const unsigned int channel_count,
    const unsigned char* data
)
    : size_(size),
      channel_count_(channel_count),
      data_(&data[0], &data[channel_count * size]) {}

bool ColorArray::is_empty() const
{
    return data_.size() == 0;
}
unsigned int ColorArray::size() const
{
    return size_;
}
unsigned int ColorArray::channel_count() const
{
    return channel_count_;
}
const unsigned char* ColorArray::data() const
{
    return data_.data();
}
