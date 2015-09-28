// Vertex Shader used by 'SpriteText'
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main() {
  color = spriteColor;
  //color = vec4(spriteColor) * texture(image, TexCoords);
}