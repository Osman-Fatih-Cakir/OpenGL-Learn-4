#version 330 core

in vec4 fColor;

uniform int depth_mode;

out vec4 out_color;

void main()
{
	if (depth_mode == 0)
		out_color = fColor;
	else 
	{
		// Transform non-linear depth values to linear depth values to see the depth values more clear with the color
		float near =  0.1; float far = 100.0; // Same with the perspective function values
		float depth = gl_FragCoord.z;
		float z = depth * 2.0 - 1.0;
		float linear_depth = (2.0 * near * far) / (far + near - z * (far - near));
		linear_depth /= far;

		out_color = vec4(vec3(linear_depth), 1.0);
	}
}