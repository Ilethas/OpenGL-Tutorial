#include "Camera.h"
#include <QtMath>


Camera::Camera(Transform transform)
    : cameraTransform(transform)
{

}

Camera::Camera(QVector3D position, QQuaternion rotation, QVector3D scale)
    : cameraTransform(position, rotation, scale)
{

}


Transform& Camera::transform()
{
    return cameraTransform;
}


const Transform& Camera::transform() const
{
    return cameraTransform;
}


void Camera::setTransform(Transform transform)
{
    cameraTransform = transform;
}


QMatrix4x4 Camera::perspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    float coeff1 = 1 / std::tan(qDegreesToRadians(fieldOfView / 2));
    float coeff2 = aspectRatio * coeff1;
    float coeff3 = (farPlane + nearPlane) / (farPlane - nearPlane);
    float coeff4 = 2 * farPlane * nearPlane / (nearPlane - farPlane);

    return QMatrix4x4(
                0,      coeff1, 0,      0,
                0,      0,      coeff2, 0,
                coeff3, 0,      0,      coeff4,
                1,      0,      0,      0);
}


QMatrix4x4 Camera::orthographic(float right, float left, float top, float bottom, float nearPlane, float farPlane)
{
    float coeff1 = 2 / (right - left);
    float coeff2 = (right + left) / (left - right);
    float coeff3 = 2 / (top - bottom);
    float coeff4 = (top + bottom) / (bottom - top);
    float coeff5 = 2 / (farPlane - nearPlane);
    float coeff6 = (farPlane + nearPlane) / (nearPlane - farPlane);

    return QMatrix4x4(
                0,      coeff1, 0,      coeff2,
                0,      0,      coeff3, coeff4,
                coeff5, 0,      0,      coeff6,
                0,      0,      0,      1);
}
