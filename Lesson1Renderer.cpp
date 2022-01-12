#include "Lesson1Renderer.h"


Lesson1Renderer::Lesson1Renderer()
{
}


Lesson1Renderer::~Lesson1Renderer()
{
    if (initialized())
    {
        delete program;
        model.clear();
    }
}


void Lesson1Renderer::initialize()
{
    TutorialRenderer::initialize();
    if (!initialized())
    {
        return;
    }

    //===============
    // Create initial light
    lights.append(std::make_unique<Light>());
    lights.setData(lights.index(0), "Światło", LightModel::Name);
    lights.setData(lights.index(0), QVector3D(4.0f, -6.0f, 2.0f), LightModel::Position);
    lights.setData(lights.index(0), QVector3D(-1.0f, 0.0f, 0.0f), LightModel::Direction);
    lights.setData(lights.index(0), QVector3D(1.0f, 1.0f, 1.0f), LightModel::Color);
    lights.setData(lights.index(0), Light::PointLight, LightModel::LightType);
    lights.setData(lights.index(0), 1.0f, LightModel::Brightness);
    lights.setData(lights.index(0), 22.0f, LightModel::AttenuationRadius);

    program = new QOpenGLShaderProgram();
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 1/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Geometry, "Assets/Lesson 1/Shaders/geometry.g");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 1/Shaders/fragment.f");
    program->link();

    model = Model("Assets\\Models\\Temple\\Temple.obj");
    camera.transform().setPosition(22.0f, 10.0f, 11.0f);
    cameraLookAt(camera, QVector3D(-0.6f, -0.7f, -0.4f));
}


void Lesson1Renderer::keyReleaseEvent(QKeyEvent* event)
{
    TutorialRenderer::keyReleaseEvent(event);
    switch (event->key())
    {
        case Qt::Key_M:
        {

        }
        break;
    }
}


Lesson1Renderer::InterpolationType Lesson1Renderer::currentInterpolationType() const
{
    return interpolationType;
}


void Lesson1Renderer::setCurrentInterpolationType(Lesson1Renderer::InterpolationType type)
{
    if (interpolationType != type)
    {
        interpolationType = type;
        emit currentInterpolationTypeChanged(type);
    }
}


void Lesson1Renderer::paint()
{
    TutorialRenderer::paint();
    if (!initialized())
    {
        return;
    }

    glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    paintSkybox(camera.transform());

    if (program && program->isLinked())
    {
        program->bind();
        glClear(GL_DEPTH_BUFFER_BIT);

        updateLightUniforms(program);
        updateCameraUniforms(program, camera.transform());
        updateObjectUniforms(program, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.01f);
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        program->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));

        program->setUniformValue("interpolationType", interpolationType);
        model.draw(program);
        program->release();

        paintGizmos(camera.transform());
        // Not strictly needed for this example, but generally useful for when
        // mixing with raw OpenGL.
        window->resetOpenGLState();
    }
}


void Lesson1Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}
