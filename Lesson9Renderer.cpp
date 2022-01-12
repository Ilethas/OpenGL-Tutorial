#include "Lesson9Renderer.h"


Lesson9Renderer::Lesson9Renderer()
{
}


Lesson9Renderer::~Lesson9Renderer()
{
    if (initialized())
    {
        delete program;
        model.clear();
    }
}


void Lesson9Renderer::initialize()
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
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 9/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 9/Shaders/fragment.f");
    program->link();

    vectorsProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Vertex, "Assets/Lesson 9/Shaders/vectorsVertex.v");
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Geometry, "Assets/Lesson 9/Shaders/vectorsGeometry.g");
    loadShaderForProgram(vectorsProgram, QOpenGLShader::Fragment, "Assets/Lesson 9/Shaders/vectorsFragment.f");
    vectorsProgram->link();

    stencilProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(stencilProgram,QOpenGLShader::Vertex,"Assets/Lesson 9/Shaders/stencil_vertex.v");
    loadShaderForProgram(stencilProgram,QOpenGLShader::Fragment,"Assets/Lesson 9/Shaders/stencil_single_color.f");
    stencilProgram->link();

    setShininess(42.0f);
    model = Model("Assets\\Models\\Temple\\Temple.obj");
    cubeModel = Model("Assets\\Models\\Cube\\Cube.obj");
    cubeModel2 = Model("Assets\\Models\\Cube\\Cube.obj");
    camera.transform().setPosition(22.0f, 10.0f, 11.0f);
    cameraLookAt(camera, QVector3D(-0.6f, -0.7f, -0.4f));
}


bool Lesson9Renderer::normalMappingUsed() const
{
    return normalMapsAllowed;
}


void Lesson9Renderer::setNormalMappingUsed(bool value)
{
    if (normalMapsAllowed != value)
    {
        normalMapsAllowed = value;
        emit normalMappingUsedChanged(value);
    }
}

float Lesson9Renderer::gamma() const
{
    return gammaValue;
}

void Lesson9Renderer::setGamma(float gamma)
{
    if (gammaValue != gamma)
    {
        gammaValue = gamma;
        emit gammaChanged(gamma);
    }
}

bool Lesson9Renderer::displayAsColor() const
{
    return displayNormalsAsColors;
}


void Lesson9Renderer::setDisplayAsColor(bool value)
{
    if (displayNormalsAsColors != value)
    {
        displayNormalsAsColors = value;
        emit displayAsColorChanged(value);
    }
}


bool Lesson9Renderer::displayVectors() const
{
    return displayNormalsAsVectors;
}


void Lesson9Renderer::setDisplayVectors(bool value)
{
    if (displayNormalsAsVectors != value)
    {
        displayNormalsAsVectors = value;
        emit displayVectorsChanged(value);
    }
}


void Lesson9Renderer::paint()
{
    TutorialRenderer::paint();
    if (!initialized())
    {
        return;
    }

    glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
    glEnable(GL_SCISSOR_TEST);
    //glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    //create the buffer
    //unsigned int colorBuffer;
    //glGenTextures();
    paintSkybox(camera.transform());

    if (program && program->isLinked())
    {
        program->bind();
        //stencil testing
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);


        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glStencilMask(0x00);
        //skoro zwykly program to powinien zrobic normalShader.use();

        updateLightUniforms(program);
        updateCameraUniforms(program, camera.transform());
        updateObjectUniforms(program, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.01f);
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        program->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));

        program->setUniformValue("normalMapsAllowed", normalMapsAllowed);
        program->setUniformValue("displayNormalsAsColors", displayNormalsAsColors);
        program->setUniformValue("displayNormalsAsVectors", displayNormalsAsVectors);
        program->setUniformValue("gammaCorrectionAllowed", gammaCorrectionAllowed);
        program->setUniformValue("outliningAllowed",outliningAllowed);
        program->setUniformValue("gamma", gammaValue);
        glStencilFunc(GL_ALWAYS,1,0xFF);
        if (outliningAllowed) {

            glStencilMask(0xFF);
        }
        model.draw(program);

        if (!cubeOutliningAllowed) {
            glStencilMask(0x00);
        } else {
            glStencilMask(0xFF);
        }

        updateObjectUniforms(program, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.03f);
        cubeModel.draw(program);

        if (cubeOutlining2Allowed) {
            glStencilMask(0xFF);
        } else {
            glStencilMask(0x00);
        }

        QVector3D secondCubeOffset;
        secondCubeOffset.setX(-15.0f);
        secondCubeOffset.setY(0.1f);
        secondCubeOffset.setZ(12.0f);
        updateObjectUniforms(program, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.03f,secondCubeOffset);
        cubeModel2.draw(program);
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
        cubeModel.draw(vectorsProgram);
        vectorsProgram->release();
    }

    if (stencilProgram && stencilProgram->isLinked()) {
        stencilProgram->bind();

        glStencilFunc(GL_NOTEQUAL,1,0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        updateLightUniforms(stencilProgram);
        updateCameraUniforms(stencilProgram, camera.transform());
        QVector3D stencilOffset;
        stencilOffset.setX(0.2f);
        stencilOffset.setY(-0.3f);
        stencilOffset.setZ(0.1f);
        updateObjectUniforms(stencilProgram, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.01f);
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        stencilProgram->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));

        if (outliningAllowed) {
            model.draw(stencilProgram);
        }


        if (cubeOutliningAllowed) {
            updateObjectUniforms(stencilProgram, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.03f);
            cubeModel.draw(stencilProgram);
        }

        if (cubeOutlining2Allowed) {
            QVector3D secondCubeOffset;
            secondCubeOffset.setX(-15.0f);
            secondCubeOffset.setY(0.1f);
            secondCubeOffset.setZ(12.0f);
            updateObjectUniforms(stencilProgram, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.03f,secondCubeOffset);
            cubeModel2.draw(stencilProgram);
        }

        stencilProgram->release();

        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

    }

    paintGizmos(camera.transform());
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    window->resetOpenGLState();
}


void Lesson9Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}


bool Lesson9Renderer::gammaCorrectionUsed() const
{
    return gammaCorrectionAllowed;
}

void Lesson9Renderer::setGammaCorrectionUsed(bool value)
{
    if (gammaCorrectionAllowed != value)
    {
        gammaCorrectionAllowed = value;
        emit gammaCorrectionUsedChanged(value);
    }
}

bool Lesson9Renderer::outliningUsed() const
{
    return outliningAllowed;
}

void Lesson9Renderer::setOutliningUsed(bool value){
    if (outliningAllowed !=value) {
        outliningAllowed=value;
        emit outliningUsedChanged(value);
    }
}

bool Lesson9Renderer::cubeOutliningUsed() const
{
    return cubeOutliningAllowed;
}

void Lesson9Renderer::setCubeOutliningUsed(bool value) {
    if (cubeOutliningAllowed != value) {
        cubeOutliningAllowed=value;
        emit cubeOutliningUsedChanged(value);
    }
}

bool Lesson9Renderer::cubeOutlining2Used() const
{
    return cubeOutlining2Allowed;
}

void Lesson9Renderer::setCubeOutlining2Used(bool value) {
    if (cubeOutlining2Allowed != value) {
        cubeOutlining2Allowed=value;
        emit cubeOutlining2UsedChanged(value);
    }
}
