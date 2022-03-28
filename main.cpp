#include <QApplication>
#include "my_widget.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MyWidget my_widget{};
  my_widget.show();
  my_widget.setWindowTitle("Игра");
  return QApplication::exec();
}
