#version 330

// Input
// TODO(student): Get vertex attributes from each location
// NU CONTEAZA ORDINEA
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
// layout(location = 3) in int v_objectType;

layout(location = 3) in vec3 v_color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;


// Output
out vec3 frag_position;
out vec3 frag_color;
out vec3 frag_normal;

// ATENTIE normala e vec2
out vec2 frag_texture;


void main()
{
    // TODO(student): Send output to fragment shader
    

    frag_color = object_color;
    frag_position = v_position;
    frag_normal = v_normal;
    frag_texture = v_texture;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
