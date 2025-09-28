#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 color = texture(texture_diffuse1, TexCoords);
    if (color.a < 0.1) discard; // Para transparencias, si aplica
    if (TexCoords.x < 0.0 || TexCoords.x > 1.0 || TexCoords.y < 0.0 || TexCoords.y > 1.0)
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rojo si UVs fuera de rango
    else
        FragColor = color;
}