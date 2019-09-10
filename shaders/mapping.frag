#version 120
#extension GL_ARB_texture_rectangle: enable

//Shader for mapping quads, see details in ofxKuVideoMapping.h

uniform sampler2DRect tex0;
uniform vec2 V00, V10, V01, V11;
//uniform vec2 screen_res;
uniform vec2 image_res;
uniform float alpha;


varying vec2 texCoordVarying;

//--------------------------------------------------------------
// inverse bilinear interpolation by Inigo Quilez, 
// https://iquilezles.org/www/articles/ibilinear/ibilinear.htm
// https://www.shadertoy.com/view/lsBSDm
// given a point p and a quad defined by four points {a,b,c,d}, return the bilinear
// coordinates of p in the quad. Returns (-1,-1) if the point is outside of the quad.

float cross2d( in vec2 a, in vec2 b ) { return a.x*b.y - a.y*b.x; }

vec2 invBilinear( in vec2 p, in vec2 a, in vec2 b, in vec2 c, in vec2 d )
{
    vec2 res = vec2(-1.0);

    vec2 e = b-a;
    vec2 f = d-a;
    vec2 g = a-b+c-d;
    vec2 h = p-a;
        
    float k2 = cross2d( g, f );
    float k1 = cross2d( e, f ) + cross2d( h, g );
    float k0 = cross2d( h, e );
    
    // if edges are parallel, this is a linear equation. Do not this test here though, do
    // it in the user code
    if( abs(k2)<0.001 )
    {
        float v = -k0/k1;
        float u  = (h.x*k1+f.x*k0) / (e.x*k1-g.x*k0);
        if( v>0.0 && v<1.0 && u>0.0 && u<1.0 )  res = vec2( u, v );
    }
	else
    {
        // otherwise, it's a quadratic
        float w = k1*k1 - 4.0*k0*k2;
        if( w<0.0 ) return vec2(-1.0);
        w = sqrt( w );

        float ik2 = 0.5/k2;
        float v = (-k1 - w)*ik2; if( v<0.0 || v>1.0 ) v = (-k1 + w)*ik2;
        float u = (h.x - f.x*v)/(e.x + g.x*v);
        if( u<0.0 || u>1.0 || v<0.0 || v>1.0 ) return vec2(-1.0);
        res = vec2( u, v );
    }
    
    return res;
}

//--------------------------------------------------------------
void main() {
	//gl_FragCoord - see https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
    vec2 pos = texCoordVarying / image_res;
	//pos.y = 1 - pos.y;
    pos = invBilinear(pos, V00, V10, V11, V01) * image_res;  
    if(pos.x>=0 && pos.x < image_res.x && pos.y >= 0 && pos.y <= image_res.y) {
		gl_FragColor = texture2DRect(tex0, pos) * vec4(1,1,1,alpha);
    }
	else {
		gl_FragColor = vec4(0,0,0,1) * vec4(1,1,1,alpha);
	}
}
