#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string> // <--- Добавь эту строку

using namespace std;

const int SIZE = 8;   // Размер поля 8x8
const int MINES = 10; // Количество мин

// Состояния видимости клеток
// 0 - закрыта, 1 - открыта, 2 - стоит флажок
int state[SIZE][SIZE] = { 0 };

// Реальное содержимое поля
// -1 - мина, 0..8 - количество мин вокруг
int board[SIZE][SIZE] = { 0 };

// Расстановка мин и подсчет цифр вокруг
void initBoard() {
  srand(time(0));
  int placed = 0;
  while (placed < MINES) {
    int r = rand() % SIZE;
    int c = rand() % SIZE;
    if (board[r][c] != -1) {
      board[r][c] = -1;
      placed++;
    }
  }

  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      if (board[r][c] == -1) continue;
      int count = 0;
      for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
          int nr = r + dr, nc = c + dc;
          if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE && board[nr][nc] == -1) {
            count++;
          }
        }
      }
      board[r][c] = count;
    }
  }
}

// Рекурсивное открытие клеток
void reveal(int r, int c) {
  if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) return;
  if (state[r][c] != 0) return;

  state[r][c] = 1;

  // Если вокруг 0 мин, открываем всех соседей
  if (board[r][c] == 0) {
    for (int dr = -1; dr <= 1; dr++) {
      for (int dc = -1; dc <= 1; dc++) {
        reveal(r + dr, c + dc);
      }
    }
  }
}

// Отрисовка поля в консоли
void printBoard(bool showAll = false) {
  cout << "\n   ";
  for (int c = 0; c < SIZE; c++) cout << c << " ";
  cout << "\n  -----------------\n";

  for (int r = 0; r < SIZE; r++) {
    cout << r << " |";
    for (int c = 0; c < SIZE; c++) {
      if (showAll || state[r][c] == 1) {
        if (board[r][c] == -1) cout << "* ";
        else if (board[r][c] == 0) cout << ". ";
        else cout << board[r][c] << " ";
      }
      else if (state[r][c] == 2) {
        cout << "F ";
      }
      else {
        cout << "# ";
      }
    }
    cout << "|\n";
  }
  cout << "  -----------------\n";
}

// Проверка победы
bool checkWin() {
  int openedSafe = 0;
  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      if (board[r][c] != -1 && state[r][c] == 1) {
        openedSafe++;
      }
    }
  }
  return openedSafe == (SIZE * SIZE - MINES);
}

// Генерация HTML-отчета об игре
void exportToHTML(bool won, int moves) {
  ofstream file("result.html");
  file << "<!DOCTYPE html>\n<html lang='ru'>\n<head>\n";
  file << "<meta charset='UTF-8'>\n<title>Результат Сапёра</title>\n";
  file << "<link rel='stylesheet' href='style.css'>\n</head>\n<body>\n";
  file << "<div class='container'>\n";
  file << "<h1>Итоги партии в Сапёра</h1>\n";
  file << "<p class='status " << (won ? "win" : "lose") << "'>"
    << (won ? "Победа! Все мины обезврежены." : "Поражение! Вы подорвались на мине.") << "</p>\n";
  file << "<p>Сделано ходов: <b>" << moves << "</b></p>\n";
  file << "<div class='grid'>\n";

  for (int r = 0; r < SIZE; r++) {
    for (int c = 0; c < SIZE; c++) {
      if (board[r][c] == -1) {
        file << "<div class='cell mine'>💣</div>\n";
      }
      else if (state[r][c] == 1) {
        file << "<div class='cell open'>" << (board[r][c] == 0 ? "" : to_string(board[r][c])) << "</div>\n";
      }
      else if (state[r][c] == 2) {
        file << "<div class='cell flag'>🚩</div>\n";
      }
      else {
        file << "<div class='cell closed'></div>\n";
      }
    }
  }

  file << "</div>\n</div>\n</body>\n</html>";
  file.close();
  cout << "\nОтчет сохранен в файл result.html! Открой его в браузере.\n";
}

void saper() {
  initBoard();

  int moves = 0;
  bool gameOver = false;
  bool won = false;

  cout << "=== КОНСОЛЬНЫЙ САПЁР ===\n";
  cout << "Формат ввода: [действие] [строка] [столбец]\n";
  cout << "Действия: o - открыть клетку, f - поставить/убрать флаг (пример: o 2 3)\n";

  while (!gameOver) {
    printBoard();
    char action;
    int r, c;
    cout << "Ваш ход: ";
    cin >> action >> r >> c;

    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) {
      cout << "Неверные координаты! Введите числа от 0 до " << SIZE - 1 << ".\n";
      continue;
    }

    moves++;

    if (action == 'f' || action == 'F') {
      if (state[r][c] == 0) state[r][c] = 2;
      else if (state[r][c] == 2) state[r][c] = 0;
    }
    else if (action == 'o' || action == 'O') {
      if (state[r][c] == 2) {
        cout << "Сначала снимите флаг с этой клетки!\n";
        continue;
      }
      if (board[r][c] == -1) {
        gameOver = true;
        won = false;
      }
      else {
        reveal(r, c);
        if (checkWin()) {
          gameOver = true;
          won = true;
        }
      }
    }
    else {
      cout << "Неизвестная команда! Используйте 'o' или 'f'.\n";
    }
  }

  printBoard(true);
  cout << (won ? "\nПОЗДРАВЛЯЕМ! ВЫ ПОБЕДИЛИ!\n" : "\nБАБАХ! ИГРА ОКОНЧЕНА.\n");
  exportToHTML(won, moves);

  return 0;
}
