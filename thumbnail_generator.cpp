#include "thumbnail_generator.h"

#include <QtGui>
#include <QtCore>

ThumbnailGenerator::ThumbnailGenerator()
{
}

void ThumbnailGenerator::setPath(const QString& path)
{
    this->path = path;
}

QString ThumbnailGenerator::getPath()
{
    return this->path;
}

void ThumbnailGenerator::setLength(const int& length)
{
    this->length = length;
}

int ThumbnailGenerator::getLength()
{
    return this->length;
}

void ThumbnailGenerator::returnThumbnail()
{
    QImageReader image_reader(path);
    int image_width = image_reader.size().width();
    int image_height = image_reader.size().height();
    if (image_width > image_height) {
      image_height = static_cast<double>(length) / image_width * image_height;
      image_width = length;
    } else if (image_width < image_height) {
      image_width = static_cast<double>(length) / image_height * image_width;
      image_height = length;
    } else {
      image_width = length;
      image_height = length;
    }

    image_reader.setScaledSize(QSize(image_width, image_height));
    QIcon icon;
    icon.addPixmap(QPixmap::fromImage(image_reader.read()));
    emit finished(icon);
    return;
}
