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

## Interaction Examples ##
1. Shader 1 + Shader 2 with alpha brush draw Fbo 
![example1: two shaders with alpha brush draw with cursor example](example1.gif)
