# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal; 
varying vec4 myvertex; 

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

	float nDotL = dot(normal, direction)  ;         
	vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

	float nDotH = dot(normal, halfvec) ; 
	vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

	vec4 retval = lambert + phong ; 
	return retval ;            
}       

void main (void) 
{       
    if (enablelighting)
	{       
        // They eye is always at (0,0,0) looking down -z axis 
		// Also compute current fragment position and direction to eye 

		const vec3 eyepos = vec3(0,0,0) ; 
		vec4 _mypos = gl_ModelViewMatrix * myvertex ; 
		vec3 mypos = _mypos.xyz / _mypos.w ; // Dehomogenize current location 
		vec3 eyedirn = normalize(eyepos - mypos) ; 

		// Compute normal, needed for shading. 
		vec3 normal = normalize(gl_NormalMatrix * mynormal) ; 

		gl_FragColor = ambient;

		for (int i = 0; i < numused; ++i)
		{
			vec4 pos = lightposn[i];
			vec4 color = lightcolor[i];
			vec4 res;
			vec3 direction;
			vec3 half_;

			if (pos.w != 0)
			{
				// w != 0 : point 
				vec3 position = pos.xyz / pos.w ; 
				direction = normalize (position - mypos) ; // no attenuation 
			}
			else
			{
				// w == 0 : directional
				direction = normalize (pos.xyz) ; 
			}
			
			half_ = normalize (direction + eyedirn) ; 
			res = ComputeLight(direction, color, normal, half_, diffuse, specular, shininess) ;

			gl_FragColor += res;
		}
    }
	else
	{
        gl_FragColor = color; 
    }
}
