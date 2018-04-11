
#include <QtGui>

#include "main-window.hh"
#include "db-common.hh"

int main( int argc, char ** argv ) {
  QApplication app( argc, argv );

  ik::app::MainWindow win;

  ik::db::init_book_connection();

  return app.exec();
}
