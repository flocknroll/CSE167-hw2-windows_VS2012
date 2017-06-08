// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    mat3 part1 = mat3(cos(glm::radians(degrees)));

    mat3 part2 = glm::transpose(mat3(pow(axis.x, 2), axis.x * axis.y, axis.x * axis.z,
                                     axis.x * axis.y, pow(axis.y, 2), axis.y * axis.z,
                                     axis.x * axis.z, axis.y * axis.z, pow(axis.z, 2)))
        * (1.0f - cos(glm::radians(degrees)));

    mat3 part3 = glm::transpose(mat3(0, -axis.z, axis.y,
                                     axis.z, 0, -axis.x,
                                     -axis.y, axis.x, 0))
        * sin(glm::radians(degrees));

    return part1 + part2 + part3;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    vec3 axis = glm::normalize(up);
    eye = rotate(degrees, axis) * eye;
    up = rotate(degrees, axis) * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    vec3 axis = glm::normalize(glm::cross(eye, up));

    eye = rotate(degrees, axis) * eye;
    up = rotate(degrees, axis) * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    vec3 w = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);

    mat4 rot = glm::transpose(mat4(u.x, u.y, u.z, 0,
                                   v.x, v.y, v.z, 0,
                                   w.x, w.y, w.z, 0,
                                   0, 0, 0, 1));

    mat4 trans = mat4();
    trans[3][0] = -eye.x;
    trans[3][1] = -eye.y;
    trans[3][2] = -eye.z;

    return rot * trans;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;

    float d = 1 / tan(glm::radians(fovy / 2));
    float A = -(zFar + zNear) / (zFar - zNear);
    float B = -(2 * zFar * zNear) / (zFar - zNear);

    ret = glm::transpose(mat4(d / aspect,   0,   0,  0,
                              0,            d,   0,  0,
                              0,            0,   A,  B,
                              0,            0,  -1,  0));

    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    mat4 ret = mat4();

    ret[0][0] = sx;
    ret[1][1] = sy;
    ret[2][2] = sz;

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    mat4 ret = mat4();

    ret[3][0] = tx;
    ret[3][1] = ty;
    ret[3][2] = tz;

    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
