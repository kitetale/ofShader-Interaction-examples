#version 150
// shader from https://www.shadertoy.com/view/Ddy3zD by wutzhuhn

// this comes from the vertex shader
in vec2 texCoordVarying;

// this is the output of the fragment shader
out vec4 outputColor;
uniform float time;

float windowWidth = 1024.0;
float windowHeight = 768.0;
vec2 resolution = vec2(windowWidth,windowHeight);

const vec3 red    = vec3(0.816, 0.325, 0.227);
const vec3 green  = vec3(0.584, 0.639, 0.38);
const vec3 blue   = vec3(0.498, 0.588, 0.49);
const vec3 yellow = vec3(0.843, 0.725, 0.353);
const vec3 white  = vec3(0.91,  0.804, 0.596);
const vec3 black  = vec3(0.125, 0.098, 0.078);

// fewer belts = more fps. Max is 32.
#define NUM_BELTS 32

float rand(float x) {
  return fract(sin(x) * 43758.5453);
}

// Helper function that returns 1 if f is between
// lo and hi, and 0 otherwise. The transition between
// 0 and 1 is smoothed to about 2 pixels.
float high_between(float f, float lo, float hi) {
  float d = 2.0 / resolution.x;
  float rad = (hi - lo) / 2.0;
  float mid = (lo + hi) / 2.0;
  return smoothstep(-d, d, rad - abs(f - mid));
}

float pattern(int i, vec2 p) {
  i = i % 4;
  float s = (p.x - p.y) / sqrt(2.0);
  if (0 == i) {
    return high_between(mod(s, 0.03), 0.2 * 0.03, 0.55 * 0.03);
  }
  if (1 == i) {
    float m = mod(s, 0.03);
    return high_between(m, 0.1 * 0.03, 0.3 * 0.03)
         + high_between(m, 0.5 * 0.03, 0.8 * 0.03);
  }
  if (2 == i) {
    return high_between(mod(s, 0.01), 0.2 * 0.01, 0.65 * 0.01);
  }
  if (3 == i) {
    mat2 rot = sqrt(2.0) / 2.0 * mat2( 1.0, -1.0,
                                       1.0,  1.0);
    vec2 dot_center = transpose(rot) * round(rot * p * 100.0) / 100.0;
    float dot_radius = mix(rand(dot_center.x + dot_center.y), 1.0, 0.8) * 0.003;
    return high_between(length(dot_center - p), dot_radius, 100.0);
  }
  return 0.0;
}

const vec3 fg_colors[8] = vec3[](blue,    red,  green, green, yellow,  blue,   red, green);
const vec3 bg_colors[8] = vec3[]( red, yellow, yellow,  blue,  white, white, white, white);

float cbrt(float f) {
  return sign(f) * pow(abs(f), 1.0 / 3.0);
}
float bezier(vec2 a, vec2 b, vec2 c, vec2 p) {
    vec2 ny = normalize((a + c) * 0.5 - b);
    vec2 nx = vec2(ny.y, -ny.x);
    float slope_a = (dot(b - a, ny) / dot(b - a, nx)) / 2.0;
    float slope_c = (dot(b - c, ny) / dot(b - c, nx)) / 2.0;
    float scale = (slope_c - slope_a) / dot(c - a, nx);
    vec2 origin = a - nx * (slope_a / scale) - ny * (slope_a * slope_a / scale);
    
    float px = dot(p - origin, nx) * scale;
    float py = dot(p - origin, ny) * scale;

    float min_x = min(slope_a, slope_c);
    float max_x = max(slope_a, slope_c);

    float e = ((1.5 - py) * py - 0.75) * py + 0.125;
    float f = 0.0625 * px * px + e / 27.0;
    if (f >= 0.0) {
        float g = sqrt(f);
        float x = cbrt(0.25 * px + g) + cbrt(0.25 * px - g);
        float cx = clamp(x, min_x, max_x);
        return length(vec2(cx - px, cx * cx - py)) / scale;
    }
    float v = acos(sqrt(-27.0 / e) * px * 0.25) / 3.0;
    float m = cos(v);
    float n = sin(v) * sqrt(3.0);
    float o = sqrt((py - 0.5) / 3.0);

    float cx1 = clamp( (m + m) * o, min_x, max_x);
    float cx2 = clamp(-(m + n) * o, min_x, max_x);
    vec2 d1 = vec2(cx1 - px, cx1 * cx1 - py);
    vec2 d2 = vec2(cx2 - px, cx2 * cx2 - py);
    return sqrt(min(dot(d1, d1), dot(d2, d2))) / scale;
}
float segment(vec2 a, vec2 b, vec2 p) {
    vec2 ba = b - a;
    vec2 pa = p - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - h * ba);
}


