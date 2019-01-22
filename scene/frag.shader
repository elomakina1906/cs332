varying vec3 normal;
varying vec3 position;  
varying vec2 texture;
  
uniform sampler2D texture_image; 

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;

uniform bool point_active;
varying vec3 point_position_view;
uniform vec3 point_ambient;
uniform vec3 point_diffuse;
uniform vec3 point_specular;

void main (void)  
{  
  vec3 texture_color = vec3(texture2D(texture_image, texture));
  vec3 color;
  vec3 norm = normalize(normal);
  if (point_active) {
    vec3 color_ambient = material_ambient*point_ambient;
	vec3 surf2light = normalize(point_position_view-position);
	float dcont = max(0.0, dot(norm, surf2light));
	vec3 color_diffuse = material_diffuse*dcont*point_diffuse;
	vec3 surf2view=normalize(-point_position_view);
    vec3 reflection=reflect(-surf2light,norm);
    float scont=pow(max(0.0,dot(surf2view,reflection)), 10);
    float dist=length(position-point_position_view);
    float att=1.0/(1.0+0.1*dist+0.01*dist*dist);
    vec3 color_specular = scont*dcont*point_specular*material_specular*att;
    color += color_ambient + color_diffuse + color_specular;
  }
  gl_FragColor += vec4(texture_color*color, 1); 
}