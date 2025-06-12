#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);

    finalColor = texelColor * colDiffuse;

    finalColor.rgb += vec3(0.2, 0.2, 0.2);

    finalColor.rgb = max(finalColor.rgb, vec3(0.3));

    finalColor.a = 1.0;
}
