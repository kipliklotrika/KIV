#include <QApplication>

#include "kiv/src/archiveextractor.h"
#include "kiv/src/picture_loader.h"

#include "kiv/src/widgets/main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("KIV");
    app.setOrganizationName("KIV");
    app.setApplicationVersion("0.4");

    QDir appdir(QCoreApplication::applicationDirPath());
    QIcon::setThemeSearchPaths(QStringList(QIcon::themeSearchPaths())
                               << appdir.path());
    const QString GENERIC_ICON_TO_CHECK = "go-next";
    if (!QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK)) {
        /* If there is no default working icon theme then we should
           use an icon theme that we provide via a icons folder
           This case happens under Windows and Mac OS X
           This does not happen under GNOME or KDE */
        const QString FALLBACK_ICON_THEME = "default";
        QIcon::setThemeName(FALLBACK_ICON_THEME);
    }

    const ArchiveExtractor ae;
    const PictureLoader pl(&ae);
    MainWindow w(&pl, &ae);
    w.showMaximized();
    return app.exec();
}
