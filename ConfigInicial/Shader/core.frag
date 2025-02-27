#version 330 core
out vec4 FragColor;
uniform vec3 color;  // Nuevo uniform para definir el color

void main()
{
    FragColor = vec4(color, 1.0f);  // Se usa el color uniforme
}
