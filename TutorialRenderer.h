#ifndef TUTORIALRENDERER_H
#define TUTORIALRENDERER_H
#include <QObject>
#include <QQuickWindow>
#include <Qtime>
#include <QDir>
#include <QtMath>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "Camera.h"
#include "Model.h"
#include "MovementSanitizer.h"
#include "LightModel.h"


class TutorialRenderer : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
    Q_PROPERTY(LightModel* lightModel READ lightModel)
    Q_PROPERTY(float maxLights READ maxLights)
    Q_PROPERTY(bool lightPilotMode READ lightPilotMode WRITE setLightPilotMode NOTIFY lightPilotModeChanged)
    Q_PROPERTY(int currentLight READ currentLight WRITE setCurrentLight NOTIFY currentLightChanged)
    Q_PROPERTY(ShadingType currentShadingType READ currentShadingType WRITE setCurrentShadingType NOTIFY currentShadingTypeChanged)
    Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)

    public:
        enum ShadingType
        {
            Phong,
            BlinnPhong
        };
        Q_ENUM(ShadingType)

        TutorialRenderer();
        virtual ~TutorialRenderer();

        virtual void initialize();

        virtual void mousePressEvent(QMouseEvent*);
        virtual void mouseReleaseEvent(QMouseEvent*);
        virtual void mouseDoubleClickEvent(QMouseEvent*);
        virtual void mouseMoveEvent(QMouseEvent*);
        virtual void keyPressEvent(QKeyEvent*);
        virtual void keyReleaseEvent(QKeyEvent*);

        void setViewportPosition(const QPoint& position);
        void setViewportSize(const QSize& size);
        void setWindow(QQuickWindow* window);

        virtual QString getOptionsUrl() const { return ""; }
        virtual QString getDescriptionUrl() const { return ""; }
        virtual float maxLights() const;
        LightModel* lightModel();

        bool lightPilotMode() const;
        void setLightPilotMode(bool active);

        bool currentLight() const;
        void setCurrentLight(int index);

        ShadingType currentShadingType() const;
        void setCurrentShadingType(ShadingType type);

        float shininess() const;
        void setShininess(float shininess);

    signals:
        void lightPilotModeChanged(bool active);
        void currentLightChanged(int index);
        void currentShadingTypeChanged(ShadingType type);
        void shininessChanged(float shininess);

    public slots:
        virtual void paint();
        void updateRenderer();

    protected:
        QPoint viewportPosition;
        QSize viewportSize;
        QQuickWindow* window = nullptr;

        QPoint oldCursorPosition;
        bool isLeftMouseButtonPressed = false;
        bool isRightMouseButtonPressed = false;

        float cameraYaw = 0.0f;
        float cameraPitch = 0.0f;
        float maxPitch = 90.0f;
        Camera camera;
        MovementSanitizer cameraMovement;
        float cameraRotationSpeed = 0.3f;
        float cameraMovementSpeed = 15.0f;

        LightModel lights;
        ShadingType shadingType = Phong;
        float phongShininess = 100.0f;

        bool isLightPilotActive = false;
        int currentlySelectedLight = -1;

        unsigned int skyboxCubemap;
        Model simpleCube;
        QOpenGLShaderProgram* skyboxProgram = nullptr;

        Model arrowModel;
        Transform arrowTransform;
        QOpenGLShaderProgram* gizmosProgram = nullptr;

        virtual void update(float deltaTime);
        void paintSkybox(Transform& cameraTransform);
        void paintGizmos(Transform& cameraTransform);
        void cameraLookAt(Camera& camera, QVector3D direction);
        void cameraLookAtLight(int lightIndex);

        void updateLightUniforms(QOpenGLShaderProgram* program);
        void updateCameraUniforms(QOpenGLShaderProgram* program, Transform& cameraTransform);
        void updateObjectUniforms(QOpenGLShaderProgram* program, Transform& objectTransform,
                                  QQuaternion rotationOffset = QQuaternion(1.0f, 0.0f, 0.0f, 0.0f),
                                  float additionalScale = 1.0f,
                                  QVector3D offset = QVector3D(0.0f, 0.0f, 0.0f));

        void loadShaderForProgram(QOpenGLShaderProgram* program, QOpenGLShader::ShaderTypeBit type, QString fileName);
        bool initialized() const;

    private:
        bool isRendererInitialized = false;
        bool isOpenGLInitialized = false;
        QTime lastUpdateTime;

};


#endif // TUTORIALRENDERER_H
