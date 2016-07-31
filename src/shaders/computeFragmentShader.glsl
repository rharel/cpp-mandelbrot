/**
 * Fragment shader for mandelbrot set computation.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#version 440


uniform dvec2 viewport_bottom_left = dvec2(-2, -1.5);
uniform double viewport_size = 3;

uniform  sampler2D value_texture;
uniform isampler2D lifetime_texture;

uniform int dt = 1;

layout(location = 1) in  vec2 in_clip_space_position;
layout(location = 0) out vec2 out_value;
layout(location = 1) out int  out_lifetime;

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
/**
 * Converts a position in normalized device coordinates (NDC) to its position
 * in the complex plane, based on viewport uniforms.
 *
 * @param position Normalized device coordinates.
 *
 * @returns Corresponding position in the complex plane.
 */
dvec2 ConvertToComplex(const vec2 position)
{
	return viewport_bottom_left + position * viewport_size;
}

/**
 * Squares a complex number.
 */
dvec2 Square(const dvec2 z)
{
	return dvec2
	(
		z.x * z.x - z.y * z.y,
		2 * z.x * z.y
	);
}
/**
 * Executes one iteration of the mandelbrot function.
 */
dvec2 ComputeMandelbrot(const dvec2 z, const dvec2 c)
{
	return Square(z) + c;
}
/**
 * Executes the mandelbrot function for a maximum number of iterations.
 *
 * @param z0  Starting value
 * @param c   Offset
 * @param dt  Duration in iterations
 * @param[out] lifetime Lifetime duration in iterations.
 */
dvec2 RepeatMandelbrot
(
	const dvec2 z0, const dvec2 c, 
	const int dt, 
	out int lifetime)
{
	dvec2 z = z0;
	lifetime = 0;
	while (dot(z, z) < 4 && lifetime < dt)
	{
		z = ComputeMandelbrot(z, c);
		++lifetime;
	}
	return z;
}

void main()
{
	vec2 ndc = ConvertToNDC(in_clip_space_position);
	
	dvec2 z = texture(value_texture, ndc).xy;
	dvec2 c = ConvertToComplex(ndc);
	
	int lifetime_offset;
	out_value = vec2(RepeatMandelbrot(z, c, dt, lifetime_offset));

	int lifetime = texture(lifetime_texture, ndc).x;
	out_lifetime = lifetime + lifetime_offset;
}
