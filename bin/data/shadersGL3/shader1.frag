#version 150

// shader from https://www.shadertoy.com/view/msyGRm by guil

out vec4 outputColor;
uniform float time;

float fire( vec2 p ) {
    
    float f=1., s=1.6;
    p.y -= time*4.;
    vec2 r = vec2(0);
    for(int i = 0;i<15;i++){
        r += sin( p*f +time*2.5)/f;
        p *= mat2(.8,.6,-.8,.6);
        p += r*.4;
        f *= s;
    }
    return length(r);
}

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    float windowWidth = 1024.0;
    float windowHeight = 768.0;
    vec2 resolution = vec2(windowWidth,windowHeight);
    
    /*
    float r = gl_FragCoord.x / windowWidth;
    float g = gl_FragCoord.y / windowHeight;
    float b = 1.0;
    float a = 1.0;
    gl_FragColor = vec4(r, g, b, a);
    */
    
    
    vec2 p = 8.*gl_FragCoord.xy/resolution-9.;
    p.x *= resolution.x/resolution.y;

    float f = fire(p);
    float r = (fire(p+.1)-.5*f)*exp(-p.y*.2-1.);
    
    outputColor = r*vec4(f*f, f, 1, 1);
}
