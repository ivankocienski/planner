
#include "main-window.hh"
#include "note.hh"

namespace ik {
namespace app {

MainWindow::MainWindow() {

  buildUI();
  connectHandlers();

  resize(400, 300);
  show();
}

void MainWindow::buildUI() {

  //QVBoxLayout *main_layout = new QVBoxLayout();

  QMenu *menu = menuBar()->addMenu("menu");

  m_new_action = new QAction("New");
  menu->addAction(m_new_action);

  m_mdi_area = new QMdiArea();
  //main_layout->addWidget(m_mdi_area);

  //m_new_button = new QPushButton("New window");
  //main_layout->addWidget(m_new_button);

  //QWidget *widget = new QWidget();
  //widget->setLayout(main_layout);

  //setCentralWidget(widget); 
  setCentralWidget(m_mdi_area); 
}

void MainWindow::connectHandlers() {

  //connect(m_new_button, SIGNAL(clicked()), this, SLOT(onNewNoteClick()));

  connect(m_new_action, SIGNAL(triggered()),
      this, SLOT(onNewNoteClick()));
}

void MainWindow::onNewNoteClick() {

  qInfo() << "note clicked";

  Note *note = new Note();

  m_mdi_area->addSubWindow(note);
  note->show();
}

} // namespce app
} // namespace ik
