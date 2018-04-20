
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
    QLabel *m_modified;

private:
    void build_ui();
    void configure_window();
    void connect_signals();

    void create_in_store(unsigned int);
    void remove_from_store();

    void closeEvent(QCloseEvent *);

public:

    Note();
    Note(const db::NoteStore&);

    db::NoteStore & store();

    void save_to_store(unsigned int);

    static void create(QMdiArea*);
    static void create(QMdiArea*, const db::NoteStore&);

public slots:
    void update_ui_values_from_store();
};

}
}
