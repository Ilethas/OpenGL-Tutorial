#ifndef TUTORIALOPENGLVIEW_H
#define TUTORIALOPENGLVIEW_H
#include <QQuickWindow>
#include <QQuickItem>
#include <Qtimer>
#include <memory>
#include "TutorialRenderer.h"
#include <Windows.h>


class TutorialOpenGLView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString rendererClass READ rendererClass WRITE setRendererClass NOTIFY rendererClassChanged)
    Q_PROPERTY(QObject* optionsReference READ optionsReference WRITE setOptionsReference NOTIFY optionsReferenceChanged)

    public:
        TutorialOpenGLView();

        QString rendererClass() const;
        void setRendererClass(QString renderer);
        QObject* optionsReference() const;
        void setOptionsReference(QObject* options);

    signals:
        void rendererClassChanged();
        void optionsReferenceChanged();

    public slots:
        void sync();
        void cleanup();

    private slots:
        void handleWindowChanged(QQuickWindow* win);
        void handleSizeChanged();

    private:
        const QMetaObject* rendererMetaObject = nullptr;
        QString rendererClassName = "";
        QObject* options = nullptr;
        std::unique_ptr<TutorialRenderer> tutorialRenderer;
        QTimer* timer;

        void resizeViewport();

    protected:
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
        virtual void mouseMoveEvent(QMouseEvent* event) override;

        virtual void keyPressEvent(QKeyEvent* event) override;
        virtual void keyReleaseEvent(QKeyEvent* event) override;
};


#endif // TUTORIALOPENGLVIEW_H
