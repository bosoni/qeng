$input v_texcoord0, v_color0, v_normal

#include "../common/common.sh"

SAMPLER2D(s_texColor, 0);

void main()
{
	vec4 color = toLinear(texture2D(s_texColor, v_texcoord0));
	if (color.a < 0.1) discard;

	gl_FragColor = toGamma(color * v_color0);
}
