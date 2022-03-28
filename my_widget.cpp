#include "my_widget.h"
#include <QHeaderView>
#include <random>
#include <iostream>

void MyWidget::TimerTriggered() {
  label_with_time.setText("Время: " + QString::number(time_passed + 1) + " секунд.");
  time_passed++;
}

MyWidget::MyWidget() {
  info.resize(amount_of_cells);
  backup_info.resize(amount_of_cells);
  field.setEditTriggers(QAbstractItemView::NoEditTriggers);
  field.verticalHeader()->setVisible(false);
  field.horizontalHeader()->setVisible(false);
  setMinimumSize(300, 450);
  AddToLayout();
  MakeStyles();
  StartNewGame();
  MakeConnections();
}

void MyWidget::Pressed(int row, int column) {
  int dist_y = row - index_of_empty_cell / 4;
  int dist_x = column - index_of_empty_cell % 4;
   if (((dist_y == 1 || dist_y == -1) && dist_x == 0) ||
      (dist_y == 0 && (dist_x == 1 || dist_x == -1))) {
    field.item(index_of_empty_cell / 4, index_of_empty_cell % 4)->setText(
    field.item(row, column)->text());
    std::swap(info[index_of_empty_cell], info[row * 4 + column]);
    field.item(row, column)->setText("");
    index_of_empty_cell = row * 4 + column;
    label_with_amount_of_moves.setText("Ходов: " + QString::number(amount_of_moves + 1));
    amount_of_moves++;
    if (info[15] != amount_of_cells) {
      return;
    }
    if (info[13] == 15 && info[14] == 14) {
      for (int k = 0; k < 13; k++) {
        if (k + 1 != info[k]) {
          return;
        }
      }
      dialog.setText(QString(
          "Победа! (так как 13-15-14 допустимо)\nКоличество ходов: ") +
          QString::number(amount_of_moves));
      dialog.open();
      return;
    }
    for (int i = 0; i < 15; i++) {
      if (i + 1 != info[i]) {
        return;
      }
    }
    dialog.setText(QString("Победа!\nКоличество ходов: ") +
        QString::number(amount_of_moves));
    dialog.open();
  }
}

void MyWidget::StartNewGame() {
  static auto rng = std::default_random_engine{std::random_device{}()};
  for (int i = 0; i < amount_of_cells; i++) {
    info[i] = i + 1;
  }
  std::shuffle(std::begin(info), std::end(info), rng);
  backup_info = info;
  for (int i = 0; i < amount_of_cells; i++) {
    if (backup_info[i] == amount_of_cells) {
      index_of_empty_cell = i;
      field.item(i / 4, i % 4)->setText("");
    } else {
      field.item(i / 4, i % 4)->setText(QString::number(backup_info[i]));
    }
  }
  time_passed = 0;
  label_with_time.setText("Время: 0 секунд.");
  amount_of_moves = 0;
  label_with_amount_of_moves.setText("Ходов: 0");
}

void MyWidget::TryAgain() {
  info = backup_info;
  for (int index = 0; index < amount_of_cells; index++) {
    if (backup_info[index] != amount_of_cells) {
      field.item(index / 4,
                 index % 4)->setText(QString::number(backup_info[index]));
    } else {
      index_of_empty_cell = index;
      field.item(index / 4, index % 4)->setText("");
    }
  }
  time_passed = 0;
  label_with_time.setText("Время: 0 секунд.");
  amount_of_moves = 0;
  label_with_amount_of_moves.setText("Ходов: 0");
}

void MyWidget::resizeEvent(QResizeEvent* event) {
  for (int i = 0; i < 4; i++) {
    field.setColumnWidth(i, (double) field.width() * 0.249);
    field.setRowHeight(i, (double) field.height() * 0.249);
  }
}

void MyWidget::MakeConnections() {
  connect(&button_for_new_game,SIGNAL(clicked(bool)),this, SLOT(StartNewGame()));
  connect(&field, SIGNAL(cellActivated(int, int)),this, SLOT(Pressed(int, int)));
  connect(&dialog, SIGNAL(finished(int)), this, SLOT(StartNewGame()));
  connect(&try_again_button, SIGNAL(clicked(bool)), this, SLOT(TryAgain()));
  my_timer.setInterval(1000);
  connect(&my_timer, SIGNAL(timeout()), this, SLOT(TimerTriggered()));
  my_timer.start();
}

void MyWidget::MakeStyles() {
  dialog.setWindowTitle("Ура!");
  field.setFont(QFont("", 20, QFont::Bold));
  for (int i = 0; i < amount_of_cells; i++) {
    field.setItem(i / 4, i % 4, new QTableWidgetItem());
    field.item(i / 4, i % 4)->setTextAlignment(Qt::AlignCenter);
  }
  try_again_button.setStyleSheet("QPushButton {background-color: green}");
  button_for_new_game.setStyleSheet("QPushButton {background-color: blue}");
  field.setStyleSheet("QTableWidget {background-color: rgba(200, 200, 230, 230) }");
}

void MyWidget::AddToLayout() {
  layout.addWidget(&button_for_new_game);
  layout.addWidget(&try_again_button);
  layout.addWidget(&field);
  layout.addWidget(&label_with_time);
  layout.addWidget(&label_with_amount_of_moves);
}
