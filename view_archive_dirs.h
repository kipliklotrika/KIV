#ifndef VIEW_ARCHIVE_DIRS_H
#define VIEW_ARCHIVE_DIRS_H

#include <QtGui/qsortfilterproxymodel.h>
#include <QtGui/qtreeview.h>

class ViewArchiveDirs : public QTreeView
{
    Q_OBJECT
public:
    ViewArchiveDirs();
    void setModel ( QAbstractItemModel * model );

private:

    class MySortFilterProxyModel : public QSortFilterProxyModel
    {
    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    };

    MySortFilterProxyModel* proxy;


};

#endif // VIEW_ARCHIVE_DIRS_H
