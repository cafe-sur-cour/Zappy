#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;
uniform mat4 matView;
uniform mat4 matProjection;

out vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    mat4 viewMatrixNoTranslation = matView;
    viewMatrixNoTranslation[3][0] = 0.0;
    viewMatrixNoTranslation[3][1] = 0.0;
    viewMatrixNoTranslation[3][2] = 0.0;

    gl_Position = mvp * vec4(vertexPosition, 1.0);

    gl_Position = gl_Position.xyww;
}
