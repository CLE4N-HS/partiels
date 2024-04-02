uniform sampler2D u_texture;
uniform float u_timer;

 float Lerp(float a , float b , float t)
 {
    if(t <= 0) return a;
    if(t >=  1) return b;
    return (b-a)*t+a;
 }

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec3 pixels = texture2D(u_texture, uv).rgb;

	vec3 color = pixels * vec3(0.01f);

	vec2 middlePos = vec2(0.5, 0.5);

	float dist = distance(middlePos, uv);

	float previous = color.r;

	color.r += dist * dist * 50;
	
	if (dist > 0.15) {
		color.r = previous + 1 - dist * dist * 7;

		//if (color.r > 0.75) {
		//	color.r = previous + 1 - dist * dist * 7;
		//}
	}

	//color.r += distance(middlePos, uv) * 5;

	if(u_timer < 0.5) {
		gl_FragColor = vec4(vec3(color), texture2D(u_texture, uv).a * Lerp(0.,1.,u_timer * 2));
	}
	else {
		gl_FragColor = vec4(vec3(color), texture2D(u_texture, uv).a * Lerp(1.,0.,(u_timer -0.5) * 2));
	}
}