void main()
{
    // (0,0) at the center, -1 left, 1 right, -1 bottom, 1 top.
    vec2  p = (2.0 * gl_FragCoord.xy - resolution.xy) / resolution.x;

    // add two levels of noise to the pixel position:
    // 1. some coarse noise to make the likes look more hand-drawn.
    p += vec2(sin(p.x * 64.0 + p.y * 128.0) * 0.000625,
              sin(p.y * 64.0 + p.x *  32.0) * 0.000625);
    // 2. some fine noise to make the edges look more like ink on paper.
    p += vec2(rand(p.x * 31.0 + p.y * 87.0) * 0.001,
              rand(p.x * 11.0 + p.y * 67.0) * 0.001);

    float outline = 0.0;
    float id = -1.0;
    for (int i = 0; i < NUM_BELTS; ++i) {
      float t = time + float(i * 16) + 1024.0;
      vec2 p0 =      vec2(-1.5,              sin(t * 0.02));
      vec2 p1 =      vec2( sin(t*0.1) * 0.1, sin(t * 0.07) * 0.7);
      vec2 p2 =      vec2( 1.5,              sin(t * 0.03));
      vec2 c0 = p1 + vec2(-0.5,              sin(t * 0.13) * 0.5);
      vec2 c1 = 2.0 * p1 - c0;
      float dist = min(
          bezier(
            p.x < p1.x ? p0 : p1,
            p.x < p1.x ? c0 : c1,
            p.x < p1.x ? p1 : p2,
            p),
          segment(
            p1,
            p.x < p1.x ? c1 : c0,
            p));

  #if 1
      vec2 p3 = p0 - vec2(1.0 - sin(t * 0.025), 1.0 - sin(t * 0.027)) * 0.05;
      vec2 p4 = p1 + vec2(      sin(t * 0.014),       sin(t * 0.032)) * 0.05;
      vec2 p5 = p2 + vec2(1.0 - sin(t * 0.014), 1.0 - sin(t * 0.032)) * 0.05;
      vec2 c2 = p4 + vec2(-1.0,                       sin(t * 0.13))  * 0.5;
      vec2 c3 = 2.0 * p4 - c0;
      float dist2 = min(
          bezier(
            p.x < p4.x ? p3 : p4,
            p.x < p4.x ? c2 : c3,
            p.x < p4.x ? p4 : p5,
            p),
          segment(
            p4,
            p.x < p4.x ? c3 : c2,
            p));
      dist = min(dist, dist2);
  #endif
      dist *= sin(p.x * 10.0 + sin(p.y)) * 0.2 + 1.0;

      float fill   = high_between(dist, -1.0,   0.025);
      float border = high_between(dist,  0.022, 0.028);
      id      = mix(id, float(i), fill);
      outline = mix(outline, border, fill);
    }
    vec3 fg = 0.0 <= id ? fg_colors[int(id)/4] : white;
    vec3 bg = 0.0 <= id ? bg_colors[int(id)/4] : white;
    vec3 color = mix(mix(fg, bg, pattern(int(id), p)), black, outline);

    // Some noise to make it look more paper-y
    color *= 0.95 + rand(p.x + p.y) * 0.1;
    outputColor = vec4(color, 1.0);
}

