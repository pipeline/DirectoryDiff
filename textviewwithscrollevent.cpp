#include "textviewwithscrollevent.h"

TextViewWithScrollEvent::TextViewWithScrollEvent(QWidget *parent) :
  QPlainTextEdit(parent)
{
}

void TextViewWithScrollEvent::wheelEvent(QWheelEvent *e) {
  emit this->scrollEvent(e);
}
