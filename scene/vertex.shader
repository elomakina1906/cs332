varying vec3 normal;
varying vec3 position;
varying vec2 texture;

uniform vec3 point_position;
varying vec3 point_position_view;

uniform mat4 light_model_matrix;

void main(void)
{
   position = vec3(gl_ModelViewMatrix * gl_Vertex);       
   normal = normalize(gl_NormalMatrix * gl_Normal);
   texture = gl_MultiTexCoord0.xy;
   point_position_view = vec3(light_model_matrix * vec4(point_position,1));
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
