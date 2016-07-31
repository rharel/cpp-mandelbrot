/**
 * Fragment shader for smooth mandelbrot set coloring.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#version 440


uniform  sampler2D value_texture;
uniform isampler2D lifetime_texture;
uniform  sampler1D lifetime_color_map_texture;
uniform int max_lifetime;

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

	vec2 z = texture(value_texture, ndc).xy;
	int lifetime = texture(lifetime_texture, ndc).x;
	float N = 2;

	// Compute log_2(log(|z|) / log(N))
	float log_z = log(dot(z, z)) / 2;
    float nu = log(log_z / log(N)) / log(2);
	float smooth_lifetime = lifetime + 1 - nu;

	int color_map_size = textureSize(lifetime_color_map_texture, 0);
	
	int color_a_index, 
	    color_b_index;
	if (smooth_lifetime >= max_lifetime)
	{
		color_a_index = color_b_index = color_map_size;
	}
	else
	{
		color_a_index = int(floor(smooth_lifetime)) % color_map_size;
		color_b_index = int(floor(smooth_lifetime) + 1) % color_map_size;
	}
	
	vec3 color_a = 
		texelFetch(lifetime_color_map_texture, color_a_index, 0).xyz;
	vec3 color_b = 
	    texelFetch(lifetime_color_map_texture, color_b_index, 0).xyz;

	out_color = mix(color_a, color_b, fract(smooth_lifetime));
}
