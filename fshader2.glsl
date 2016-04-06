#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;

varying vec2 v_texcoord;

void main()
{
    // Set fragment color from texture
    vec4 tmp = texture2D(texture, v_texcoord, 0.1);
    tmp.x -= 0.05;
    tmp.z += 0.25;
    gl_FragColor = tmp;
}
