#include "TutorialRenderer.h"


TutorialRenderer::TutorialRenderer()
{

}


TutorialRenderer::~TutorialRenderer()
{
    if (isOpenGLInitialized && initialized())
    {
        glDeleteTextures(1, &skyboxCubemap);
        arrowModel.clear();
        simpleCube.clear();
        delete skyboxProgram;
        delete gizmosProgram;
    }
}


void TutorialRenderer::initialize()
{
    bool success = initializeOpenGLFunctions();
    if (success)
    {
        isOpenGLInitialized = true;
    }
    else
    {
        MessageBoxA(reinterpret_cast<HWND>(window->winId()), "Could not initialize OpenGL 3.3 core profile", "Error", MB_ICONWARNING);
        return;
    }

    //===============
    // Create default skybox
    glGenTextures(1, &skyboxCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    QString skyboxPath = "Assets/Textures/Skybox/";
    std::vector<QString> faces =
    {
        "SunSetRight2048.png",
        "SunSetLeft2048.png",
        "SunSetUp2048.png",
        "SunSetDown2048.png",
        "SunSetBack2048.png",
        "SunSetFront2048.png",
    };
    for (int i = 0; i < static_cast<int>(faces.size()); i++)
    {
        QImage image(skyboxPath + faces[i]);
        if (!image.isNull())
        {
            image = image.convertToFormat(QImage::Format_RGBA8888);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image.bits());
        }
        else
        {
            QString message = QString("Failed to load skybox face: ") + skyboxPath + faces[i];
            qWarning(message.toLatin1());
        }
    }

    skyboxProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(skyboxProgram, QOpenGLShader::Vertex, "Assets/Common/skyboxVertex.v");
    loadShaderForProgram(skyboxProgram, QOpenGLShader::Fragment, "Assets/Common/skyboxFragment.f");
    skyboxProgram->link();

    //===============
    // Load shaders for painting gizmos
    gizmosProgram = new QOpenGLShaderProgram();
    loadShaderForProgram(gizmosProgram, QOpenGLShader::Vertex, "Assets/Common/arrowVertex.v");
    loadShaderForProgram(gizmosProgram, QOpenGLShader::Fragment, "Assets/Common/arrowFragment.f");
    gizmosProgram->link();

    //===============
    // Load models
    simpleCube = Model("Assets\\Models\\SimpleCube\\SimpleCube.obj");
    arrowModel = Model("Assets\\Models\\Arrow\\Arrow.obj");
}


void TutorialRenderer::mousePressEvent(QMouseEvent* event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            isLeftMouseButtonPressed = true;
        }
        break;

        case Qt::RightButton:
        {
            isRightMouseButtonPressed = true;
            oldCursorPosition = window->cursor().pos();
            window->setCursor(Qt::BlankCursor);
        }
        break;
    }
}


void TutorialRenderer::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            isLeftMouseButtonPressed = false;
        }
        break;

        case Qt::RightButton:
        {
            isRightMouseButtonPressed = false;
            window->unsetCursor();
        }
        break;
    }
}


void TutorialRenderer::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_UNUSED(event)
}


void TutorialRenderer::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::RightButton)
    {
        QCursor cur = window->cursor();
        cur.setPos(oldCursorPosition);
        window->setCursor(cur);
        window->setCursor(Qt::BlankCursor);

        //===============
        // Rotate camera
        QPoint offset = event->globalPos() - oldCursorPosition;
        cameraYaw += offset.x() * cameraRotationSpeed;
        cameraPitch += offset.y() * cameraRotationSpeed;
        if (cameraPitch > maxPitch)
        {
            cameraPitch = maxPitch;
        }
        else if (cameraPitch < -maxPitch)
        {
            cameraPitch = -maxPitch;
        }

        camera.transform().setRotation(0.0f, 0.0f, cameraYaw);
        camera.transform().rotate(0.0f, cameraPitch, 0.0f);
        window->update();

        //===============
        // Rotate light if in pilot mode
        if (isLightPilotActive && currentlySelectedLight != -1 && currentlySelectedLight < maxLights())
        {
            lights.setLightProperty(currentlySelectedLight, camera.transform().localX(), LightModel::Direction);
        }
    }
}


