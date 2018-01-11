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


vec4 
CalcLight (const vec3 light_dir, const vec3 normal, 
 		   const vec3 half_vec, const vec4 light_color, 
 		   const vec4 obj_diffuse, const vec4 obj_specular, 
 			const float obj_shininess) 
{
        float NL = dot(normal, light_dir)  ;         
        vec4 diffuse = light_color * obj_diffuse * max (NL, 0.0) ;
          
        float NH = dot(normal, half_vec) ; 
        vec4 specular = light_color * obj_specular * pow (max(NH, 0.0), obj_shininess) ;
         
        return diffuse + specular ;            
}       



 

void main (void) 
{       
    if (enablelighting) {      
     
        vec4 finalcolor = vec4(0.0f);
        vec3 light_dir;
        
        const vec3 eye_pos = vec3(0,0,0); // This is the convension
        vec4 mv_vertex = modelview * myvertex; // Preferably, this would be done in verx shader to same calculations
        
        vec3 frag_pos = mv_vertex.xyz / mv_vertex.w;               
        vec3 normal = normalize(mat3(transpose(inverse(modelview))) * mynormal);
        vec3 eye_dir = normalize(eye_pos - frag_pos) ;
                     
        for (int i = 0; i < numused; ++i) {

			// Point light Light        
        	if (lightposn[i].w  > 0.0f) {
        		light_dir = normalize((lightposn[i].xyz / lightposn[i].w) - frag_pos);
        	}
        	
        	// Directional light
        	else {
        		light_dir = normalize(lightposn[i].xyz);
        	}
        	
        	vec3 halfvec = normalize(light_dir + eye_dir);
        	finalcolor += CalcLight(light_dir, normal, halfvec, lightcolor[i], diffuse, specular, shininess);
                
        }
        
        // We already calculated diffusive and specular, we also add ambient and emission
        finalcolor += ambient + emission;
		fragColor = finalcolor;
         
    } else {
        fragColor = vec4(color, 1.0f); 
    }
}
