#ifndef LESSON1RENDERER_H
#define LESSON1RENDERER_H
#include "TutorialRenderer.h"


class Lesson1Renderer : public TutorialRenderer
{
    Q_OBJECT
    Q_PROPERTY(InterpolationType currentInterpolationType READ currentInterpolationType WRITE setCurrentInterpolationType NOTIFY currentInterpolationTypeChanged)

    public:
        enum InterpolationType
        {
            Flat,
            Smooth,
            Gouraud
        };
        Q_ENUM(InterpolationType)

        Q_INVOKABLE Lesson1Renderer();
        ~Lesson1Renderer() override;

        virtual void initialize() override;
        virtual void keyReleaseEvent(QKeyEvent* event) override;

        virtual QString getOptionsUrl() const override { return "Lesson1Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 1/Description/description.html"; }

        InterpolationType currentInterpolationType() const;
        void setCurrentInterpolationType(InterpolationType type);

    signals:
        void currentInterpolationTypeChanged(InterpolationType type);

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;

    private:
        InterpolationType interpolationType = Flat;
        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
};
Q_DECLARE_METATYPE(Lesson1Renderer*)


#endif // LESSON1RENDERER_H
