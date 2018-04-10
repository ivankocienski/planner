
#include "note.hh"

Note::Note() : QMdiSubWindow() {

  setWindowTitle("Note");
  setWidget(new QTextEdit());
  setAttribute(Qt::WA_DeleteOnClose);
}

