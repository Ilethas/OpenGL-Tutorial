#ifndef LIGHT_H
#define LIGHT_H
#include <QObject>
#include <QColor>
#include "Transform.h"


class Light : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(LightType lightType READ lightType WRITE setLightType NOTIFY lightTypeChanged)
    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(float attenuationRadius READ attenuationRadius WRITE setAttenuationRadius NOTIFY attenuationRadiusChanged)
    Q_PROPERTY(float innerConeAngle READ innerConeAngle WRITE setInnerConeAngle NOTIFY innerConeAngleChanged)
    Q_PROPERTY(float outerConeAngle READ outerConeAngle WRITE setOuterConeAngle NOTIFY outerConeAngleChanged)

    public:
        enum LightType
        {
            PointLight,
            DirectionalLight,
            SpotLight
        };
        Q_ENUM(LightType)
        explicit Light(QObject* parent = nullptr);

        QVector3D position() const;
        void setPosition(QVector3D position);

        QVector3D direction() const;
        void setDirection(QVector3D direction);

        QString name() const;
        void setName(QString name);

        LightType lightType() const;
        void setLightType(LightType lightType);

        bool visibility() const;
        void setVisibility(bool visibility);

        QColor color() const;
        void setColor(QColor color);

        float brightness() const;
        void setBrightness(float brightness);

        float attenuationRadius() const;
        void setAttenuationRadius(float attenuationRadius);

        float innerConeAngle() const;
        void setInnerConeAngle(float innerConeAngle);

        float outerConeAngle() const;
        void setOuterConeAngle(float outerConeAngle);

    signals:
        void positionChanged(QVector3D position);
        void directionChanged(QVector3D direction);
        void nameChanged(QString name);
        void lightTypeChanged(LightType lightType);
        void visibilityChanged(bool visibility);
        void colorChanged(QColor color);
        void brightnessChanged(float brightness);
        void attenuationRadiusChanged(float attenuationRadius);
        void innerConeAngleChanged(float innerConeAngle);
        void outerConeAngleChanged(float outerConeAngle);

    public slots:

    private:
        QVector3D lightPosition = QVector3D(0.0f, 0.0f, 0.0f);
        QVector3D lightDirection = QVector3D(1.0f, 0.0f, 0.0f);
        LightType type = PointLight;
        bool lightVisibility = true;
        QColor lightColor = QColor::fromRgb(255, 255, 255);
        QString lightName = "Light";
        float lightBrightness = 1.0f;
        float lightAttenuationRadius = 10.0f;
        float lightInnerConeAngle = 30.0f;
        float lightOuterConeAngle = 60.0f;
};


#endif // LIGHT_H
