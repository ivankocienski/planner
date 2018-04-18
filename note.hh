
#pragma once

#include <QtWidgets>

#include "notestore.hh"

namespace ik {
namespace app {

class Note : public QMdiSubWindow {
Q_OBJECT

private:

    db::NoteStore m_store;
    QTextEdit *m_edit;
    QPushButton *m_edit_button;
    
private:
    void build_ui();
    void connect_signals();

    void create_in_store(unsigned int);
    void remove_from_store();

    void closeEvent(QCloseEvent *);

public:

    Note();
    Note(const db::NoteStore&);

    void save_to_store(unsigned int);

    static void create(QMdiArea*);
    static void create(QMdiArea*, const db::NoteStore&);

private slots:

    void on_edit_button();
    void on_edit_changed();
};

}
}
