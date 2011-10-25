#include "pictureitem.h"

#include <QtGui/qevent.h>

PictureItem::PictureItem(bool opengl, QWidget *parent, Qt::WindowFlags f)
{
    this->opengl = opengl;
    setCursor(Qt::OpenHandCursor);
    picItemShared = new PictureItemShared();
    connect(picItemShared, SIGNAL(updateCursor(Qt::CursorShape)), this, SLOT(setMouseCursor(Qt::CursorShape)));
    connect(picItemShared, SIGNAL(update()), this, SLOT(update()));

    QVBoxLayout *vboxMain = new QVBoxLayout(this);
    vboxMain->setSpacing(0);
    vboxMain->setMargin(0);

    initPictureItem();

    this->setLayout(vboxMain);
}


void PictureItem::initPictureItem()
{
    if (this->opengl)
    {
        imageDisplayGL = new PictureItemGL(picItemShared);
        connect(picItemShared, SIGNAL(toggleFullscreen()), this, SIGNAL(toggleFullscreen()));
        connect(imageDisplayGL, SIGNAL(zoomChanged()), this, SIGNAL(zoomChanged()));
        connect(imageDisplayGL, SIGNAL(imageChanged()), this, SIGNAL(imageChanged()));
        connect(picItemShared, SIGNAL(pageNext()), this, SIGNAL(pageNext()));
        connect(picItemShared, SIGNAL(pagePrevious()), this, SIGNAL(pagePrevious()));

        imageDisplayRaster = 0;
        this->layout()->addWidget(imageDisplayGL);
    }
    else
    {
        imageDisplayRaster = new PictureItemRaster(picItemShared);
        connect(picItemShared, SIGNAL(toggleFullscreen()), this, SIGNAL(toggleFullscreen()));
        connect(imageDisplayRaster, SIGNAL(zoomChanged()), this, SIGNAL(zoomChanged()));
        connect(imageDisplayRaster, SIGNAL(imageChanged()), this, SIGNAL(imageChanged()));
        connect(picItemShared, SIGNAL(pageNext()), this, SIGNAL(pageNext()));
        connect(picItemShared, SIGNAL(pagePrevious()), this, SIGNAL(pagePrevious()));

        imageDisplayGL = 0;
        this->layout()->addWidget(imageDisplayRaster);
    }
}

void PictureItem::setHardwareAcceleration(bool b)
{
    if (this->opengl != b)
    {
        if (this->opengl)
        {

            delete imageDisplayGL;
        }
        else
        {
            delete imageDisplayRaster;
        }

        this->opengl = b;
        initPictureItem();
    }
}

bool PictureItem::getHardwareAcceleration()
{
    return opengl;
}

bool PictureItem::isPixmapNull()
{
    return picItemShared->isPixmapNull();
}

void PictureItem::setZoom(qreal z)
{
    picItemShared->setZoom(z);
}

qreal PictureItem::getZoom()
{
    return picItemShared->getZoom();
}

void PictureItem::setPixmap(const FileInfo &info)
{
    picItemShared->widgetSize = this->size();

    if (opengl)
    {
        imageDisplayGL->setFile(info);
    }
    else
    {
        imageDisplayRaster->setFile(info);
    }
}

void PictureItem::setRotation(qreal r)
{
    if (this->opengl)
    {
        this->imageDisplayGL->setRotation(r);
    }
    else
    {
        this->imageDisplayRaster->setRotation(r);
    }
}

qreal PictureItem::getRotation()
{
    return this->picItemShared->getRotation();
}

void PictureItem::setLockMode(LockMode::Mode mode)
{
    this->picItemShared->setLockMode(mode);
}

QVector<qreal> PictureItem::getDefaultZoomSizes()
{
    return this->picItemShared->getDefaultZoomSizes();
}

LockMode::Mode PictureItem::getLockMode()
{
    return this->picItemShared->getLockMode();
}

void PictureItem::zoomIn()
{
    this->picItemShared->zoomIn();
}

void PictureItem::zoomOut()
{
    this->picItemShared->zoomOut();
}

void PictureItem::fitToScreen()
{
    this->picItemShared->fitToScreen();
}

void PictureItem::fitWidth()
{
    this->picItemShared->fitWidth();
}

void PictureItem::fitHeight()
{
    this->picItemShared->fitHeight();
}

void PictureItem::mousePressEvent(QMouseEvent *ev)
{
    this->setFocus();

    this->picItemShared->processMousePressEvent(ev);
}

void PictureItem::mouseMoveEvent(QMouseEvent *ev)
{
    this->picItemShared->drag(ev->pos());
}

void PictureItem::mouseReleaseEvent(QMouseEvent *ev)
{
    if (this->picItemShared->dragging && ev->button() == Qt::LeftButton)
    {
        this->picItemShared->endDrag();
        this->setCursor(Qt::OpenHandCursor);
    }
}

//End Region Drag



void PictureItem::resizeEvent(QResizeEvent *)
{
    if (this->picItemShared->isPixmapNull())
    {
        return;
    }

    this->picItemShared->widgetSize = this->size();
    this->picItemShared->avoidOutOfScreen();
    this->picItemShared->updateLockMode();
}


void PictureItem::keyPressEvent(QKeyEvent *ev)
{
    if (this->picItemShared->processKeyPressEvent(ev->key()))
    {
        ev->accept();
    }
}

void PictureItem::wheelEvent(QWheelEvent *event)
{
    this->picItemShared->processWheelEvent(event);
    this->update();
}

void PictureItem::setMouseCursor(Qt::CursorShape cursor)
{
    this->setCursor(cursor);
}
