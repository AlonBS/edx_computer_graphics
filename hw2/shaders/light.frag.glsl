# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal; 
in vec4 myvertex; 

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess. 
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess;

vec4 calculatePositionalLight(const vec3 fragPos,
							  const vec3 lightPos,
							  const vec4 lightColor,
							  const vec3 normal,
							  const vec4 objDiffuse,
							  const vec4 objSpecular,
							  const float objShininess)
{
	// Diffuse element
	vec3 lightDir = normalize(lightPos - fragPos);
	float NdotL = dot(normal,lightDir);
	vec4 diff = lightColor * objDiffuse * max(NdotL ,0.0);
	
	
	// Specular
	vec3 eyePos = vec3(0.0f, 0.0f, 0.0f); // This is our convention
	vec3 eyeDir = normalize(eyePos - fragPos);
	vec3 halfAngle = normalize(eyeDir + lightDir);
	float NdotH = dot(normal, halfAngle);
	vec4 spec = lightColor * objSpecular * pow(max(dot(normal, halfAngle), 0.0), shininess);
		
	return diff + spec;
}


vec4 ComputeLight (const in vec3 direction,
 				   const in vec4 lightcolor, 
 				   const in vec3 normal, 
 				   const in vec3 halfvec, 
 				   const in vec4 mydiffuse, 
 				   const in vec4 myspecular, 
 				   const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

        vec4 retval = lambert + phong ; 
        return retval ;            
}       



 

void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor;
        
        const vec3 eyepos = vec3(0,0,0); // This is the convension               
        vec3 fragPos = myvertex.xyz / myvertex.w;
        vec3 normal = normalize(mat3(transpose(inverse(modelview))) * mynormal);
        vec3 eyeDir = normalize(eyepos - fragPos) ;
        vec3 lightDir;
        
        finalcolor = ambient + emission;
                     
        for (int i = 0; i < numused; ++i) {

			// Point light Light        
        	if (lightposn[i].w  > 0.0f) {
        		lightDir = normalize((lightposn[i].xyz / lightposn[i].w) - fragPos);
        	}
        	
        	// Directional light
        	else {
        		lightDir = normalize(lightposn[i].xyz);
        	}
        	
        	vec3 halfvec = normalize(lightDir + eyeDir);
        	finalcolor += ComputeLight(lightDir, lightcolor[i], normal, halfvec, diffuse, specular, shininess);
                
        }

		fragColor = finalcolor;
        //fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); 
    } else {
        fragColor = vec4(color, 1.0f); 
    }
}