void TutorialRenderer::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_W:
        {
            cameraMovement.movingForward = true;
        }
        break;

        case Qt::Key_S:
        {
            cameraMovement.movingBackward = true;
        }
        break;

        case Qt::Key_D:
        {
            cameraMovement.movingRight = true;
        }
        break;

        case Qt::Key_A:
        {
            cameraMovement.movingLeft = true;
        }
        break;

        case Qt::Key_E:
        {
            cameraMovement.movingUp = true;
        }
        break;

        case Qt::Key_Q:
        {
            cameraMovement.movingDown = true;
        }
        break;
    }
}


void TutorialRenderer::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_W:
        {
            cameraMovement.movingForward = false;
        }
        break;

        case Qt::Key_S:
        {
            cameraMovement.movingBackward = false;
        }
        break;

        case Qt::Key_D:
        {
            cameraMovement.movingRight = false;
        }
        break;

        case Qt::Key_A:
        {
            cameraMovement.movingLeft = false;
        }
        break;

        case Qt::Key_E:
        {
            cameraMovement.movingUp = false;
        }
        break;

        case Qt::Key_Q:
        {
            cameraMovement.movingDown = false;
        }
        break;
    }
}


void TutorialRenderer::setViewportPosition(const QPoint& position)
{
    viewportPosition = position;
}


void TutorialRenderer::setViewportSize(const QSize& size)
{
    viewportSize = size;
}


void TutorialRenderer::setWindow(QQuickWindow* window)
{
    this->window = window;
}


float TutorialRenderer::maxLights() const
{
    return 25;
}


LightModel* TutorialRenderer::lightModel()
{
    return &lights;
}


bool TutorialRenderer::lightPilotMode() const
{
    return isLightPilotActive;
}


void TutorialRenderer::setLightPilotMode(bool active)
{
    if (isLightPilotActive != active)
    {
        isLightPilotActive = active;
        emit lightPilotModeChanged(active);

        if (isLightPilotActive)
        {
            cameraLookAtLight(currentlySelectedLight);
        }
    }
}


bool TutorialRenderer::currentLight() const
{
    return currentlySelectedLight;
}


void TutorialRenderer::setCurrentLight(int index)
{
    if (currentlySelectedLight != index)
    {
        currentlySelectedLight = index;
        emit currentLightChanged(index);

        if (isLightPilotActive)
        {
            cameraLookAtLight(currentlySelectedLight);
        }
    }
}


TutorialRenderer::ShadingType TutorialRenderer::currentShadingType() const
{
    return shadingType;
}


void TutorialRenderer::setCurrentShadingType(ShadingType type)
{
    if (shadingType != type)
    {
        shadingType = type;
        emit currentShadingTypeChanged(type);
    }
}


float TutorialRenderer::shininess() const
{
    return phongShininess;
}


void TutorialRenderer::setShininess(float shininess)
{
    if (phongShininess != shininess)
    {
        phongShininess = shininess;
        emit shininessChanged(shininess);
    }
}


void TutorialRenderer::paint()
{
    if (!isRendererInitialized)
    {
        isRendererInitialized = true;
        initialize();
    }
}


void TutorialRenderer::updateRenderer()
{
    update(lastUpdateTime.elapsed() / 1000.0f);
    if (window)
    {
        window->update();
    }
    lastUpdateTime.start();
}


void TutorialRenderer::update(float deltaTime)
{
    //===============
    // Move camera
    QVector3D offset = cameraMovement.movementVector();
    float offsetZ = offset.z();

    offset.setZ(0.0f);
    offset = camera.transform().rotation().rotatedVector(offset);
    offset.setZ(offset.z() + offsetZ);

    camera.transform().move(offset * deltaTime * cameraMovementSpeed);

    //===============
    // Move light if in pilot mode
    if (isLightPilotActive && currentlySelectedLight != -1 && currentlySelectedLight < maxLights())
    {
        lights.setLightProperty(currentlySelectedLight, camera.transform().position(), LightModel::Position);
    }
}


