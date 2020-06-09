$input v_wpos, v_view, v_normal, v_texcoord0, v_color0

#include "../common/common.sh"

SAMPLER2D(s_texColor,  0);

uniform vec4 u_lightPosRadius[4];
uniform vec4 u_lightRgbInnerR[4];

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	//vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	vec3 reflected = 2.0*ndotl*_normal - _lightDir;
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

vec4 lit(float _ndotl, float _rdotv, float _m)
{
	float diff = max(0.0, _ndotl);
	float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
	return vec4(1.0, diff, spec, 1.0);
}

vec4 powRgba(vec4 _rgba, float _pow)
{
	vec4 result;
	result.xyz = pow(_rgba.xyz, vec3_splat(_pow) );
	result.w = _rgba.w;
	return result;
}

vec3 calcLight(int _idx, vec3 _wpos, vec3 _normal, vec3 _view)
{
	vec3 lp = u_lightPosRadius[_idx].xyz - _wpos;
	float attn = 1.0 - smoothstep(u_lightRgbInnerR[_idx].w, 1.0, length(lp) / u_lightPosRadius[_idx].w);
	vec3 lightDir = normalize(lp); //mul( normalize(lp), _tbn );
	vec2 bln = blinn(lightDir, _normal, _view);
	vec4 lc = lit(bln.x, bln.y, 1.0);
	vec3 rgb = u_lightRgbInnerR[_idx].xyz * saturate(lc.y) * attn;
	return rgb;
}

void main()
{
	vec3 normal = v_normal;
	vec3 view = normalize(v_view);

	vec3 lightColor;
	lightColor =  calcLight(0, v_wpos, normal, view);
	lightColor += calcLight(1, v_wpos, normal, view);
	lightColor += calcLight(2, v_wpos, normal, view);
	lightColor += calcLight(3, v_wpos, normal, view);

	vec4 color = toLinear(texture2D(s_texColor, v_texcoord0) );

	gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
	gl_FragColor.w = 1.0;
	gl_FragColor = toGamma(gl_FragColor * v_color0);
}
