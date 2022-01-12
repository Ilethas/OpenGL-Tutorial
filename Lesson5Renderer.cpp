#include "Lesson5Renderer.h"


Lesson5Renderer::Lesson5Renderer()
{
}


Lesson5Renderer::~Lesson5Renderer()
{
    if (initialized())
    {
        delete program;
        model.clear();
    }
}


void Lesson5Renderer::initialize()
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
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 5/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 5/Shaders/fragment.f");
    program->link();

    vectorsProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Vertex, "Assets/Lesson 5/Shaders/vectorsVertex.v");
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Geometry, "Assets/Lesson 5/Shaders/vectorsGeometry.g");
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Fragment, "Assets/Lesson 5/Shaders/vectorsFragment.f");
    vectorsProgram->link();

    setShininess(42.0f);
    model = Model("Assets\\Models\\Temple\\Temple.obj");
    camera.transform().setPosition(22.0f, 10.0f, 11.0f);
    cameraLookAt(camera, QVector3D(-0.6f, -0.7f, -0.4f));
}


bool Lesson5Renderer::normalMappingUsed() const
{
    return normalMapsAllowed;
}


void Lesson5Renderer::setNormalMappingUsed(bool value)
{
    if (normalMapsAllowed != value)
    {
        normalMapsAllowed = value;
        emit normalMappingUsedChanged(value);
    }
}

float Lesson5Renderer::gamma() const
{
    return gammaValue;
}

void Lesson5Renderer::setGamma(float gamma)
{
    if (gammaValue != gamma)
    {
        gammaValue = gamma;
        emit gammaChanged(gamma);
    }
}

float Lesson5Renderer::exposure() const {
    return exposureValue;
}

void Lesson5Renderer::setExposure(float exposure) {
    if (exposureValue != exposure) {
        exposureValue = exposure;
        emit exposureChanged(exposure);
    }
}


bool Lesson5Renderer::displayAsColor() const
{
    return displayNormalsAsColors;
}


void Lesson5Renderer::setDisplayAsColor(bool value)
{
    if (displayNormalsAsColors != value)
    {
        displayNormalsAsColors = value;
        emit displayAsColorChanged(value);
    }
}


bool Lesson5Renderer::displayVectors() const
{
    return displayNormalsAsVectors;
}


void Lesson5Renderer::setDisplayVectors(bool value)
{
    if (displayNormalsAsVectors != value)
    {
        displayNormalsAsVectors = value;
        emit displayVectorsChanged(value);
    }
}


void Lesson5Renderer::paint()
{
    TutorialRenderer::paint();
    if (!initialized())
    {
        return;
    }

    glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
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

        program->setUniformValue("normalMapsAllowed", normalMapsAllowed);
        program->setUniformValue("displayNormalsAsColors", displayNormalsAsColors);
        program->setUniformValue("displayNormalsAsVectors", displayNormalsAsVectors);
        program->setUniformValue("gammaCorrectionAllowed", gammaCorrectionAllowed);
        program->setUniformValue("hdrAllowed",hdrAllowed);
        program->setUniformValue("gamma", gammaValue);
        program->setUniformValue("exposure",exposureValue);
        model.draw(program);
        program->release();
    }

    if (vectorsProgram && vectorsProgram->isLinked() && displayNormalsAsVectors)
    {
        vectorsProgram->bind();
        updateLightUniforms(vectorsProgram);
        updateCameraUniforms(vectorsProgram, camera.transform());
        updateObjectUniforms(vectorsProgram, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.01f);
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        vectorsProgram->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));

        model.draw(vectorsProgram);
        vectorsProgram->release();
    }

    paintGizmos(camera.transform());
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    window->resetOpenGLState();
}


void Lesson5Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}


bool Lesson5Renderer::gammaCorrectionUsed() const
{
    return gammaCorrectionAllowed;
}

void Lesson5Renderer::setGammaCorrectionUsed(bool value)
{
    if (gammaCorrectionAllowed != value)
    {
        gammaCorrectionAllowed = value;
        emit gammaCorrectionUsedChanged(value);
    }
}

bool Lesson5Renderer::hdrUsed() const
{
    return hdrAllowed;
}

void Lesson5Renderer::setHdrUsed(bool value) {
    if (hdrAllowed != value) {
        hdrAllowed = value;
        emit hdrUsedChanged(value);
    }
}


