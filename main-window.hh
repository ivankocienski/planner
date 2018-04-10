
#pragma once

#include <QtWidgets>

class MainWindow : public QMainWindow {
Q_OBJECT

private: 

  QMdiArea *m_mdi_area;
  QPushButton *m_new_button;
  QAction *m_new_action;

private:

  void buildUI();
  void connectHandlers();

public:

  MainWindow();

private slots:

  void onNewNoteClick();
};

