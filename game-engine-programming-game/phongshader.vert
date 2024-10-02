attribute vec3 position;
attribute vec2 uv;

varying vec2 outUv;

void main(void)
{
	gl_Position = vec4(position, 1.0);
	outUv = uv;
}
