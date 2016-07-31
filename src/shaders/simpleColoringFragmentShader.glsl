/**
 * Fragment shader for mandelbrot set coloring.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#version 440


uniform isampler2D lifetime_texture;
uniform  sampler1D lifetime_color_map_texture;

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
	vec2 ndc = ConvertToNDC(in_clip_space_position);
	int lifetime = texture(lifetime_texture, ndc).x;
	int color_index = lifetime % textureSize(lifetime_color_map_texture, 0);
	out_color = texelFetch(lifetime_color_map_texture, color_index, 0).xyz;
}