void TutorialRenderer::paintSkybox(Transform& cameraTransform)
{
    if (skyboxProgram && skyboxProgram->isLinked())
    {
        skyboxProgram->bind();
        QQuaternion rotation = cameraTransform.rotation();
        skyboxProgram->setUniformValue("cameraRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());

        float aspectRatio = viewportSize.width() / static_cast<float>(viewportSize.height());
        skyboxProgram->setUniformValue("projection", Camera::perspective(75.0f, aspectRatio, 0.1f, 500.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(1, skyboxCubemap);
        simpleCube.draw(skyboxProgram);

        skyboxProgram->release();
    }
}


void TutorialRenderer::paintGizmos(Transform& cameraTransform)
{
    if (gizmosProgram && gizmosProgram->isLinked())
    {
        float nearPlane = 0.1f;
        float farPlane = 6.5f;
        int width = 100;
        int height = 100;

        gizmosProgram->bind();
        glViewport(viewportPosition.x(), viewportPosition.y(), width, height);
        glScissor(viewportPosition.x(), viewportPosition.y(), width, height);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        QQuaternion rotation = cameraTransform.rotation();
        gizmosProgram->setUniformValue("cameraRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());

        float halfDist = (farPlane - nearPlane) / 2.0f;
        gizmosProgram->setUniformValue("projection", Camera::orthographic(halfDist, -halfDist, halfDist, -halfDist, nearPlane, farPlane));
        gizmosProgram->setUniformValue("nearPlane", nearPlane);
        gizmosProgram->setUniformValue("farPlane", farPlane);
        gizmosProgram->setUniformValue("offset", QVector3D(0.25f, 0.0f, 0.0f));
        gizmosProgram->setUniformValue("objectScale", arrowTransform.scale());

        float center = (nearPlane + farPlane) / 2.0f;
        //===============
        // Draw x axis arrow
        arrowTransform.setRotation(0.0f, 0.0f, 0.0f);
        rotation = cameraTransform.rotation().conjugated() * arrowTransform.rotation();
        gizmosProgram->setUniformValue("objectRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());

        arrowTransform.setPosition(center, 0.0f, 0.0f);
        gizmosProgram->setUniformValue("objectPosition", arrowTransform.position());

        gizmosProgram->setUniformValue("objectColor", QVector3D(1.0f, 0.25f, 0.25f));
        arrowModel.draw(gizmosProgram);

        //===============
        // Draw y axis arrow
        arrowTransform.setRotation(0.0f, 0.0f, 90.0f);
        rotation = cameraTransform.rotation().conjugated() * arrowTransform.rotation();
        gizmosProgram->setUniformValue("objectRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());

        arrowTransform.setPosition(center, 0.0f, 0.0f);
        gizmosProgram->setUniformValue("objectPosition", arrowTransform.position());

        gizmosProgram->setUniformValue("objectColor", QVector3D(0.35f, 1.0f, 0.35f));
        arrowModel.draw(gizmosProgram);

        //===============
        // Draw z axis arrow
        arrowTransform.setRotation(0.0f, -90.0f, 0.0f);
        rotation = cameraTransform.rotation().conjugated() * arrowTransform.rotation();
        gizmosProgram->setUniformValue("objectRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());

        arrowTransform.setPosition(center, 0.0f, 0.0f);
        gizmosProgram->setUniformValue("objectPosition", arrowTransform.position());

        gizmosProgram->setUniformValue("objectColor", QVector3D(0.25f, 0.4f, 1.0f));
        arrowModel.draw(gizmosProgram);

        gizmosProgram->release();
    }
}


void TutorialRenderer::cameraLookAt(Camera& camera, QVector3D direction)
{
    direction = direction.normalized();
    QVector3D up = QVector3D(0.0f, 0.0f, 1.0f);
    if (qFuzzyCompare(QVector3D::dotProduct(direction, up), 1.0f))
    {
        up = QVector3D(1.0f, 0.0f, 0.0f);
    }
    camera.transform().lookAt(direction, up);

    //===============
    // Update internal camera pitch and yaw
    QQuaternion rotation = camera.transform().rotation();
    float roll, pitch, yaw;

    rotation.getEulerAngles(&roll, &pitch, &yaw);
    cameraYaw = yaw;

    rotation = QQuaternion::fromEulerAngles(0.0f, 0.0f, -yaw) * rotation;
    rotation.getEulerAngles(&roll, &pitch, &yaw);
    cameraPitch = pitch;
}


void TutorialRenderer::cameraLookAtLight(int lightIndex)
{
    if (lightIndex != -1 && lightIndex < maxLights())
    {
        QVector3D direction = lights.getLightProperty(lightIndex, LightModel::Direction).value<QVector3D>().normalized();

        cameraLookAt(camera, direction);
        camera.transform().setPosition(lights.getLightProperty(lightIndex, LightModel::Position).value<QVector3D>());
    }
}


void TutorialRenderer::updateLightUniforms(QOpenGLShaderProgram* program)
{
    for (int i = 0; i < maxLights(); i++)
    {
        std::string name = "lights[" + std::to_string(i) + "].";
        if (i < lights.rowCount())
        {
            program->setUniformValue((name + "active").c_str(), lights.getLightProperty(i, LightModel::Visibility).value<bool>());
            program->setUniformValue((name + "lightType").c_str(), lights.getLightProperty(i, LightModel::LightType).value<Light::LightType>());
            program->setUniformValue((name + "position").c_str(), lights.getLightProperty(i, LightModel::Position).value<QVector3D>());

            QVector3D direction = lights.getLightProperty(i, LightModel::Direction).value<QVector3D>();
            program->setUniformValue((name + "direction").c_str(), direction);

            QColor color = lights.getLightProperty(i, LightModel::Color).value<QColor>();
            program->setUniformValue((name + "color").c_str(), color.redF(), color.greenF(), color.blueF());
            program->setUniformValue((name + "brightness").c_str(), lights.getLightProperty(i, LightModel::Brightness).value<float>());
            program->setUniformValue((name + "attenuationRadius").c_str(), lights.getLightProperty(i, LightModel::AttenuationRadius).value<float>());

            program->setUniformValue((name + "innerConeAngle").c_str(), qDegreesToRadians(lights.getLightProperty(i, LightModel::InnerConeAngle).value<float>()));
            program->setUniformValue((name + "outerConeAngle").c_str(), qDegreesToRadians(lights.getLightProperty(i, LightModel::OuterConeAngle).value<float>()));
        }
        else
        {
            program->setUniformValue((name + "active").c_str(), false);
        }
    }

    program->setUniformValue("shadingType", shadingType);
    program->setUniformValue("shininess", phongShininess);
}


void TutorialRenderer::updateCameraUniforms(QOpenGLShaderProgram* program, Transform &cameraTransform)
{
    QQuaternion rotation = cameraTransform.rotation();
    program->setUniformValue("cameraPosition", cameraTransform.position());
    program->setUniformValue("cameraRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());
}


void TutorialRenderer::updateObjectUniforms(QOpenGLShaderProgram* program, Transform &objectTransform, QQuaternion rotationOffset, float additionalScale, QVector3D offset)
{
    QQuaternion rotation = objectTransform.rotation() * rotationOffset;
    program->setUniformValue("objectPosition", objectTransform.position() + offset);
    program->setUniformValue("objectRotation", rotation.x(), rotation.y(), rotation.z(), rotation.scalar());
    program->setUniformValue("objectScale", objectTransform.scale() * additionalScale);
}


void TutorialRenderer::loadShaderForProgram(QOpenGLShaderProgram* program, QOpenGLShader::ShaderTypeBit type, QString fileName)
{
    QFile shaderFile(fileName);
    if (!shaderFile.open(QIODevice::ReadOnly))
    {
        QString message = "Failed to load shader: ";
        message += fileName;
        qWarning(message.toLatin1());
    }
    else
    {
        QString code = shaderFile.readAll();
        shaderFile.close();
        program->addCacheableShaderFromSourceCode(type, code);
    }
}


bool TutorialRenderer::initialized() const
{
    return isRendererInitialized && isOpenGLInitialized;
}
