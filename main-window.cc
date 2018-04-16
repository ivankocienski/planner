
#include "main-window.hh"
#include "note.hh"

namespace ik {
namespace app {

MainWindow::MainWindow() {

  buildUI();
  connectHandlers();
  restoreWindows();

  restore_window_state();

  show();
}

void MainWindow::restore_window_state() {

    QSettings settings;

    resize(settings.value("mainwindow/size", QSize(800, 600)).toSize());
    move(settings.value("mainwindow/position", QPoint(20, 20)).toPoint());
}

void MainWindow::save_window_state() {

    QSettings settings;

    settings.setValue("mainwindow/size", size());
    settings.setValue("mainwindow/position", pos());
}

void MainWindow::buildUI() {

  QMenu *menu = menuBar()->addMenu("menu");

  m_new_action = new QAction("New");
  m_new_action->setIcon(QIcon::fromTheme("document-new"));

  m_exit_action = new QAction("Exit");
  m_exit_action->setIcon(QIcon::fromTheme("application-exit"));

  menu->addAction(m_new_action);
  menu->addSeparator();
  menu->addAction(m_exit_action);

  QToolBar *toolbar = addToolBar("Basic toolbar");
  toolbar->addAction(m_new_action);
  toolbar->addAction(m_exit_action);

  m_mdi_area = new QMdiArea();

  setCentralWidget(m_mdi_area); 
}

void MainWindow::connectHandlers() {

  connect(m_new_action, SIGNAL(triggered()),
      this, SLOT(onNewNoteClick()));

  connect(m_exit_action, SIGNAL(triggered()),
          this, SLOT(close()));
}

void MainWindow::restoreWindows() {
    QList<db::NoteStore> note_list = db::NoteStore::find_all();

    for(auto & it : note_list) {
        Note::create(m_mdi_area, it);
    }

}

void MainWindow::closeEvent(QCloseEvent *ev) {

    auto windows = m_mdi_area->subWindowList(QMdiArea::StackingOrder);
    int stack_pos = 0;

    for( auto & it : windows ) {
        reinterpret_cast<Note *>(it)->save_to_store(stack_pos);
        stack_pos++;
    }

    save_window_state();

    ev->accept();
}

void MainWindow::onNewNoteClick() {

  qInfo() << "note clicked";

  Note::create(m_mdi_area);

  //Note *note = new Note();

  //m_mdi_area->addSubWindow(note);
  //note->show();
}

} // namespce app
} // namespace ik
