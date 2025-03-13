#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 colorDifuso;  // Pasar Kd desde OpenGL

void main()
{    
    vec4 texColor = texture(texture_diffuse1, TexCoords);

    // Si la textura no existe o es negra, usa el color difuso del material
    if (texColor.rgb == vec3(0.0, 0.0, 0.0)) {
        texColor = vec4(colorDifuso, 1.0);
    }

    FragColor = texColor;
}
