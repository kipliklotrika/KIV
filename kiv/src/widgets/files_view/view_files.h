#ifndef VIEWFILES_H
#define VIEWFILES_H

#include <QComboBox>
#include <QFileSystemModel>
#include <QFutureWatcher>
#include <QLayout>
#include <QListView>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QSplitter>
#ifdef QT5
#include <QtConcurrent/QtConcurrentMap>
#else
#include <QtConcurrentMap>
#endif
#include <QTreeView>
#include <QUrl>

#include "kiv/include/IArchiveExtractor.h"
#include "kiv/src/helper.h"
#include "kiv/src/models/archive_model.h"
#include "kiv/src/models/filesystem_model.h"
#include "kiv/src/settings.h"
#include "kiv/src/widgets/files_view/thumbnailitemdelegate.h"

class ListViewFiles;
class TreeViewFiles;

class ListViewFiles : public QListView
{
    Q_OBJECT

public:
    explicit ListViewFiles(QWidget *parent = 0);

    void setViewMode(const int mode);

protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);

signals:
    void rowsInserted(const QModelIndexList &indexes);
};


class TreeViewFiles : public QTreeView
{
    Q_OBJECT

public:
    explicit TreeViewFiles(QWidget *parent = 0);

protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int end);

signals:
    void rowsInserted(const QModelIndexList &indexes);
};


class ViewFiles : public QWidget
{
    Q_OBJECT

public:
    explicit ViewFiles(IPictureLoader *picture_loader,
                       IArchiveExtractor *archive_extractor,
                       FileSystemModel *model_filesystem,
                       QWidget *parent = 0);
    void setViewMode(const int mode);
    void setCurrentFile(const FileInfo &info);
    void setThumbnailsSize(const QSize &size);
    FileInfo getCurrentFileInfo() const;
    void setModel(QAbstractItemModel *model);
    void setShowThumbnails(const bool b);
    QFileSystemModel* getFilesystemModel() const;
    void saveCurrentFile(const QString &fileName) const;

public slots:
    void pageNext();
    void pagePrevious();
    void setLocationUrl(const QUrl &url);

private:
    class FileListSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        explicit FileListSortFilterProxyModel(QObject *parent = 0)
            : QSortFilterProxyModel(parent)
        {}
    protected:
        bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    };

    class ContainersSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        explicit ContainersSortFilterProxyModel(QObject *parent = 0)
            : QSortFilterProxyModel(parent)
        {}
    protected:
        bool filterAcceptsColumn(int source_column,
                                 const QModelIndex &source_parent) const;
        bool filterAcceptsRow(int source_row,
                              const QModelIndex &source_parent) const;
    };

    class ArchiveDirsSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        explicit ArchiveDirsSortFilterProxyModel(QObject *parent = 0)
            : QSortFilterProxyModel(parent)
        {}

    protected:
        bool filterAcceptsColumn(int source_column,
                                 const QModelIndex &source_parent) const;
        bool filterAcceptsRow(int sourceRow,
                              const QModelIndex &sourceParent) const;
    };

    void initViewItem();
    void showThumbnails();

    IPictureLoader *m_picture_loader;
    IArchiveExtractor *m_archive_extractor;
    FileInfo m_fileinfo_current;
    int m_view_mode;
    bool m_show_thumbnails;
    bool m_flag_opening;
    QSize m_thumb_size;

    QTreeView *m_view_archiveDirs;
    QTreeView *m_view_filesystem;

    TreeViewFiles *m_treeView_files;
    ListViewFiles *m_listView_files;
    QAbstractItemView *m_view_current;

    QPointer<ArchiveModel> m_model_archive_files;
    FileSystemModel *m_model_filesystem;

    /* source model is FileSystemModel or ArchiveModel */
    FileListSortFilterProxyModel *m_proxy_file_list;

    /* source model is FileSystemModel */
    ContainersSortFilterProxyModel *m_proxy_containers;

    /* source model is ArchiveModel */
    ArchiveDirsSortFilterProxyModel *m_proxy_archive_dirs;

    ThumbnailItemDelegate *m_thumbnail_delegate;

    QVBoxLayout *m_layout_files_list;
    QComboBox *m_combobox_sort;

public slots:
    void dirUp();

private slots:
    void on_item_activated(const QModelIndex &index);
    void on_FilesView_currentRowChanged(const QModelIndex &current,
                                        const QModelIndex &previous);
    void on_thumbnail_finished(const QModelIndex &index);
    void on_rows_inserted(const QModelIndexList &indexes);

    void on_archiveDirsView_currentRowChanged(const QModelIndex &current,
                                              const QModelIndex &previous);
    void on_filesystemView_currentRowChanged(const QModelIndex &current,
                                             const QModelIndex &previous);
    void on_combobox_sort_currentIndexChanged(int index);

signals:
    void urlChanged(const QUrl &url);
};

#endif // VIEWFILES_H
