#include "Lesson8Renderer.h"


Lesson8Renderer::Lesson8Renderer()
{
    gameTime.start();
}


Lesson8Renderer::~Lesson8Renderer()
{
    if (initialized())
    {
        delete program;
        model.clear();
    }
}


void Lesson8Renderer::initialize()
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
    lights.setData(lights.index(0), QVector3D(0.45f, 1.0f, -0.8f), LightModel::Direction);
    lights.setData(lights.index(0), QVector3D(1.0f, 1.0f, 1.0f), LightModel::Color);
    lights.setData(lights.index(0), Light::DirectionalLight, LightModel::LightType);
    lights.setData(lights.index(0), 1.0f, LightModel::Brightness);

    program = new QOpenGLShaderProgram();
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 8/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 8/Shaders/fragment.f");
    program->link();

    setShininess(10.0f);
    model = Model("Assets\\Models\\Water\\Water.obj");
    camera.transform().setPosition(0.0f, 0.0f, 20.0f);
    cameraLookAt(camera, QVector3D(-9.0f, -20.0f, 0.0f));
    cameraMovementSpeed = 80.0f;
}


void Lesson8Renderer::paint()
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
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        program->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 1500.0f));

        program->setUniformValue("numberOfWaves", 6);
        program->setUniformValue("waveSteepness[0]", 0.65f);
        program->setUniformValue("waveAmplitude[0]", 16.0f);
        program->setUniformValue("waveLength[0]", 350.0f);
        program->setUniformValue("waveSpeed[0]", 8.0f);
        program->setUniformValue("waveDirection[0]", QVector2D(1.0f, 1.0f));

        program->setUniformValue("waveSteepness[1]", 0.65f);
        program->setUniformValue("waveAmplitude[1]", 16.0f);
        program->setUniformValue("waveLength[1]", 350.0f);
        program->setUniformValue("waveSpeed[1]", 8.0f);
        program->setUniformValue("waveDirection[1]", QVector2D(-1.0f, 0.25f));

        program->setUniformValue("waveSteepness[2]", 0.45f);
        program->setUniformValue("waveAmplitude[2]", 8.0f);
        program->setUniformValue("waveLength[2]", 175.0f);
        program->setUniformValue("waveSpeed[2]", 12.0f);
        program->setUniformValue("waveDirection[2]", QVector2D(6.0f, -0.25f));

        program->setUniformValue("waveSteepness[3]", 0.25f);
        program->setUniformValue("waveAmplitude[3]", 4.5f);
        program->setUniformValue("waveLength[3]", 175.0f);
        program->setUniformValue("waveSpeed[3]", 12.0f);
        program->setUniformValue("waveDirection[3]", QVector2D(0.5f, -0.25f));

        program->setUniformValue("waveSteepness[4]", 0.25f);
        program->setUniformValue("waveAmplitude[4]", 2.0f);
        program->setUniformValue("waveLength[4]", 50.0f);
        program->setUniformValue("waveSpeed[4]", 5.0f);
        program->setUniformValue("waveDirection[4]", QVector2D(-0.75f, -0.45f));

        program->setUniformValue("waveSteepness[5]", 0.5f);
        program->setUniformValue("waveAmplitude[5]", 12.0f);
        program->setUniformValue("waveLength[5]", 350.0f);
        program->setUniformValue("waveSpeed[5]", 7.0f);
        program->setUniformValue("waveDirection[5]", QVector2D(0.75f, 0.25f));

        program->setUniformValue("time", gameTime.elapsed() / 1000.0f);

        updateObjectUniforms(program, modelTransform);
        model.draw(program);
        float waterSize = 400.0f;
        int bounds = 3;
        for (int i = -bounds; i <= bounds; i++)
        {
            for (int j = -bounds; j <= bounds; j++)
            {
                updateObjectUniforms(program, modelTransform, QQuaternion(1.0f, 0.0f, 0.0f, 0.0f), 1.0f, QVector3D(i * waterSize, j * waterSize, 0.0f));
                model.draw(program);
            }
        }
        program->release();
    }

    paintGizmos(camera.transform());
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    window->resetOpenGLState();
}


void Lesson8Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}
