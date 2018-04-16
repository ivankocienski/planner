
#pragma once

#include <QtWidgets>

#include "notestore.hh"

namespace ik {
namespace app {

class Note : public QMdiSubWindow {
Q_OBJECT

private:

    db::NoteStore m_store;

private:

    void create_in_store(unsigned int);
    void remove_from_store();

    void moveEvent(QMoveEvent*);
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);

public:

    Note();
    Note(const db::NoteStore&);

    void save_to_store(unsigned int);

    static void create(QMdiArea*);
    static void create(QMdiArea*, const db::NoteStore&);

};

}
}
