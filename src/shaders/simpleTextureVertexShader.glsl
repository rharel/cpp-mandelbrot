/**
 * Vertex shader for single texture drawing.
 *
 * @author Raoul Harel
 * @url github.com/rharel/cpp-mandelbrot
 */


#version 440


layout(location = 0) in  vec3 in_world_position;
layout(location = 1) out vec2 out_clip_space_position;

void main()
{
	out_clip_space_position = in_world_position.xy;
	gl_Position = vec4(in_world_position.xy, 0, 1);
}
