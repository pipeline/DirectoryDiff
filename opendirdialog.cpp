#include "opendirdialog.h"
#include "ui_opendirdialog.h"
#include <QFileDialog>

OpenDirDialog::OpenDirDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::OpenDirDialog)
{
  ui->setupUi(this);
}

OpenDirDialog::~OpenDirDialog()
{
  delete ui;
}

void OpenDirDialog::on_pushButtonBrowse1_clicked() {
  ui->lineEditPath1->setText(QFileDialog::getExistingDirectory(this));
}

void OpenDirDialog::on_pushButtonBrowse2_clicked() {
  ui->lineEditPath2->setText(QFileDialog::getExistingDirectory(this));
}

QString OpenDirDialog::getDir1Name() {
  return ui->lineEditName1->text();
}

QString OpenDirDialog::getDir1Path() {
  return ui->lineEditPath1->text();
}

QString OpenDirDialog::getDir2Name() {
  return ui->lineEditName2->text();
}

QString OpenDirDialog::getDir2Path() {
  return ui->lineEditPath2->text();
}
