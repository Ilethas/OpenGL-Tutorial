#ifndef LESSON5RENDERER_H
#define LESSON5RENDERER_H
#include "TutorialRenderer.h"


class Lesson5Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(bool normalMappingUsed READ normalMappingUsed WRITE setNormalMappingUsed NOTIFY normalMappingUsedChanged)
    Q_PROPERTY(bool displayAsColor READ displayAsColor WRITE setDisplayAsColor NOTIFY displayAsColorChanged)
    Q_PROPERTY(bool displayVectors READ displayVectors WRITE setDisplayVectors NOTIFY displayVectorsChanged)
    Q_PROPERTY(bool gammaCorrectionUsed READ gammaCorrectionUsed WRITE setGammaCorrectionUsed NOTIFY gammaCorrectionUsedChanged)
    Q_PROPERTY(bool hdrUsed READ hdrUsed WRITE setHdrUsed NOTIFY hdrUsedChanged)
    Q_PROPERTY(float gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
    Q_PROPERTY(float exposure READ exposure WRITE setExposure NOTIFY exposureChanged)

    public:
        Q_INVOKABLE Lesson5Renderer();
        ~Lesson5Renderer();

        virtual void initialize() override;

        virtual QString getOptionsUrl() const override { return "Lesson5Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 5/Description/description.html"; }

        bool normalMappingUsed() const;
        void setNormalMappingUsed(bool value);

        bool displayAsColor() const;
        void setDisplayAsColor(bool value);

        bool displayVectors() const;
        void setDisplayVectors(bool value);

        bool gammaCorrectionUsed() const;
        void setGammaCorrectionUsed(bool value);

        bool hdrUsed() const;
        void setHdrUsed(bool value);

        float gamma() const;
        void setGamma(float gamma);

        float exposure() const;
        void setExposure(float exposure);

    signals:
        void normalMappingUsedChanged(bool value);
        void displayAsColorChanged(bool value);
        void displayVectorsChanged(bool value);
        void gammaCorrectionUsedChanged(bool value);
        void gammaChanged(float gamma);
        void hdrUsedChanged(bool value);
        void exposureChanged(float exposure);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;
        float gammaValue = 1.0f;
        float exposureValue = 1.0f;

    private:
        bool normalMapsAllowed = true;
        bool displayNormalsAsColors = false;
        bool displayNormalsAsVectors = false;
        bool gammaCorrectionAllowed = true;
        bool hdrAllowed = false;
        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
        QOpenGLShaderProgram* vectorsProgram = nullptr;
};
Q_DECLARE_METATYPE(Lesson5Renderer*)


#endif // LESSON5RENDERER_H
