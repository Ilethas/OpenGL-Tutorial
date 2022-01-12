#include "Lesson6Renderer.h"


Lesson6Renderer::Lesson6Renderer()
{
}


Lesson6Renderer::~Lesson6Renderer()
{
    if (initialized())
    {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteRenderbuffers(1, &renderbuffer);
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteTextures(1, &renderedTexture);
        glDeleteFramebuffers(1, &framebuffer2);
        glDeleteRenderbuffers(1, &renderbuffer2);
        glDeleteTextures(1, &renderedTexture2);
        glDeleteFramebuffers(1, &framebuffer3);
        glDeleteRenderbuffers(1, &renderbuffer3);
        glDeleteTextures(1, &renderedTexture3);
        glDeleteFramebuffers(1, &framebuffer33);
        glDeleteRenderbuffers(1, &renderbuffer33);
        glDeleteTextures(1, &renderedTexture33);
        delete program;
    }
}

//#define SCR_WIDTH 1280
//#define SCR_HEIGHT 720
#define SCR_WIDTH 2560
#define SCR_HEIGHT 1440
unsigned int blurFBO;
unsigned int colorBuffers[5];
//unsigned int pingpongFBO[2];
//unsigned int pingpongBuffer[2];
unsigned int EBO;
unsigned int VAO;
unsigned int VBO;
void Lesson6Renderer::initialize()
{
    TutorialRenderer::initialize();
    if (!initialized())
    {
        return;
    }

    // set up floating point framebuffer to render scene to

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    float vertices[] = {
        1.0f, 1.0, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0, 0.0f,
        -1.0f, 1.0f, 0.0f,
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //

    glGenFramebuffers(1, &blurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);

    reinitTextures();
    /*glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
        );
    }
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);*/


    //glGenFramebuffers(2, pingpongFBO);


    //mariuszowo
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    std::vector<float> vertices2 =
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
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float), vertices2.data(), GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));

    // Texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    glBindVertexArray(0);
    //
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

    //2
    glGenTextures(1, &renderedTexture2);
    glBindTexture(GL_TEXTURE_2D, renderedTexture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferSize, framebufferSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &renderbuffer2);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer2);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize, framebufferSize);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture2, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer2);

    //3
    glGenTextures(1, &renderedTexture3);
    glBindTexture(GL_TEXTURE_2D, renderedTexture3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferSize, framebufferSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &renderbuffer3);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer3);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize, framebufferSize);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &framebuffer3);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer3);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture3, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer3);

    //33
    glGenTextures(1, &renderedTexture33);
    glBindTexture(GL_TEXTURE_2D, renderedTexture33);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebufferSize, framebufferSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &renderbuffer33);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer33);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebufferSize, framebufferSize);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &framebuffer33);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer33);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture33, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer33);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    //loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 1/Shaders/vertex.v");
    //loadShaderForProgram(program, QOpenGLShader::Geometry, "Assets/Lesson 1/Shaders/geometry.g");
    //loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 6/Shaders/fragment.f");
    loadShaderForProgram(program, QOpenGLShader::Vertex, "Assets/Lesson 7/Shaders/vertex.v");
    loadShaderForProgram(program, QOpenGLShader::Fragment, "Assets/Lesson 7/Shaders/fragment.f");
    program->link();

    brightProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(brightProgram, QOpenGLShader::Vertex, "Assets/Lesson 6/Shaders/brightFilter.v");
    loadShaderForProgram(brightProgram, QOpenGLShader::Fragment, "Assets/Lesson 6/Shaders/brightFilter.f");
    brightProgram->link();

    blurProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(blurProgram, QOpenGLShader::Vertex, "Assets/Lesson 6/Shaders/blur.v");
    loadShaderForProgram(blurProgram, QOpenGLShader::Fragment, "Assets/Lesson 6/Shaders/blur.f");
    blurProgram->link();

    blurProgram2 = new QOpenGLShaderProgram();
    loadShaderForProgram(blurProgram2, QOpenGLShader::Vertex, "Assets/Lesson 6/Shaders/blur2.v");
    loadShaderForProgram(blurProgram2, QOpenGLShader::Fragment, "Assets/Lesson 6/Shaders/blur2.f");
    blurProgram2->link();

    blendProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(blendProgram, QOpenGLShader::Vertex, "Assets/Lesson 6/Shaders/bloomBlending.v");
    loadShaderForProgram(blendProgram, QOpenGLShader::Fragment, "Assets/Lesson 6/Shaders/bloomBlending.f");
    blendProgram->link();

    model = Model("Assets\\Models\\Temple\\Temple.obj");
    camera.transform().setPosition(22.0f, 10.0f, 11.0f);
    cameraLookAt(camera, QVector3D(-0.6f, -0.7f, -0.4f));
    InterpolationType t = Smooth;
    interpolationType = t;
}

void Lesson6Renderer::keyReleaseEvent(QKeyEvent* event)
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


Lesson6Renderer::InterpolationType Lesson6Renderer::currentInterpolationType() const
{
    return interpolationType;
}


void Lesson6Renderer::setCurrentInterpolationType(Lesson6Renderer::InterpolationType type)
{
    if (interpolationType != type)
    {
        interpolationType = type;
        emit currentInterpolationTypeChanged(type);
    }
}

