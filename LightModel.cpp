#include "LightModel.h"
#include <QJSValue>
#include <algorithm>


LightModel::LightModel(QObject* parent) : QAbstractListModel(parent)
{

}


int LightModel::updateCounter() const
{
    return counter;
}


void LightModel::setUpdateCounter(int value)
{
    if (counter != value)
    {
        counter = value;
        emit updateCounterChanged(value);
    }
}


void LightModel::append(std::unique_ptr<Light> light)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    lights.push_back(std::move(light));
    endInsertRows();
}


void LightModel::append(QVariant light)
{
    if (!light.canConvert<QJSValue>())
    {
        return;
    }
    QJSValue object = light.value<QJSValue>();

    std::unique_ptr<Light> newLight = std::make_unique<Light>();
    newLight->setName(object.property("name").toString());

    append(std::move(newLight));
}


void LightModel::remove(quint32 index)
{
    removeRows(index, 1);
}


bool LightModel::setLightProperty(int lightIndex, const QVariant& value, LightRole role)
{
    if (lightIndex < 0 || lightIndex >= static_cast<int>(lights.size()))
    {
        return false;
    }

    Light& light = *lights[lightIndex];
    if (role == Name && value.canConvert<QString>())
    {
        light.setName(value.value<QString>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == Position && value.canConvert<QVector3D>())
    {
        light.setPosition(value.value<QVector3D>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == Direction && value.canConvert<QVector3D>())
    {
        light.setDirection(value.value<QVector3D>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == LightType && value.canConvert<Light::LightType>())
    {
        light.setLightType(value.value<Light::LightType>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == Visibility && value.canConvert<bool>())
    {
        light.setVisibility(value.value<bool>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == Color && value.canConvert<QColor>())
    {
        light.setColor(value.value<QColor>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == Brightness && value.canConvert<float>())
    {
        light.setBrightness(value.value<float>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == AttenuationRadius && value.canConvert<float>())
    {
        light.setAttenuationRadius(value.value<float>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == InnerConeAngle && value.canConvert<float>())
    {
        light.setInnerConeAngle(value.value<float>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    else if (role == OuterConeAngle && value.canConvert<float>())
    {
        light.setOuterConeAngle(value.value<float>());
        emit dataChanged(index(lightIndex), index(lightIndex), QVector<int>({role}));
        setUpdateCounter(counter + 1);
        return true;
    }
    return false;
}


QVariant LightModel::getLightProperty(int lightIndex, LightRole role) const
{
    if (lightIndex < 0 || lightIndex >= static_cast<int>(lights.size()))
    {
        return QVariant();
    }

    const Light& light = *lights[lightIndex];
    if (role == Name)
    {
        return light.name();
    }
    else if (role == Position)
    {
        return light.position();
    }
    else if (role == Direction)
    {
        return light.direction();
    }
    else if (role == LightType)
    {
        return light.lightType();
    }
    else if (role == Visibility)
    {
        return light.visibility();
    }
    else if (role == Color)
    {
        return light.color();
    }
    else if (role == Brightness)
    {
        return light.brightness();
    }
    else if (role == AttenuationRadius)
    {
        return light.attenuationRadius();
    }
    else if (role == InnerConeAngle)
    {
        return light.innerConeAngle();
    }
    else if (role == OuterConeAngle)
    {
        return light.outerConeAngle();
    }
    return QVariant();
}


int LightModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return lights.size();
}


QVariant LightModel::data(const QModelIndex& index, int role) const
{
    return getLightProperty(index.row(), static_cast<LightRole>(role));
}


bool LightModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return setLightProperty(index.row(), value, static_cast<LightRole>(role));
}


bool LightModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 || row + count > static_cast<int>(lights.size()))
    {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    lights.erase(lights.begin() + row, lights.begin() + row + count);
    endRemoveRows();
    return true;
}


QHash<int, QByteArray> LightModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Position] = "position";
    roles[Direction] = "direction";
    roles[LightType] = "lightType";
    roles[Visibility] = "visibility";
    roles[Color] = "color";
    roles[Brightness] = "brightness";
    roles[AttenuationRadius] = "attenuationRadius";
    roles[InnerConeAngle] = "innerConeAngle";
    roles[OuterConeAngle] = "outerConeAngle";
    return roles;
}
