#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out VS_OUT{
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(normalMatrix * aNormal));
    gl_Position = view * model * vec4(aPos, 1.0);
}

#shader geometry
#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;
uniform mat4 projection;

void GenerateLine(int index) {
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = projection * (gl_in[index].gl_Position + MAGNITUDE * vec4(gs_in[index].normal, 0.0));
    EmitVertex();

    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}