
#include <QtGui>

#include "main-window.hh"
#include "book.hh"

int main( int argc, char ** argv ) {
  QApplication app( argc, argv );

  MainWindow win;

  init_book_connection();

  return app.exec();
}
