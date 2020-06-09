$input a_position, a_normal, a_texcoord0
$output v_wpos, v_view, v_normal, v_texcoord0, v_color0

#include "../common/common.sh"

uniform vec4 u_color;

void main()
{
	vec3 wpos = mul(u_model[0], vec4(a_position, 1.0) ).xyz;
	v_wpos = wpos;

	gl_Position = mul(u_viewProj, vec4(wpos, 1.0) );

	v_normal = a_normal;
	v_texcoord0 = a_texcoord0;
	v_color0 = u_color;
	
	vec3 weyepos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;	
	v_view = weyepos - wpos;
}
