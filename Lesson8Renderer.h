#ifndef LESSON8RENDERER_H
#define LESSON8RENDERER_H
#include "TutorialRenderer.h"


class Lesson8Renderer : public TutorialRenderer
{
    Q_OBJECT

    public:
        Q_INVOKABLE Lesson8Renderer();
        ~Lesson8Renderer() override;

        virtual void initialize() override;

        virtual QString getOptionsUrl() const override { return "Lesson8Options.qml"; }
        virtual QString getDescriptionUrl() const override { return "file:///" + QDir::currentPath() + "/Assets/Lesson 8/Description/description.html"; }

    signals:

    public slots:
        virtual void paint() override;

    protected:
        virtual void update(float deltaTime) override;

    private:
        QTime gameTime;
        Transform modelTransform;
        Model model;

        QOpenGLShaderProgram* program = nullptr;
};
Q_DECLARE_METATYPE(Lesson8Renderer*)


#endif // LESSON8RENDERER_H
