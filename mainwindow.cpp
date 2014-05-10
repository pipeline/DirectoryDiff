#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "textviewwithscrollevent.h"
#include "diff_match_patch.h"
#include "opendirdialog.h"
#include <QScrollBar>
#include <QTextBlock>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  textView1 = new TextViewWithScrollEvent(this);
  textView2 = new TextViewWithScrollEvent(this);
  ui->verticalLayoutText1->addWidget(textView1);
  ui->verticalLayoutText2->addWidget(textView2);
  connect(textView1, SIGNAL(scrollEvent(QWheelEvent*)), this, SLOT(textViewScrolled(QWheelEvent*)));
  connect(textView2, SIGNAL(scrollEvent(QWheelEvent*)), this, SLOT(textViewScrolled(QWheelEvent*)));

  ui->labelMultiple->setVisible(false);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::textViewScrolled(QWheelEvent* evt) {
  if(evt->orientation() == Qt::Vertical) {
    textView1->verticalScrollBar()->setValue(textView1->verticalScrollBar()->value() + evt->delta() * (-1));
    textView2->verticalScrollBar()->setValue(textView2->verticalScrollBar()->value() + evt->delta() * (-1));
  }
}

void MainWindow::ShowDifferenceFiles(QString one, QString two) {
  QFile fileOne(one);
  QFile fileTwo(two);
  fileOne.open(QFile::ReadOnly);
  fileTwo.open(QFile::ReadOnly);

  QString response1, response2;

  if(fileOne.exists())
    response1 = QString(fileOne.readAll().data());
  if(fileTwo.exists())
    response2 = QString(fileTwo.readAll().data());

  if(fileOne.isOpen())
    fileOne.close();
  if(fileTwo.isOpen())
    fileTwo.close();

  if(response1 == "" || response2 == "") {
    textView1->setPlainText(response1);
    textView2->setPlainText(response2);
    return;
  }

  diff_match_patch diff;
  QList<QVariant> lineDetails = diff.diff_linesToChars(response1, response2);
  QList<Diff> diffs = diff.diff_main(lineDetails[0].toString(), lineDetails[1].toString());
  diff.diff_charsToLines(diffs, lineDetails[2].toStringList());

  int firstDiff = -1, lineCount = 0;
  QString html1, html2;
  int count1 = 0, count2 = 0;
  int addCount = 0;

  foreach(Diff diff, diffs) {
    if(diff.operation == EQUAL) {
      for(int i = 0; i < count1; i++) {
        html2 += "<br>";
        lineCount++;
      }
      for(int i = 0; i < count2; i++) {
        html1 += "<br>";
      }

      html1 += diff.text.toHtmlEscaped().replace(" ", "&nbsp;");
      html2 += diff.text.toHtmlEscaped().replace(" ", "&nbsp;");
      count1 = count2 = 0;
      lineCount += countNewLines(diff.text);
    }
    else if(diff.operation == DELETE) {
      if(firstDiff == -1)
        firstDiff = lineCount;
      html2  += ("<span style=\"background-color: #e8aaaa;\">" + diff.text.toHtmlEscaped().replace(" ", "&nbsp;") + "</span>");
      count2 += countNewLines(diff.text);
    }
    else {
      if(firstDiff == -1)
        firstDiff = lineCount;

      addCount++;
      html1     += ("<span style=\"background-color: #aae8ac;\">" + diff.text.toHtmlEscaped().replace(" ", "&nbsp;") + "</span>");
      count1    += countNewLines(diff.text);
      lineCount += countNewLines(diff.text);
    }
  }

  textView1->clear();
  textView2->clear();
  textView1->appendHtml(html1.replace("\n", "<br>"));
  textView2->appendHtml(html2.replace("\n", "<br>"));

  if(addCount >= 2) {
    ui->labelMultiple->setVisible(true);
    ui->labelMultiple->setText(QString("There are ") + QString::number(addCount) + QString(" differences between these files"));
  }
  else {
    ui->labelMultiple->setVisible(false);
  }

  goToLine(textView1, firstDiff - 5);
}

