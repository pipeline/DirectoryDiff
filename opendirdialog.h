#pragma once

#include <QDialog>

namespace Ui {
class OpenDirDialog;
}

class OpenDirDialog : public QDialog
{
  Q_OBJECT

public:
  explicit OpenDirDialog(QWidget *parent = 0);
  ~OpenDirDialog();

  QString getDir1Name();
  QString getDir1Path();
  QString getDir2Name();
  QString getDir2Path();

private slots:
  void on_pushButtonBrowse1_clicked();
  void on_pushButtonBrowse2_clicked();

private:
  Ui::OpenDirDialog *ui;
};
