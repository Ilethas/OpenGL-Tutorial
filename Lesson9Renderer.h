#ifndef LESSON9RENDERER_H
#define LESSON9RENDERER_H
#include "TutorialRenderer.h"


class Lesson9Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(bool normalMappingUsed READ normalMappingUsed WRITE setNormalMappingUsed NOTIFY normalMappingUsedChanged)
    Q_PROPERTY(bool displayAsColor READ displayAsColor WRITE setDisplayAsColor NOTIFY displayAsColorChanged)
    Q_PROPERTY(bool displayVectors READ displayVectors WRITE setDisplayVectors NOTIFY displayVectorsChanged)
    Q_PROPERTY(bool gammaCorrectionUsed READ gammaCorrectionUsed WRITE setGammaCorrectionUsed NOTIFY gammaCorrectionUsedChanged)
    Q_PROPERTY(bool outliningUsed READ outliningUsed WRITE setOutliningUsed NOTIFY outliningUsedChanged)
    Q_PROPERTY(bool cubeOutliningUsed READ cubeOutliningUsed WRITE setCubeOutliningUsed NOTIFY cubeOutliningUsedChanged)
    Q_PROPERTY(bool cubeOutlining2Used READ cubeOutlining2Used WRITE setCubeOutlining2Used NOTIFY cubeOutlining2UsedChanged)
    Q_PROPERTY(float gamma READ gamma WRITE setGamma NOTIFY gammaChanged)

    public:
        Q_INVOKABLE Lesson9Renderer();
        ~Lesson9Renderer();

        virtual void initialize() override;

        virtual QString getOptionsUrl() const override { return "Lesson9Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 9/Description/description.html"; }

        bool normalMappingUsed() const;
        void setNormalMappingUsed(bool value);

        bool displayAsColor() const;
        void setDisplayAsColor(bool value);

        bool displayVectors() const;
        void setDisplayVectors(bool value);

        bool gammaCorrectionUsed() const;
        void setGammaCorrectionUsed(bool value);

        bool outliningUsed() const;
        void setOutliningUsed(bool value);

        bool cubeOutliningUsed() const;
        bool cubeOutlining2Used() const;

        void setCubeOutliningUsed(bool value);
        void setCubeOutlining2Used(bool value);

        float gamma() const;
        void setGamma(float gamma);

    signals:
        void normalMappingUsedChanged(bool value);
        void displayAsColorChanged(bool value);
        void displayVectorsChanged(bool value);
        void gammaCorrectionUsedChanged(bool value);
        void outliningUsedChanged(bool value);
        void cubeOutliningUsedChanged(bool value);
        void cubeOutlining2UsedChanged(bool value);
        void gammaChanged(float gamma);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;
        float gammaValue = 1.0f;

    private:
        bool normalMapsAllowed = true;
        bool displayNormalsAsColors = false;
        bool displayNormalsAsVectors = false;
        bool gammaCorrectionAllowed = true;
        bool outliningAllowed = true;
        bool cubeOutliningAllowed = false;
        bool cubeOutlining2Allowed = false;
        Transform modelTransform;
        Model model;
        Model cubeModel;
        Model cubeModel2;

        QOpenGLShaderProgram* program = nullptr;
        QOpenGLShaderProgram* vectorsProgram = nullptr;
        QOpenGLShaderProgram* stencilProgram = nullptr;
};
Q_DECLARE_METATYPE(Lesson9Renderer*)


#endif // LESSON9RENDERER_H
