# ofShader Interaction Examples
3.11.2023 ~

ofShader + interaction mix and match example prototypes in openFrameworks for my Capstone project!

## Some notes for myself ##
* **shader** : .frag + .vert files
* **fragment shader** : gradients and pixel colors between vertices by math
* **vertex shader** : physical displacement of vertices 
* **Fbo** : Framebuffer objects; think of it as a box that can carry shader/img/video textures
* shadersGL3 for macOS, shadersES2 for raspberry Pi
* `gl_Position` : .vert position output 
* `gl_FragCoord` : .frag fragment coord input
* `gl_FragColor` : [#version 120] .frag color output
* `out vec4 outputColor;` : [#version 150] .frag color output
* `uniform float time;` : time input; input time value in ofApp.cpp with `shader.setUniform1f("time", ofGetElapsedTimef());`
* `uniform sampler2DRect tex0;` : texture input; input tex value in ofApp.cpp with `shader.setUniformTexture("tex0", tex0Fbo.getTexture(), 1 );`
* `maskTex.setTextureWrap(GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER);` : define no texture repetition


## Interaction Examples ##
(use numkeys to switch between examples)

0. Shader 1 + Shader 2 with alpha brush draw Fbo 

![example 1: two shaders with alpha brush draw with cursor example](./documentation/example1.gif)

Modified fragment shader codes [1](https://www.shadertoy.com/view/msyGRm), [2](https://www.shadertoy.com/view/XlX3DM) by guil and ofShader example #7 to fit with openFrameworks setting.

1. Shader 1 + Shader 2 with alpha shadow mov file

![example 2: two shaders with alpha silhouette shadow of people example](./documentation/example2.gif)

Modified fragment shader codes by [wutzhuhn](https://www.shadertoy.com/view/Ddy3zD), [guil](https://www.shadertoy.com/view/XlX3DM), and ofShader example #7 to fit with openFrameworks setting. <br/> Shadow mov clips from [golanlevin](https://github.com/golanlevin/lectures/tree/master/lecture_shadow/images/clips).


## ffmpeg gif transition ##
With palette (reduces box noise)
> `ffmpeg -i example1.mov -vf scale=flags=lanczos,palettegen example1.palette.png -y` <br/>

> `ffmpeg -i example1.mov -i example1.palette.png -filter_complex “fps=12,scale=360:-1:flags=lanczos[x],[x][1:v]paletteuse” example1.gif -y`

Without palette
> `ffmpeg -i example1.mov -vf "fps=12, scale=360:-1" example1.gif`


## Reference Links ##
* [ofBook Shader Introduction](https://openframeworks.cc/ofBook/chapters/shaders.html)
* [ofShader documentation](https://openframeworks.cc/documentation/gl/ofShader/)
* [shader toy](https://www.shadertoy.com/)
