#include "Lesson7Renderer.h"


Lesson7Renderer::Lesson7Renderer()
{
}


Lesson7Renderer::~Lesson7Renderer()
{
    if (initialized())
    {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteRenderbuffers(1, &renderbuffer);
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteTextures(1, &renderedTexture);
        delete program;
        delete outlineProgram;
        model.clear();
    }
}

float Lesson7Renderer::levels() const
{
    return levelsValue;
}

void Lesson7Renderer::setLevels(float levels)
{
    if (levelsValue != levels)
    {
        levelsValue = levels;
        emit levelsChanged(levels);
    }
}

void Lesson7Renderer::initialize()
{
    TutorialRenderer::initialize();
    if (!initialized())
    {
        return;
    }

    //===============
    // Initialize geometry for a simple plane
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    std::vector<float> vertices =
    {
        // Position           Texture coordinates
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
    };
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));

    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    glBindVertexArray(0);

    //===============
    // Initialize FBO and a texture for offscreen rendering
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferSize, framebufferSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize, framebufferSize);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //===============
    // Create initial light
    lights.append(std::make_unique<Light>());
    lights.setData(lights.index(0), "Światło", LightModel::Name);
    lights.setData(lights.index(0), QVector3D(4.0f, -6.0f, 2.0f), LightModel::Position);
    lights.setData(lights.index(0), QVector3D(-1.0f, 0.0f, 0.0f), LightModel::Direction);
    lights.setData(lights.index(0), QVector3D(0.99f, 0.92f, 0.64f), LightModel::Color);
    lights.setData(lights.index(0), Light::PointLight, LightModel::LightType);
    lights.setData(lights.index(0), 1.0f, LightModel::Brightness);
    lights.setData(lights.index(0), 50.0f, LightModel::AttenuationRadius);

    program = new QOpenGLShaderProgram();
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 7/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 7/Shaders/fragment.f");
    program->link();

    outlineProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(outlineProgram, QOpenGLShader::Vertex, "Assets/Lesson 7/Shaders/toonVertex.v");
    loadShaderForProgram(outlineProgram, QOpenGLShader::Fragment, "Assets/Lesson 7/Shaders/toonFragment.f");
    outlineProgram->link();

    setShininess(42.0f);
    model = Model("Assets\\Models\\Temple\\Temple.obj");
    camera.transform().setPosition(-5.0f, 0.0f, 4.0f);
    cameraLookAt(camera, QVector3D(0.0f, 0.0f, 0.0f));
}


void Lesson7Renderer::paint()
{
    TutorialRenderer::paint();
    if (!initialized())
    {
        return;
    }

    if (program && program->isLinked())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, framebufferSize, framebufferSize);
        glDisable(GL_SCISSOR_TEST);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        paintSkybox(camera.transform());
        glClear(GL_DEPTH_BUFFER_BIT);

        program->bind();
        updateLightUniforms(program);
        updateCameraUniforms(program, camera.transform());
        updateObjectUniforms(program, modelTransform, QQuaternion::fromEulerAngles(90.0f, 0.0f, 0.0f), 0.01f);
        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        program->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));
        program->setUniformValue("levels", levelsValue);
        model.draw(program);
        program->release();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    if (outlineProgram && outlineProgram->isLinked())
    {
        glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        glEnable(GL_SCISSOR_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        outlineProgram->bind();
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        outlineProgram->setUniformValue("renderedTexture", 0);

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    paintGizmos(camera.transform());
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    window->resetOpenGLState();
}


void Lesson7Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}
