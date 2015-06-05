#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // QSettings will save the config file in $XDG_CONFIG_HOME/<organization>/<program>.conf
    app.setOrganizationName("dictation_helper");

    MainWindow w;
    w.show();

    return app.exec();
}
