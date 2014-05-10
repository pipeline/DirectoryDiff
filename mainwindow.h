#pragma once

#include <QMainWindow>
#include <QDir>
#include <QTreeWidgetItem>
#include "textviewwithscrollevent.h"
#include "opendirdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void textViewScrolled(QWheelEvent*);
  void on_action_Open_triggered();
  void openDlgCompleted();
  void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
  Ui::MainWindow *ui;
  TextViewWithScrollEvent *textView1, *textView2;

  void ShowDifferenceFiles(QString one, QString two);
  void goToLine(QPlainTextEdit* ctrl, int line);
  int countNewLines(QString text);

  void openDirectories();
  void processDir(QTreeWidgetItem* treeParent, QString dirBase, QDir dir);
  void processFile(QTreeWidgetItem* parent, QString path);

  QString dir1, dir2;
  QStringList files;
  OpenDirDialog* openDirDialog;
};
