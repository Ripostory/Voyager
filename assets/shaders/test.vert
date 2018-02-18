#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texcoord;
         
smooth out vec3 color; 
out vec3 fragPos;
out vec3 normal;
out vec3 viewDir;
out vec2 tCoord;
          
uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
        
void main(void) 
{ 
  vec4 v = vec4(v_position, 1.0); 
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; 
  fragPos = vec3(modelMatrix * vec4(v_position, 1.0));
  
  tCoord = v_texcoord;
  
  //did normal calculation in color array
  //modify normals to world space
  vec3 aNormal = v_color;
  
  //transform to proper coordinates
  normal = vec3(modelMatrix * vec4(aNormal, 0.0f));
  
  //get view position
  //we know where the camera is, since it isn't the one that moves
  viewDir = fragPos - vec3(0,0,-50);
}
          
