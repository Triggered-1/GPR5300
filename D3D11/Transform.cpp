#include "Transform.h"

Transform::Transform()
{
    position = Vector3();
    rotation = Vector3();
    scale = Vector3(1.0f, 1.0f, 1.0f);
}

XMMATRIX Transform::GetTransformationMatrix()
{
    return
        XMMatrixScaling(scale.x, scale.y, scale.z) *
        XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
        XMMatrixTranslation(position.x, position.y, position.z);
}

Vector3 Transform::forward()
{
    Vector3 vector = {};
    vector.x = cos(rotation.x) * sin(rotation.y);
    vector.y = -sin(rotation.x);
    vector.z = cos(rotation.x) * cos(rotation.y);
    return vector;
}

Vector3 Transform::back()
{
    Vector3 vector = {};
    vector.x = -cos(rotation.x) * sin(rotation.y);
    vector.y = sin(rotation.x);
    vector.z = -cos(rotation.x) * cos(rotation.y);
    return vector;
}

Vector3 Transform::right()
{
    Vector3 vector = {};
    vector.x = cos(rotation.y);
    vector.y = 0;
    vector.z = -sin(rotation.y);
    return vector;
}

Vector3 Transform::left()
{
    Vector3 vector = {};
    vector.x = -cos(rotation.y);
    vector.y = 0;
    vector.z = sin(rotation.y);
    return vector;
}

Vector3 Transform::up()
{
    Vector3 vector = {};
    vector.x = sin(rotation.x) * sin(rotation.y);
    vector.y = cos(rotation.x);
    vector.z = sin(rotation.x) * cos(rotation.y);
    return vector;
}

Vector3 Transform::down() 
{
    Vector3 vector = {};
    vector.x = -sin(rotation.x) * sin(rotation.y);
    vector.y = -cos(rotation.x);
    vector.z = -sin(rotation.x) * cos(rotation.y);
    return vector;
}
