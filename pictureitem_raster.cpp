#include "pictureitem_raster.h"
#include "settings.h"

//#include <QtCore/qdebug.h>
#include <QtGui/qpainter.h>
#include <QtGui/qevent.h>

PictureItemRaster::PictureItemRaster( PictureItemShared* pis, QWidget * parent, Qt::WindowFlags f )
{
    this->pis = pis;
    connect(pis, SIGNAL(pixmapChanged()), this, SLOT(setPixmap()));
    connect(pis, SIGNAL(zoomChanged()), this, SLOT(setZoom()));
    setCursor(Qt::OpenHandCursor);
//    LockDrag = false;
}

void PictureItemRaster::setPixmap()
{

    this->pixmap_edited = pis->getPixmap();

    pis->boundingRect = QRect( 0, 0, pis->getPixmap().width(), pis->getPixmap().height() );
    if(pis->getLockMode() != LockMode::Zoom)
    {
        pis->setZoom(1);
        emit zoomChanged();
    }
    pis->rotation = 0.0;

    pis->afterPixmapLoad();

    update();
}


void PictureItemRaster::paintEvent( QPaintEvent *event )
{
    if( pis->isPixmapNull() ) return;

//    QTime myTimer;
//    myTimer.start();

    QPainter p( this );
    p.setClipRect( event->region().boundingRect() );
    p.setRenderHint( QPainter::SmoothPixmapTransform );

    qreal zoom = pis->getZoom();
    QRectF sourceRect = QRectF( -pis->boundingRect.x() / zoom,
                                -pis->boundingRect.y() / zoom,
                                event->region().boundingRect().width() / zoom,
                                event->region().boundingRect().height() / zoom );

    QRectF drawRect = (QRectF)event->region().boundingRect();

    if( drawRect.width()  > ( this->pixmap_edited.width() * pis->getZoom() ) )
    {
        drawRect.moveLeft( (drawRect.width() - (this->pixmap_edited.width() * zoom) ) / 2 );
    }
    if( drawRect.height()  > ( this->pixmap_edited.height() * pis->getZoom() ) )
    {
        drawRect.moveTop( ( drawRect.height() - (this->pixmap_edited.height() * zoom  ) ) / 2 );
    }

    p.drawPixmap( drawRect, this->pixmap_edited, sourceRect );
    p.end();

//    qDebug() << "Paint: " << myTimer.elapsed();
}


//Region Rotation

void PictureItemRaster::setRotation( qreal r )
{
    if( pis->isPixmapNull() ) return;

//    QTime myTimer;
//    myTimer.start();

    pis->rotation = r;

    if( (int)pis->rotation % 360 == 0 )
    {
        this->pixmap_edited = pis->getPixmap();
    }
    else
    {
        QTransform tRot;
        tRot.rotate( pis->rotation );

        Qt::TransformationMode rotateMode;
        rotateMode = Qt::SmoothTransformation;
    //    rotateMode = Qt::FastTransformation;

        this->pixmap_edited = pis->getPixmap().transformed( tRot, rotateMode );
    }

    pis->boundingRect.setWidth( this->pixmap_edited.width() * pis->getZoom() );
    pis->boundingRect.setHeight( this->pixmap_edited.height() * pis->getZoom() );
    pis->avoidOutOfScreen();
    update();

//    qDebug() << "Rotate: " << myTimer.elapsed();

}


//End Region Rotation



//Region Zoom

void PictureItemRaster::setZoom()
{
    if( pis->isPixmapNull() ) return;

    qreal z = pis->getZoom();

    QPointF p = pis->pointToOrigin( (this->pixmap_edited.width() * z), (this->pixmap_edited.height() * z) );

    pis->boundingRect = QRectF( p.x(), p.y(), (this->pixmap_edited.width() * z), (this->pixmap_edited.height() * z ) );

    pis->avoidOutOfScreen();
    this->update();

    emit this->zoomChanged();
}

//End Region Zoom



//Region Drag


