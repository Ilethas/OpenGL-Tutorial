#ifndef LIGHTMODEL_H
#define LIGHTMODEL_H
#include <QAbstractListModel>
#include <vector>
#include <memory>
#include "Light.h"


class LightModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int updateCounter READ updateCounter WRITE setUpdateCounter NOTIFY updateCounterChanged)

    public:
        enum LightRole
        {
            Name = Qt::UserRole + 1,
            Position,
            Direction,
            LightType,
            Visibility,
            Color,
            Brightness,
            AttenuationRadius,
            InnerConeAngle,
            OuterConeAngle
        };
        Q_ENUM(LightRole)
        LightModel(QObject* parent = nullptr);
        int updateCounter() const;
        void setUpdateCounter(int value);

        void append(std::unique_ptr<Light> light);
        Q_INVOKABLE void append(QVariant light);
        Q_INVOKABLE void remove(quint32 index);
        Q_INVOKABLE bool setLightProperty(int lightIndex, const QVariant& value, LightRole role);
        Q_INVOKABLE QVariant getLightProperty(int lightIndex, LightRole role) const;

        Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    signals:
        void updateCounterChanged(int counter);

    protected:
        QHash<int, QByteArray> roleNames() const override;

    private:
        std::vector<std::unique_ptr<Light>> lights;
        int counter = 0;
};


#endif // LIGHTMODEL_H
