#include "Light.h"


Light::Light(QObject* parent) : QObject(parent)
{

}


QVector3D Light::position() const
{
    return lightPosition;
}


void Light::setPosition(QVector3D position)
{
    if (!qFuzzyCompare((lightPosition - position).length(), 0))
    {
        lightPosition = position;
        positionChanged(position);
    }
}


QVector3D Light::direction() const
{
    return lightDirection;
}


void Light::setDirection(QVector3D direction)
{
    if (!qFuzzyCompare((lightDirection.normalized() - direction.normalized()).length(), 0))
    {
        lightDirection = direction;
        positionChanged(direction);
    }
}


QString Light::name() const
{
    return lightName;
}


void Light::setName(QString name)
{
    if (lightName != name)
    {
        lightName = name;
        emit nameChanged(name);
    }
}


Light::LightType Light::lightType() const
{
    return type;
}


void Light::setLightType(Light::LightType lightType)
{
    if (type != lightType)
    {
        type = lightType;
        emit lightTypeChanged(lightType);
    }
}


bool Light::visibility() const
{
    return lightVisibility;
}


void Light::setVisibility(bool visibility)
{
    if (lightVisibility != visibility)
    {
        lightVisibility = visibility;
        emit visibilityChanged(visibility);
    }
}


QColor Light::color() const
{
    return lightColor;
}


void Light::setColor(QColor color)
{
    if (lightColor != color)
    {
        lightColor = color;
        emit colorChanged(color);
    }
}

float Light::brightness() const
{
    return lightBrightness;
}

void Light::setBrightness(float brightness)
{
    if (lightBrightness != brightness)
    {
        lightBrightness = brightness;
        emit brightnessChanged(brightness);
    }
}


float Light::attenuationRadius() const
{
    return lightAttenuationRadius;
}


void Light::setAttenuationRadius(float attenuationRadius)
{
    if (lightAttenuationRadius != attenuationRadius)
    {
        lightAttenuationRadius = attenuationRadius;
        emit attenuationRadiusChanged(attenuationRadius);
    }
}


float Light::innerConeAngle() const
{
    return lightInnerConeAngle;
}


void Light::setInnerConeAngle(float innerConeAngle)
{
    if (innerConeAngle > lightOuterConeAngle)
    {
        innerConeAngle = lightOuterConeAngle;
    }

    if (lightInnerConeAngle != innerConeAngle)
    {
        lightInnerConeAngle = innerConeAngle;
        emit innerConeAngleChanged(innerConeAngle);
    }
}


float Light::outerConeAngle() const
{
    return lightOuterConeAngle;
}


void Light::setOuterConeAngle(float outerConeAngle)
{
    if (outerConeAngle < lightInnerConeAngle)
    {
        outerConeAngle = lightInnerConeAngle;
    }

    if (lightOuterConeAngle != outerConeAngle)
    {
        lightOuterConeAngle = outerConeAngle;
        emit outerConeAngleChanged(outerConeAngle);
    }
}
