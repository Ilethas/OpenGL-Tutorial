#include "Transform.h"
#include <QMatrix3x3>


Transform::Transform(QVector3D position, QQuaternion rotation, QVector3D scale)
    : objectPosition(position)
    , objectRotation(rotation)
    , objectScale(scale)
{

}


QVector3D Transform::position() const
{
    return objectPosition;
}


QQuaternion Transform::rotation() const
{
    return objectRotation;
}


QVector3D Transform::scale() const
{
    return objectScale;
}


void Transform::setPosition(QVector3D position)
{
    objectPosition = position;
}

void Transform::setPosition(float x, float y, float z)
{
    objectPosition = QVector3D(x, y, z);
}


void Transform::setRotation(QQuaternion rotation)
{
    objectRotation = rotation;
}


void Transform::setRotation(QVector3D axis, float angle)
{
    objectRotation = QQuaternion::fromAxisAndAngle(axis, angle);
}


void Transform::setRotation(float xAngle, float yAngle, float zAngle)
{
    objectRotation = QQuaternion::fromEulerAngles(xAngle, yAngle, zAngle);
}


void Transform::setRotation(QVector3D xyzAngles)
{
    objectRotation = QQuaternion::fromEulerAngles(xyzAngles);
}


void Transform::setScale(QVector3D scale)
{
    objectScale = scale;
}

void Transform::setScale(float x, float y, float z)
{
    objectScale = QVector3D(x, y, z);
}


void Transform::setScale(float scale)
{
    objectScale = QVector3D(scale, scale, scale);
}

void Transform::move(float x, float y, float z)
{
    objectPosition += QVector3D(x, y, z);
}


void Transform::move(QVector3D offset)
{
    objectPosition += offset;
}


void Transform::scaleBy(float value)
{
    objectScale *= value;
}


void Transform::scaleBy(QVector3D values)
{
    objectScale *= values;
}


void Transform::rotate(QQuaternion rotation, Space space)
{
    if (space == Space::Local)
    {
        objectRotation = objectRotation * rotation;
    }
    else
    {
        objectRotation = rotation * objectRotation;
    }
}


void Transform::rotate(QVector3D axis, float angle, Space space)
{
    rotate(QQuaternion::fromAxisAndAngle(axis.x(), axis.y(), axis.z(), angle), space);
}


void Transform::rotate(float xAngle, float yAngle, float zAngle, Space space)
{
    rotate(QQuaternion::fromEulerAngles(xAngle, yAngle, zAngle), space);
}


void Transform::lookAt(QVector3D direction, QVector3D upVector, bool orthogonalize)
{
    upVector = upVector.normalized();

    QVector3D forward = direction.normalized();
    QVector3D up = orthogonalize? (upVector - QVector3D::dotProduct(forward, upVector) * forward).normalized() : upVector;
    QVector3D right = QVector3D::crossProduct(up, forward);

    float lookRotation[] =
    {
        forward.x(), right.x(), up.x(),
        forward.y(), right.y(), up.y(),
        forward.z(), right.z(), up.z()
    };

    objectRotation = QQuaternion::fromRotationMatrix(QMatrix3x3(lookRotation));
}


void Transform::lookAt(float directionX, float directionY, float directionZ, float upVectorX, float upVectorY, float upVectorZ, bool orthogonalize)
{
    lookAt(QVector3D(directionX, directionY, directionZ), QVector3D(upVectorX, upVectorY, upVectorZ), orthogonalize);
}


QVector3D Transform::localX() const
{
    return objectRotation.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
}


QVector3D Transform::localY() const
{
    return objectRotation.rotatedVector(QVector3D(0.0f, 1.0f, 0.0f));
}


QVector3D Transform::localZ() const
{
    return objectRotation.rotatedVector(QVector3D(0.0f, 0.0f, 1.0f));
}
