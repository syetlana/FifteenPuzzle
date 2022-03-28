#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <vector>
#include <QLabel>
#include <QGridLayout>
#include <QTimer>

class MyWidget : public QWidget {
 Q_OBJECT
public:
  MyWidget();
  void MakeConnections();
  void MakeStyles();
  void AddToLayout();

  QTableWidget field{4, 4, this};
  QTimer my_timer{};
  const int amount_of_cells = 16;
  QGridLayout layout{this};
  QPushButton try_again_button{"Повторить игру", this};
  QLabel label_with_amount_of_moves{"Ходов: 0"};
  int amount_of_moves = 0;
  QPushButton button_for_new_game{"Новая игра", this};
  QLabel label_with_time{"Время: 0 секунд."};
  int time_passed = 0;
  QMessageBox dialog{};
  std::vector<int> info{};
  std::vector<int> backup_info{};
  int index_of_empty_cell;

public slots:
  void TimerTriggered();
  void Pressed(int row, int column);
  void TryAgain();
  void StartNewGame();
  void resizeEvent(QResizeEvent* event) override;
};

#endif //MY_WIDGET_H