void MainWindow::goToLine(QPlainTextEdit* ctrl, int line) {
  // TODO: This isn't working
  QTextCursor cursor = ctrl->textCursor();
  qDebug("Current block: %d", cursor.blockNumber());
  int linenumber = cursor.blockNumber();
  int linedif = line - linenumber;
  qDebug("Line diff: %d", linedif);
  if (linedif < 0)
    cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor, -linedif);
  else
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, linedif);
  ctrl->setTextCursor(cursor);
}

int MainWindow::countNewLines(QString text) {
  int count = 0;
  int idx = text.indexOf("\n");
  while(idx != -1) {
    count++;
    idx = text.indexOf("\n", idx + 1);
  }
  return count;
}

void MainWindow::on_action_Open_triggered() {
  openDirDialog = new OpenDirDialog(this);
  connect(openDirDialog, SIGNAL(accepted()), this, SLOT(openDlgCompleted()));
  openDirDialog->show();
}

void MainWindow::openDlgCompleted() {
  ui->labelText1->setText(openDirDialog->getDir1Name());
  ui->labelText2->setText(openDirDialog->getDir2Name());
  dir1 = openDirDialog->getDir1Path();
  dir2 = openDirDialog->getDir2Path();
  openDirectories();
}

void MainWindow::openDirectories() {
  QStringList l;
  l.append("/");
  QTreeWidgetItem* treeRoot = new QTreeWidgetItem(l);
  ui->treeWidget->addTopLevelItem(treeRoot);
  treeRoot->setExpanded(true);

  processDir(treeRoot, dir1, QDir(dir1));
  processDir(treeRoot, dir2, QDir(dir2));
}

void MainWindow::processDir(QTreeWidgetItem* treeParent, QString dirBase, QDir dir) {
  QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);

  foreach(QFileInfo fileinfo, files) {
    QString relativepath = fileinfo.absoluteFilePath().replace(dirBase, "");

    if(fileinfo.isDir()) {
      QTreeWidgetItem* treeItem = NULL;
      for(int i = 0; i < treeParent->childCount(); i++) {
        if(treeParent->child(i)->text(0) == fileinfo.fileName()) {
          treeItem = treeParent->child(i);
          break;
        }
      }

      if(treeItem == NULL) {
        QStringList l;
        l.append(fileinfo.fileName());
        treeItem = new QTreeWidgetItem(l);
        treeParent->addChild(treeItem);
        treeItem->setExpanded(true);
      }

      processDir(treeItem, dirBase, dir.absoluteFilePath(fileinfo.fileName()));
    }
    else if(fileinfo.isFile()) {
      if(!this->files.contains(relativepath)) {
        processFile(treeParent, relativepath);
      }
    }
  }
}

void MainWindow::processFile(QTreeWidgetItem* parent, QString path) {
  QFile file1(dir1 + path);
  QFile file2(dir2 + path);

  QString filename = path.mid(path.lastIndexOf("/") + 1);
  this->files.append(path);

  if(!file1.exists() || !file2.exists()) {
    QStringList l;
    l.append(filename);
    QTreeWidgetItem* item = new QTreeWidgetItem(l);
    item->setData(0, Qt::UserRole, path);
    item->setForeground(0, QBrush(Qt::red));
    parent->addChild(item);
    return;
  }

  file1.open(QFile::ReadOnly);
  file2.open(QFile::ReadOnly);

  QString response1 = QString(file1.readAll().data());
  QString response2 = QString(file2.readAll().data());

  if(response1 != response2) {
    QStringList l;
    l.append(filename);
    QTreeWidgetItem* item = new QTreeWidgetItem(l);
    item->setData(0, Qt::UserRole, path);
    item->setForeground(0, QBrush(Qt::darkGreen));
    parent->addChild(item);
  }

  file1.close();
  file2.close();

}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
  QString path = current->data(0, Qt::UserRole).toString();
  if(path != "") {
    ShowDifferenceFiles(dir1 + path, dir2 + path);
  }
}
