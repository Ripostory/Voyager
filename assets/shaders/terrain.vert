#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texcoord;
         
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 texCoordModel;
        
void main(void) 
{ 
  gl_Position = vec4(v_position, 1.0); 
  texCoordModel = v_texcoord; 
}
