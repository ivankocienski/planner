
#include "note.hh"

namespace ik {
namespace app {

Note::Note() : QMdiSubWindow() {

  setWindowTitle("Note");
  setWidget(new QTextEdit());
  setAttribute(Qt::WA_DeleteOnClose);
}

}
}
