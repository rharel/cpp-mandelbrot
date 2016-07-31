/**
 * Fragment shader for single texture drawing.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#version 440


uniform sampler2D image_texture;
uniform dvec2 relative_position_offset = dvec2(0, 0);
uniform double relative_size_factor = 1.0;

layout(location = 1) in  vec2 in_clip_space_position;
layout(location = 0) out vec3 out_color;

/**
 * Converts a position in clip-space to normalized device coordinates (NDC).
 *
 * @param position Clip-space coordinates.
 *
 * @returns Corresponding NDC.
 */
vec2 ConvertToNDC(const vec2 position)
{
	return 0.5 * (position + 1);
}

void main()
{
	dvec2 uv = ConvertToNDC(in_clip_space_position);
	uv -= 0.5;
	uv *= relative_size_factor;
	uv += 0.5;
	uv += relative_position_offset;
	
	out_color = texture(image_texture, vec2(uv)).xyz;
}
