#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qtwebengineglobal.h>
#include <QLoggingCategory>
#include <QQmlContext>
#include <QDir>
#include "LightModel.h"
#include "TutorialOpenGLView.h"
#include "Lesson1Renderer.h"
#include "Lesson2Renderer.h"
#include "Lesson4Renderer.h"
#include "Lesson5Renderer.h"
#include "Lesson6Renderer.h"
#include "Lesson7Renderer.h"
#include "Lesson8Renderer.h"
#include "Lesson9Renderer.h"

int main(int argc, char *argv[])
{
    qputenv("QSG_RENDER_LOOP", "basic");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QLoggingCategory::setFilterRules("qt.scenegraph.general=true");
    QtWebEngine::initialize();
    qmlRegisterType<TutorialOpenGLView>("OpenGLTutorial", 1, 0, "TutorialOpenGLView");
    qmlRegisterType<Light>("LightModel", 1, 0, "Light");
    qmlRegisterType<LightModel>("LightModel", 1, 0, "LightModel");

    qRegisterMetaType<Lesson1Renderer*>();
    qRegisterMetaType<Lesson2Renderer*>();
    qRegisterMetaType<Lesson4Renderer*>();
    qRegisterMetaType<Lesson5Renderer*>();
    qRegisterMetaType<Lesson6Renderer*>();
    qRegisterMetaType<Lesson7Renderer*>();
    qRegisterMetaType<Lesson8Renderer*>();
    qRegisterMetaType<Lesson9Renderer*>();


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("currentPath", QDir::currentPath());
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
