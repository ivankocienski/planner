
#include <QtGui>

#include "main-window.hh"
#include "db-common.hh"

int main( int argc, char ** argv ) {

    QCoreApplication::setOrganizationName("iksoft");
    QCoreApplication::setApplicationName("Planner");

    QApplication app( argc, argv );

    ik::db::init_book_connection();

    ik::app::MainWindow win;

    return app.exec();
}
