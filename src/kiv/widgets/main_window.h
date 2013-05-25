#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QToolBar>

#include "models/filesystem_model.h"
#include "picture_loader.h"
#include "widgets/files_view/view_files.h"
#include "widgets/urlnavigator.h"
#include "widgets/picture_item/pictureitem.h"
#include "widgets/zoom_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~MainWindow();

private:
    void createActions();
    void createMenus();
    void connectActions();
    void updateSettings();
    void populateBookmarks();
    void spreadUrl(const QUrl &url);


    FileSystemModel *m_model_filesystem;
    Settings *m_settings;

    ViewFiles *m_view_files;

    QSplitter *m_splitter_main;
    PictureItem *m_picture_item;

    QMenuBar *m_menu_main;

    QToolBar *m_toolbar;
    UrlNavigator *m_urlNavigator;
    ZoomWidget *m_comboBox_zoom;
    QIcon m_windowIcon;

    QMenu *m_menu_bookmarks;
    QMenu *m_menu_history;
    QMenu *m_menu_context_picture;
    QMenu *m_menu_context_bookmark;

    QAction *m_act_bookmark_delete;

    /* File Menu Actions */
    QAction *m_act_open;
    QAction *m_act_save;
    QAction *m_act_pageNext;
    QAction *m_act_pagePrevious;
    QAction *m_act_dirUp;
    QAction *m_act_back;
    QAction *m_act_forward;
    QAction *m_act_refreshPath;
    QAction *m_act_exit;

    /* Bookmarks Menu Actions */
    QAction *m_act_bookmark_add;

    /* Image Menu Actions */
    QAction *m_act_zoomIn;
    QAction *m_act_zoomOut;
    QAction *m_act_zoomReset;
    QAction *m_act_rotateLeft;
    QAction *m_act_rotateRight;
    QAction *m_act_rotateReset;
    QAction *m_act_fitToWindow;
    QAction *m_act_fitToWidth;
    QAction *m_act_fitToHeight;
    QAction *m_act_lockNone;
    QAction *m_act_lockZoom;
    QAction *m_act_lockAutofit;
    QAction *m_act_lockFitWidth;
    QAction *m_act_lockFitHeight;

    /* View Menu Actions */
    QAction *m_act_thumbnails;
    QAction *m_act_mode_list;
    QAction *m_act_mode_details;
    QAction *m_act_mode_icons;
    QAction *m_act_sidebar;
    QAction *m_act_fullscreen;

    /* Options Menu Actions */
    QAction *m_act_largeIcons;
    QAction *m_act_settings;

    /* Help Menu Actions */
    QAction *m_act_webSite;
    QAction *m_act_about;

private slots:

    void updateActions();

    /* File Menu Actions Slots */
    void openFileDialog();
    void saveAs();
    void goBack();
    void goForward();

    /* Bookmark Menu Action Slots */
    void addBookmark();
    void on_bookmark_triggered();
    void deleteBookmark();

    /* Edit Menu Actions Slots */
    void rotateLeft();
    void rotateRight();
    void rotateReset();
    void zoomReset();
    void lockZoom();
    void lockAutofit();
    void lockFitWidth();
    void lockFitHeight();
    void lockNone();

    /* Options Menu Actions Slots */
    void toggleFullscreen(bool);
    void toggleSidebar(bool);
    void toggleLargeIcons(bool b);
    void settingsDialog();
    void toggleShowThumbnails(bool);

    /* Help Menu Actions Slots */
    void about();
    void website();

    /* Directory Toolbar Actions Slots */
    void refreshPath();

    void on_customContextMenuRequested(const QPoint &pos);
    void on_bookmark_customContextMenuRequested(const QPoint &pos);
    void on_view_mode_list_triggered();
    void on_view_mode_details_triggered();
    void on_view_mode_icons_triggered();
    void on_pictureItemMousePress(const QMouseEvent * const event);
    void on_pictureItemMouseDoubleClick(const QMouseEvent * const event);
    void on_pictureItemMouseWheel(const QWheelEvent * const event);
    void on_historyMenuRequested(const QPoint &pos);
    void on_historyMenuTriggered(QAction *action);
    void on_urlHistoryChanged();

    void populateHistoryMenu();
    bool setLocationUrl(const QUrl &url);
    bool openFile(const FileInfo &info);
    bool openFilePath(const QString &path);
    bool openUrl(const QUrl &url);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void showEvent(QShowEvent *event);
};

#endif // MAINWINDOW_H
