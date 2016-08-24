#pragma once
#include <string>

static const std::string blurFragShader = \
"uniform sampler2D texture;" \
"uniform float radius;" \
"uniform vec2 dimensions;" \
"void main()" \
"{" \
"	vec2 offx = vec2(1/dimensions.x, 0.0);" \
"	vec2 offy = vec2(0.0, 1/dimensions.y);" \
"	" \
"   vec4 pixel; " \
"   for(float i = -radius; i<radius+0.5; i+=1) {" \
"		for(float j = -radius; j<radius+0.5; j+=1) {" \
"           pixel = pixel + texture2D(texture, gl_TexCoord[0].xy + offx*i + offy*j); " \
"       }" \
"   }" \
"	" \
"	gl_FragColor = gl_Color * (pixel / ((radius*2+1)*(radius*2+1)));" \
"}";