#include "common.h"

#include "globals.h"

void calculate_mvp(vec3 pos, mat4 out)
{
    // Translate to the object position
    mat4 translation;
    glm_mat4_identity(translation);
    glm_translate(translation, pos);

    // Negativelly translate by the camera position
    vec3 negative_camera_pos;
    glm_vec3_copy(camera_position, negative_camera_pos);
    negative_camera_pos[0] = -negative_camera_pos[0];
    negative_camera_pos[1] = -negative_camera_pos[1];
    negative_camera_pos[2] = -negative_camera_pos[2];
    glm_translate(translation, negative_camera_pos);

    glm_mul(projection_matrix, translation, out);
}

void calculate_mvp_without_camera(vec3 pos, mat4 out)
{
    mat4 translation;
    glm_mat4_identity(translation);
    glm_translate(translation, pos);

    glm_mul(projection_matrix, translation, out);
}