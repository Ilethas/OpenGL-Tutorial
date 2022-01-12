#ifndef LESSON7RENDERER_H
#define LESSON7RENDERER_H
#include "TutorialRenderer.h"


class Lesson7Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(float levels READ levels WRITE setLevels NOTIFY levelsChanged)

    public:
        Q_INVOKABLE Lesson7Renderer();
        ~Lesson7Renderer() override;

        virtual void initialize() override;

        virtual QString getOptionsUrl() const override { return "Lesson7Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 7/Description/description.html"; }

        bool normalMappingUsed() const;
        void setNormalMappingUsed(bool value);

        bool displayAsColor() const;
        void setDisplayAsColor(bool value);

        bool displayVectors() const;
        void setDisplayVectors(bool value);

        float levels() const;
        void setLevels(float gamma);

    signals:
        void normalMappingUsedChanged(bool value);
        void displayAsColorChanged(bool value);
        void displayVectorsChanged(bool value);
        void levelsChanged(bool value);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;
        float levelsValue = 5.0f;

    private:
        unsigned int planeVAO;
        unsigned int planeVBO;

        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
        QOpenGLShaderProgram* outlineProgram = nullptr;
        unsigned int renderedTexture;
        unsigned int framebuffer;
        unsigned int renderbuffer;
        int framebufferSize = 2048;
};
Q_DECLARE_METATYPE(Lesson7Renderer*)


#endif // LESSON7RENDERER_H
