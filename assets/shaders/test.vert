#version 120

attribute vec3 v_position;
attribute vec3 v_color;
attribute vec2 v_texcoord;
         
varying vec3 color;
varying vec3 fragPos;
varying vec3 normal;
varying vec3 viewDir;
varying vec2 tCoord;
          
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
  viewDir = fragPos - vec3(0,0,-70);
}
          
