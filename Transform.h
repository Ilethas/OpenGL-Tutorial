#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include <QVector3D>
#include <QQuaternion>


class Transform
{
    public:
        enum class Space
        {
            Local,
            World
        };

        Transform(const QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
                  QQuaternion rotation = QQuaternion(1.0f, 0.0f, 0.0f, 0.0f),
                  QVector3D scale = QVector3D(1.0f, 1.0f, 1.0f));

        QVector3D position() const;
        QQuaternion rotation() const;
        QVector3D scale() const;

        void setPosition(QVector3D position);
        void setPosition(float x, float y, float z);
        void setRotation(QQuaternion rotation);
        void setRotation(QVector3D axis, float angle);
        void setRotation(float xAngle, float yAngle, float zAngle);
        void setRotation(QVector3D xyzAngles);
        void setScale(QVector3D scale);
        void setScale(float x, float y, float z);
        void setScale(float scale);

        void move(float x, float y, float z);
        void move(QVector3D offset);
        void scaleBy(float value);
        void scaleBy(QVector3D values);
        void rotate(QQuaternion rotation, Space space = Space::Local);
        void rotate(QVector3D axis, float angle, Space space = Space::Local);
        void rotate(float xAngle, float yAngle, float zAngle, Space space = Space::Local);
        void lookAt(QVector3D direction, QVector3D upVector = QVector3D(0.0f, 0.0f, 1.0f), bool orthogonalize = true);
        void lookAt(float directionX, float directionY, float directionZ, float upVectorX = 0.0f, float upVectorY = 0.0f, float upVectorZ = 1.0f, bool orthogonalize = true);

        QVector3D localX() const;
        QVector3D localY() const;
        QVector3D localZ() const;

    private:
        QVector3D objectPosition;
        QQuaternion objectRotation;
        QVector3D objectScale;
};


#endif // TRANSFORM_H_