void Lesson6Renderer::RenderQuad() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool Lesson6Renderer::checkSizeChange() {
    bool changed = false;
    if (oldW - viewportSize.width() < 0.001f) changed = true;
    if (oldH - viewportSize.height() < 0.001f) changed = true;
    oldW = viewportSize.width();
    oldH = viewportSize.height();
    return changed;
}

void Lesson6Renderer::reinitTextures() {
    if (texturesDone == true) glDeleteTextures(5, colorBuffers);
    texturesDone = true;
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBO);
    glGenTextures(5, colorBuffers);
    for (unsigned int i = 0; i < 5; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB16F, viewportSize.width(), viewportSize.height(), 0, GL_RGB, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
        );
    }
    unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
    glDrawBuffers(5, attachments);
}

void Lesson6Renderer::paint()
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

        model.draw(program);
        program->release();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    if (brightProgram && brightProgram->isLinked())
    {
        //glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        //glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        //glEnable(GL_SCISSOR_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        brightProgram->bind();
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        brightProgram->setUniformValue("renderedTexture", 0);
        float threshold = thresholdValue;
        if (!bloomAllowed) threshold = 0;
        brightProgram->setUniformValue("threshold", threshold);

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        brightProgram->release();
    }
    if (blurProgram && blurProgram->isLinked())
    {
        //glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        //glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        //glEnable(GL_SCISSOR_TEST);

        int blu = (int)blurValue;
        if (!bloomAllowed) blu = 0;

        for (int i = 0; i <= blu; ++i) {
            blurProgram->bind();
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer3);
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            if (i == 0)
                glBindTexture(GL_TEXTURE_2D, renderedTexture2);
            else {
                glBindTexture(GL_TEXTURE_2D, renderedTexture33);
            }
            blurProgram->setUniformValue("renderedTexture", 0);

            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            blurProgram->release();
            //////////
            blurProgram2->bind();
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer33);
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderedTexture3);
            blurProgram2->setUniformValue("renderedTexture", 0);

            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            blurProgram2->release();
        }

    }
    if (blendProgram && blendProgram->isLinked())
    {
        glViewport(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        glScissor(viewportPosition.x(), viewportPosition.y(), viewportSize.width(), viewportSize.height());
        glEnable(GL_SCISSOR_TEST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        blendProgram->bind();
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        GLint texLoc;
        texLoc = blendProgram->uniformLocation("image");
        glUniform1i(texLoc, 0);
        texLoc = blendProgram->uniformLocation("bloomBlur");
        glUniform1i(texLoc, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, renderedTexture33);

        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture3);
        blendProgram->setUniformValue("bloomBlur", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        blendProgram->setUniformValue("image", 0);*/

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        blendProgram->release();
    }


    /*float ox = 0.0;//nieudana próba uratowania przesunięcia obrazu
    float oy = 0.0;
    if (brightProgram && brightProgram->isLinked())
    {
        brightProgram->bind();
        float threshold = thresholdValue;
        if (!bloomAllowed) threshold = 0;
        brightProgram->setUniformValue("threshold", threshold);
        brightProgram->setUniformValue("ox", ox);
        brightProgram->setUniformValue("oy", oy);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   //wybieram domyślny bufor
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);  //binduje do niego teksture
        RenderQuad();
        brightProgram->release();
    }*/
    /*if (blurProgram && blurProgram->isLinked())
    {
        blurProgram->bind();
        blurProgram->setUniformValue("ox", ox);
        blurProgram->setUniformValue("oy", oy);
        blurProgram->setUniformValue("size", (int)blurValue);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(
            GL_TEXTURE_2D, colorBuffers[2]
        );
        RenderQuad();
        blurProgram->release();
    }*/
    /*
    if (blendProgram && blendProgram->isLinked())
    {
        blendProgram->bind();
        blendProgram->setUniformValue("ox", ox);
        blendProgram->setUniformValue("oy", oy);
        GLint texLoc;
        texLoc = blendProgram->uniformLocation("image");
        glUniform1i(texLoc, 0);
        texLoc = blendProgram->uniformLocation("bloomBlur");
        glUniform1i(texLoc, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[3]);

        RenderQuad();
        glActiveTexture(GL_TEXTURE0);
        blendProgram->release();
    }*/
    paintGizmos(camera.transform());
    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    window->resetOpenGLState();
}

void Lesson6Renderer::update(float deltaTime)
{
    TutorialRenderer::update(deltaTime);
}

float Lesson6Renderer::blur() const
{
    return blurValue;
}

void Lesson6Renderer::setBlur(float blur)
{
    if (blurValue != blur)
    {
        blurValue = blur;
        emit blurChanged(blur);
    }
}

float Lesson6Renderer::threshold() const
{
    return thresholdValue;
}

void Lesson6Renderer::setThreshold(float threshold)
{
    if (thresholdValue != threshold)
    {
        thresholdValue = threshold;
        emit thresholdChanged(threshold);
    }
}

bool Lesson6Renderer::bloomUsed() const
{
    return bloomAllowed;
}

void Lesson6Renderer::setBloomUsed(bool value)
{
    if (bloomAllowed != value)
    {
        bloomAllowed = value;
        emit bloomUsedChanged(value);
    }
}
