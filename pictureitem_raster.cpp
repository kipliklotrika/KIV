#include "pictureitem.h"
#include "settings.h"
#include "picture_loader.h"

//#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

PictureItem::PictureItemRaster::PictureItemRaster(PictureItem *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_picItem(parent)
    , m_pixmap(QPixmap())
    , m_pixmap_edited(m_pixmap)
{
}

void PictureItem::PictureItemRaster::setImage(const QImage &img)
{
    this->setUpdatesEnabled(false);

    m_pixmap = QPixmap::fromImage(img);

    m_pixmap_edited = m_pixmap;

    m_picItem->m_boundingRect = QRect(0, 0, m_pixmap.width(), m_pixmap.height());
    if (m_picItem->getLockMode() == LockMode::None)
    {
        m_picItem->setZoom(1);
    }
    m_picItem->setRotation(0);

    m_picItem->afterPixmapLoad();
    m_picItem->updateSize();

    this->setUpdatesEnabled(true);
    this->update();
}

void PictureItem::PictureItemRaster::paintEvent(QPaintEvent *event)
{
    if (m_picItem->isPixmapNull())
    {
        return;
    }

    QPainter p(this);
    p.setClipRect(event->region().boundingRect());
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.fillRect(event->region().boundingRect(), m_picItem->m_color_clear);
    const qreal zoom = m_picItem->getZoom();
    QRectF sourceRect = QRectF(-m_picItem->m_boundingRect.x() / zoom,
                               -m_picItem->m_boundingRect.y() / zoom,
                               event->region().boundingRect().width() / zoom,
                               event->region().boundingRect().height() / zoom);
    QRectF drawRect = QRectF(QPointF(m_picItem->m_offsetX, m_picItem->m_offsetY), event->region().boundingRect().size());
    p.drawPixmap(drawRect, m_pixmap_edited, sourceRect);
    p.end();
}

void PictureItem::PictureItemRaster::setRotation(const qreal r)
{
    if ((int)r % 360 == 0)
    {
        m_pixmap_edited = m_pixmap;
    }
    else
    {
        QTransform tRot;
        tRot.rotate(r);

        const Qt::TransformationMode rotateMode = Qt::SmoothTransformation;
    //    rotateMode = Qt::FastTransformation;

        m_pixmap_edited = m_pixmap.transformed(tRot, rotateMode);
    }

    m_picItem->m_boundingRect.setWidth(m_pixmap_edited.width() * m_picItem->getZoom());
    m_picItem->m_boundingRect.setHeight(m_pixmap_edited.height() * m_picItem->getZoom());
    m_picItem->avoidOutOfScreen();
    m_picItem->updateSize();
    this->update();
}

void PictureItem::PictureItemRaster::setZoom(const qreal current, const qreal previous)
{
    const QPointF p = m_picItem->pointToOrigin((m_pixmap_edited.width() * current), (m_pixmap_edited.height() * current));

    m_picItem->m_boundingRect = QRectF(p.x(), p.y(), (m_pixmap_edited.width() * current), (m_pixmap_edited.height() * current));

    m_picItem->avoidOutOfScreen();
    m_picItem->updateSize();
    this->update();
}
