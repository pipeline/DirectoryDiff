#pragma once

#include <QPlainTextEdit>

class TextViewWithScrollEvent : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit TextViewWithScrollEvent(QWidget *parent = 0);

signals:
  void scrollEvent(QWheelEvent* e);

public slots:

protected:
  virtual void wheelEvent(QWheelEvent *e);
};
