#version 330

uniform vec4 arcColor;
//uniform vec4 bgArcColor;
out vec4 arcFragColor;
//out vec4 bgArcFragColor;

void main(void)
{
    arcFragColor = arcColor;
    //bgArcFragColor = bgArcColor;
}
