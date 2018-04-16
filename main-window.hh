
#pragma once

#include <QtWidgets>

namespace ik {
namespace app {

class MainWindow : public QMainWindow {
    Q_OBJECT

private:

    QMdiArea *m_mdi_area;
    QAction *m_new_action;
    QAction *m_exit_action;

private:

    void buildUI();
    void connectHandlers();
    void restoreWindows();

    void restore_window_state();
    void save_window_state();

    void closeEvent(QCloseEvent *);

public:

    MainWindow();

private slots:

    void onNewNoteClick();
};

}
}

