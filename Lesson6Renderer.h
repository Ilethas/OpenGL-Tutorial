#ifndef LESSON6RENDERER_H
#define LESSON6RENDERER_H
#include "TutorialRenderer.h"


class Lesson6Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(InterpolationType currentInterpolationType READ currentInterpolationType WRITE setCurrentInterpolationType NOTIFY currentInterpolationTypeChanged)
    Q_PROPERTY(bool bloomUsed READ bloomUsed WRITE setBloomUsed NOTIFY bloomUsedChanged)
    Q_PROPERTY(float blur READ blur WRITE setBlur NOTIFY blurChanged)
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    public:
    float oldH = 0;
    float oldW = 0;
    bool checkSizeChange();
    void reinitTextures();
    bool texturesDone = false;

        bool bloomUsed() const;
        void setBloomUsed(bool value);

        float blur() const;
        void setBlur(float blur);

        float threshold() const;
        void setThreshold(float threshold);

        void RenderQuad();
        enum InterpolationType
        {
            Flat,
            Smooth,
            Gouraud
        };
        Q_ENUM(InterpolationType)

        Q_INVOKABLE Lesson6Renderer();
        ~Lesson6Renderer() override;

        virtual void initialize() override;
        virtual void keyReleaseEvent(QKeyEvent* event) override;

        virtual QString getOptionsUrl() const override { return "Lesson6Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 6/Description/description.html"; }

        InterpolationType currentInterpolationType() const;
        void setCurrentInterpolationType(InterpolationType type);

    signals:
        void currentInterpolationTypeChanged(InterpolationType type);
        void bloomUsedChanged(bool value);
        void blurChanged(float blur);
        void thresholdChanged(float threshold);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;
        float blurValue = 10.0f;
        float thresholdValue = 1.0f;

        unsigned int renderedTexture;
        unsigned int framebuffer;
        unsigned int renderbuffer;

        unsigned int renderedTexture2;
        unsigned int framebuffer2;
        unsigned int renderbuffer2;

        unsigned int renderedTexture3;
        unsigned int framebuffer3;
        unsigned int renderbuffer3;

        unsigned int renderedTexture33;
        unsigned int framebuffer33;
        unsigned int renderbuffer33;

        int framebufferSize = 2048;

    private:
        unsigned int planeVAO;
        unsigned int planeVBO;
        bool bloomAllowed = true;
        InterpolationType interpolationType = Flat;
        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
        QOpenGLShaderProgram* blurProgram = nullptr;
        QOpenGLShaderProgram* blurProgram2 = nullptr;
        QOpenGLShaderProgram* brightProgram = nullptr;
        QOpenGLShaderProgram* blendProgram = nullptr;
};
Q_DECLARE_METATYPE(Lesson6Renderer*)


#endif // LESSON6RENDERER_H
