#include "TutorialOpenGLView.h"
#include "TutorialRenderer.h"


TutorialOpenGLView::TutorialOpenGLView()
{
    connect(this, &QQuickItem::windowChanged, this, &TutorialOpenGLView::handleWindowChanged);
    connect(this, &QQuickItem::widthChanged, this, &TutorialOpenGLView::handleSizeChanged);
    connect(this, &QQuickItem::heightChanged, this, &TutorialOpenGLView::handleSizeChanged);

    setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
    forceActiveFocus();

    timer = new QTimer(this);
    timer->start(10);
}


void TutorialOpenGLView::setRendererClass(QString rendererClass)
{
    rendererClassName = rendererClass + '*';

    int id = QMetaType::type(rendererClassName.toLatin1());
    if (id != QMetaType::UnknownType)
    {
        rendererMetaObject = QMetaType::metaObjectForType(id);
    }

    tutorialRenderer.reset();
}


QString TutorialOpenGLView::rendererClass() const
{
    return rendererClassName;
}


void TutorialOpenGLView::setOptionsReference(QObject* options)
{
    this->options = options;
}


QObject* TutorialOpenGLView::optionsReference() const
{
    return options;
}


void TutorialOpenGLView::sync()
{
    if (!tutorialRenderer && rendererMetaObject != nullptr)
    {
        if (options)
        {
            options->setProperty("lessonOptionsUrl", "");
            options->setProperty("lessonDescriptionUrl", "");
        }
        tutorialRenderer = std::unique_ptr<TutorialRenderer>(static_cast<TutorialRenderer*>(rendererMetaObject->newInstance()));

        if (options)
        {
            options->setProperty("lessonOptionsUrl", tutorialRenderer->getOptionsUrl());
            options->setProperty("lessonDescriptionUrl", tutorialRenderer->getDescriptionUrl());

            QVariant variant = options->property("optionsControl");
            if (variant.canConvert<QObject*>())
            {
                QObject* optionsControl = qvariant_cast<QObject*>(variant);
                QMetaObject::invokeMethod(optionsControl, "connectToRenderer",
                                          Q_ARG(QVariant, QVariant::fromValue<QObject*>(tutorialRenderer.get())));
            }
        }

        connect(window(), &QQuickWindow::afterRendering, tutorialRenderer.get(), &TutorialRenderer::paint, Qt::DirectConnection);
        connect(timer, &QTimer::timeout, tutorialRenderer.get(), &TutorialRenderer::updateRenderer);

        resizeViewport();
        tutorialRenderer->setWindow(window());
    }
}

void TutorialOpenGLView::cleanup()
{
    if (tutorialRenderer)
    {
        if (options)
        {
            options->setProperty("lessonOptionsUrl", "");
            options->setProperty("lessonDescriptionUrl", "");
        }
        tutorialRenderer.reset();
    }
}


void TutorialOpenGLView::handleWindowChanged(QQuickWindow* win)
{
    if (win)
    {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &TutorialOpenGLView::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &TutorialOpenGLView::cleanup, Qt::DirectConnection);

        win->setClearBeforeRendering(false);
    }
}


void TutorialOpenGLView::handleSizeChanged()
{
    resizeViewport();
}


void TutorialOpenGLView::resizeViewport()
{
    if (tutorialRenderer)
    {
        QPointF viewportPosition = mapToScene(QPointF(0, 0)) + QPointF(0, window()->height() - height());
        QSizeF viewportSize = QSizeF(width(), height());

        tutorialRenderer->setViewportPosition(QPoint(viewportPosition.x(), viewportPosition.y()));
        tutorialRenderer->setViewportSize(QSize(viewportSize.width(), viewportSize.height()));
    }
}


void TutorialOpenGLView::mousePressEvent(QMouseEvent* event)
{
    forceActiveFocus();
    if (tutorialRenderer)
    {
        tutorialRenderer->mousePressEvent(event);
    }
}


void TutorialOpenGLView::mouseReleaseEvent(QMouseEvent* event)
{
    if (tutorialRenderer)
    {
        tutorialRenderer->mouseReleaseEvent(event);
    }
}


void TutorialOpenGLView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (tutorialRenderer)
    {
        tutorialRenderer->mouseDoubleClickEvent(event);
    }
}


void TutorialOpenGLView::mouseMoveEvent(QMouseEvent* event)
{
    if (tutorialRenderer)
    {
        tutorialRenderer->mouseMoveEvent(event);
    }
}


void TutorialOpenGLView::keyPressEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        if (tutorialRenderer)
        {
            tutorialRenderer->keyPressEvent(event);
        }
    }
}


void TutorialOpenGLView::keyReleaseEvent(QKeyEvent* event)
{
    if (!event->isAutoRepeat())
    {
        if (tutorialRenderer)
        {
            tutorialRenderer->keyReleaseEvent(event);
        }
    }
}
