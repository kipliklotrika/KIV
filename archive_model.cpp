#include "archive_model.h"
#include "komicviewer_enums.h"
#include "system_icons.h"
#include "quazip/quazip.h"
#include "quazip/quazipfile.h"

#include <QtCore/qfile.h>
#include <QtCore/qdebug.h>
#include <QtGui/qimagereader.h>



ArchiveModel::ArchiveModel()
{
}

void ArchiveModel::setArchiveName(const QString &filePath)
{
    this->clear();

//    qDebug() << "ArchiveModel::setArchiveName" << filePath;
    QFile zipFile(filePath);
    QuaZip zip(&zipFile);
    if(!zip.open(QuaZip::mdUnzip))
    {
        qWarning("testRead(): zip.open(): %d", zip.getZipError());
        return;
    }
    zip.setFileNameCodec("UTF-8");

    archive_files = zip.getFileNameList();

    zip.close();
    if(zip.getZipError()!=UNZ_OK) {
        qWarning("testRead(): zip.close(): %d", zip.getZipError());
        return;
    }


    //Populate treeViewFile

    QFileIconProvider fip;
    QStandardItem* root = new QStandardItem();
    root->setData(TYPE_ARCHIVE);
    root->setIcon(fip.icon(QFileInfo(zipFile)));
    root->setText(zipFile.fileName());
    QStandardItem* node = root;

    for(int i=0; i < archive_files.count() ; i++)
    {
        node = root;
        QStringList file_path_parts = archive_files.at(i).split('/');
        for (int j = 0; j < file_path_parts.count(); j++)
        {
            if (file_path_parts.at(j).count() > 0)
            {
                if (j < file_path_parts.count() - 1)
                {
                    node = AddNode(node, file_path_parts.at(j), TYPE_DIR);
                }
                else
                    //if (j == file_path_parts.count() - 1)
                {
                    QFileInfo fi(archive_files.at(i));
                    if(QImageReader::supportedImageFormats().contains(fi.suffix().toLower().toLocal8Bit()))
                    {
//                            qDebug() << fi.completeBaseName() << fi.suffix();
                        node = AddNode(node, file_path_parts.at(j), makeArchiveNumberForItem(i));
                    }
                }
            }
        }
    }

    this->invisibleRootItem()->appendRow(root);

}

QStandardItem* ArchiveModel::AddNode(QStandardItem* node, QString name, int index)
{
    for(int i = 0; i < node->rowCount(); i++)
    {
        if(node->child(i)->text() == name)
        {
            return node->child(i);
        }
    }

    QStandardItem* ntvi = new QStandardItem();
    ntvi->setData(index, ROLE_TYPE);
    ntvi->setText(name);
    if(index == TYPE_DIR)
    {
        ntvi->setIcon(SystemIcons::getDirectoryIcon());
    }
    ntvi->setData(name, Qt::ToolTipRole);
    node->appendRow(ntvi);
    return ntvi;
}
