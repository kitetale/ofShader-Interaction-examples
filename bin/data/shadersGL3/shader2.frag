#version 150

// shader from https://www.shadertoy.com/view/XlX3DM by guil

out vec4 outputColor;
uniform float time;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    float windowWidth = 1024.0;
    float windowHeight = 768.0;
    vec3 resolution = vec3(windowWidth,windowHeight,1.);
    
    float k=0.;
    vec3 d =  (vec3(gl_FragCoord.xy,1.0))/resolution-.5, o = d, c=k*d, p;
    
    for( int i=0; i<99; i++ ){
        
        p = o+sin(time*.1);
        for (int j = 0; j < 10; j++)
        
            p = abs(p.zyx-.4) -.7,k += exp(-6. * abs(dot(p,o)));
        
        
        k/=3.;
        o += d *.05*k;
        c = .97*c + .1*k*vec3(k*k,k,1);
    }
    c =  .4 *log(1.+c);
    outputColor = vec4(c,1.);
}
