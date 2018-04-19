
#include "note.hh"
#include "noteeditor.hh"

namespace ik {
namespace app {

// - create new note (add to DB)
// - move note around, resize
// - close note
// - restore notes to MDI on app load

Note::Note(const db::NoteStore &ns) : QMdiSubWindow(), m_store(ns) {

    setAttribute(Qt::WA_DeleteOnClose);

    build_ui();

    resize(m_store.width(),
           m_store.height());

    move(m_store.xpos(), m_store.ypos());

    connect_signals();

    // load widget values from NoteStore
    update_ui_values_from_store();
}

Note::Note() : QMdiSubWindow() {

    setWindowTitle("New Note");
    setAttribute(Qt::WA_DeleteOnClose);

    build_ui();
    connect_signals();
    update_ui_values_from_store();
}

db::NoteStore & Note::store() {
    return m_store;
}

void Note::build_ui() {
    m_edit = new QTextEdit();
    m_edit->setReadOnly(true);

    setWidget(m_edit);
}

void Note::connect_signals() {
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
                "New Note",
                stack_pos,
                "");
}

void Note::remove_from_store() {
    m_store.delete_record();
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

// static
void Note::create(QMdiArea *mdi_area, const db::NoteStore &ns) {

    Note * note = new Note(ns);

    mdi_area->addSubWindow(note);
    note->show();
}

void Note::update_ui_values_from_store() {

    setWindowTitle(m_store.title());
    m_edit->setText(m_store.body_text());
}

}
}
