#ifndef LESSON2RENDERER_H
#define LESSON2RENDERER_H
#include "TutorialRenderer.h"


class Lesson2Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(bool normalMappingUsed READ normalMappingUsed WRITE setNormalMappingUsed NOTIFY normalMappingUsedChanged)
    Q_PROPERTY(bool displayAsColor READ displayAsColor WRITE setDisplayAsColor NOTIFY displayAsColorChanged)
    Q_PROPERTY(bool displayVectors READ displayVectors WRITE setDisplayVectors NOTIFY displayVectorsChanged)

    public:
        Q_INVOKABLE Lesson2Renderer();
        ~Lesson2Renderer() override;

        virtual void initialize() override;

        virtual QString getOptionsUrl() const override { return "Lesson2Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 2/Description/description.html"; }

        bool normalMappingUsed() const;
        void setNormalMappingUsed(bool value);

        bool displayAsColor() const;
        void setDisplayAsColor(bool value);

        bool displayVectors() const;
        void setDisplayVectors(bool value);

    signals:
        void normalMappingUsedChanged(bool value);
        void displayAsColorChanged(bool value);
        void displayVectorsChanged(bool value);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;

    private:
        bool normalMapsAllowed = true;
        bool displayNormalsAsColors = false;
        bool displayNormalsAsVectors = false;
        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
        QOpenGLShaderProgram* vectorsProgram = nullptr;
};
Q_DECLARE_METATYPE(Lesson2Renderer*)


#endif // LESSON2RENDERER_H
