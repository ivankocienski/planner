#pragma once

#include <QtWidgets>
#include "notestore.hh"

namespace ik {
namespace app {

class NoteEditor : public QDialog {
Q_OBJECT
private:

    db::NoteStore & m_store;

    QLineEdit *m_title_edit;
    QTextEdit *m_body_edit;
    QPushButton *m_save_button;
    QPushButton *m_cancel_button;

    void build_ui();
    void connect_signals();
    void set_values();
    void capture_values();

public:
    NoteEditor(QWidget*, db::NoteStore &);

signals:

    void update_triggered();

private slots:

    void on_save_button();
};


}
}
