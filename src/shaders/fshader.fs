#version 330 core
out vec4 FragColor;

in vec3 ourPosition;
in vec2 TexCoord;

uniform sampler2D ourTexture;


float dist(vec3 pos){
  return sqrt(pow(pos.x,2) + pow(pos.y,2) + pow(pos.z,2));
}

void main(){
  FragColor = vec4(1-ourPosition.x,1-ourPosition.y,1-ourPosition.z,1.0);
  //FragColor = texture(ourTexture, TexCoord);
}
