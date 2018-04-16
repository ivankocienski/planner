
#include "note.hh"

namespace ik {
namespace app {

// - create new note (add to DB)
// - move note around, resize
// - close note
// - restore notes to MDI on app load

Note::Note(const db::NoteStore &ns) : QMdiSubWindow(), m_store(ns) {

  setWindowTitle("Note");
  setWidget(new QTextEdit());
  setAttribute(Qt::WA_DeleteOnClose);

  resize(m_store.width(),
         m_store.height());

  move(m_store.xpos(), m_store.ypos());
}

Note::Note() : QMdiSubWindow() {

  setWindowTitle("Note");
  setWidget(new QTextEdit());
  setAttribute(Qt::WA_DeleteOnClose);
}

void Note::save_to_store(unsigned int stack_pos) {

    m_store.set_pos(
                pos().x(),
                pos().y());

    m_store.set_size(
                size().width(),
                size().height());

    m_store.set_stack_order(stack_pos);
    m_store.save();
}

void Note::create_in_store(unsigned int stack_pos) {

    m_store = db::NoteStore::create(
                pos().x(),
                pos().y(),
                size().width(),
                size().height(),
                "Note",
                stack_pos);
}

void Note::remove_from_store() {
    m_store.delete_record();
}

void Note::moveEvent(QMoveEvent *e) {
    QMdiSubWindow::moveEvent(e);
    //save_to_store();
}

void Note::resizeEvent(QResizeEvent *e) {
    QMdiSubWindow::resizeEvent(e);
    //save_to_store();
}

void Note::closeEvent(QCloseEvent *ev) {
    qDebug() << "Window close";

    ev->accept();
    remove_from_store();
}

// static
void Note::create(QMdiArea *mdi_area) {

    Note * note = new Note();

    mdi_area->addSubWindow(note);
    note->show();

    auto windows = mdi_area->subWindowList();

    note->create_in_store(windows.size());
}

void Note::create(QMdiArea *mdi_area, const db::NoteStore &ns) {

    Note * note = new Note(ns);

    mdi_area->addSubWindow(note);
    note->show();
}

}
}
