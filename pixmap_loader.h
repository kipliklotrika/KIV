#ifndef PIXMAPLOADER_H
#define PIXMAPLOADER_H

#include <QtCore/qobject.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qicon.h>

class PixmapLoader : public QObject
{
    Q_OBJECT

public:
    PixmapLoader();
    void setFilePath(const QString &filepath, bool isZip = false, const QString &zipFileName = "");
    QString getFilePath();
    void setThumbnailSize(int length);

private:
    QString filepath;
    bool isZip;
    QString zipFileName;
    int length;
    void loadFromFile();
    void loadFromZip();
    QSize ThumbnailImageSize ( int image_width, int image_height );

public slots:
    void loadPixmap();

signals:
    void finished(QPixmap);
};

#endif // PIXMAPLOADER_H
