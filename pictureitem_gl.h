#ifndef PICTUREITEMGL_H
#define PICTUREITEMGL_H

#include "komicviewer_enums.h"

#include <QtCore/qtimer.h>
#include <QtOpenGL/qgl.h>

class PictureItemGL : public QGLWidget
{
    Q_OBJECT

public:
    PictureItemGL( QWidget * parent = 0, Qt::WindowFlags f = 0 );
    QPixmap getPixmap();
    void setZoom(qreal z);
    qreal getZoom();
    void setPixmap(const QPixmap &p);
    void setRotation(qreal r);
    qreal getRotation();
    void setLockMode(LockMode::Mode);
    QVector<qreal> getDefaultZoomSizes();
    LockMode::Mode getLockMode();

private:
    QPoint pointToOrigin(int width, int height);
    void avoidOutOfScreen();
    void drag(const QPoint &pt);
    void beginDrag(const QPoint &pt);
    void endDrag();
    void updateLockMode();
    void ScrollPageHorizontal(int value);
    void ScrollPageVertical(int value);
    void start_timerScrollPage();

    QPixmap bmp;
    qreal zoom;
    qreal rotation;
    QRect boundingRect;
    bool dragging;
    bool rotating;
    bool flagJumpToEnd;
    QPoint dragPoint;
    QPoint translatePoint;
    QVector<qreal> defaultZoomSizes;
    LockMode::Mode lockMode;
    QTimer *timerScrollPage;
//    bool LockDrag;
    //    MiddleClick::Action middleClickAction;
    //    Wheel::Action wheelAction;

protected:
    void paintEvent(QPaintEvent *event);
//    QSize sizeHint();
    void wheelEvent( QWheelEvent* );
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *ev);

signals:
    void pageNext();
    void pagePrevious();
    void toggleFullscreen();
    void zoomChanged();

public slots:
    void zoomIn();
    void zoomOut();
    void fitToScreen();
    void fitWidth();
    void fitHeight();
private slots:
    void on_timerScrollPage_timeout();

};


#endif // PICTUREITEMGL_H