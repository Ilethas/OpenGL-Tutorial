#ifndef CAMERA_H
#define CAMERA_H
#include "Transform.h"
#include <QMatrix4x4>


class Camera
{
    public:
        Camera(Transform transform);
        Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
               QQuaternion rotation = QQuaternion(1.0f, 0.0f, 0.0f, 0.0f),
               QVector3D scale = QVector3D(1.0f, 1.0f, 1.0f));

        Transform& transform();
        const Transform& transform() const;
        void setTransform(Transform transform);

        static QMatrix4x4 perspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
        static QMatrix4x4 orthographic(float right, float left, float top, float bottom, float nearPlane, float farPlane);

    private:
        Transform cameraTransform;
};


#endif // CAMERA_H
