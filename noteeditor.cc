#include "noteeditor.hh"

namespace ik {
namespace app {

NoteEditor::NoteEditor(QWidget *parent, db::NoteStore & store) :
    QDialog(parent),
    m_store(store) {

    setWindowTitle("Edit Note - Planner");

    build_ui();
    connect_signals();
    set_values();

    show();

    qDebug() << "keh?";
}

void NoteEditor::build_ui() {

    m_title_edit = new QLineEdit();

    m_body_edit = new QTextEdit();

    m_save_button = new QPushButton("Save");

    m_cancel_button = new QPushButton("Cancel");

    //
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(new QLabel("Title"));
    layout->addWidget(m_title_edit);

    layout->addWidget(new QLabel("Note"));
    layout->addWidget(m_body_edit);

    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->addWidget(m_save_button);
    button_layout->addWidget(m_cancel_button);

    layout->addItem(button_layout);

    setLayout(layout);

}

void NoteEditor::connect_signals() {

    connect(m_cancel_button, SIGNAL(clicked()),
            this, SLOT(close()));

    connect(m_save_button, SIGNAL(clicked()),
            this, SLOT(on_save_button()));
}

void NoteEditor::set_values() {

    m_title_edit->setText(m_store.title());
    m_body_edit->document()->setPlainText(m_store.body_text());
}

void NoteEditor::capture_values() {

    m_store.set_title(m_title_edit->text());
    m_store.set_body_text(m_body_edit->document()->toPlainText());
}

void NoteEditor::on_save_button() {

    capture_values();

    m_store.save();

    emit update_triggered();
    close();
}

} // namespace app
} // namespace ik
