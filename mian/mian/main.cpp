#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int SIZE = 10;

struct Player {
  int x = 5;
  int y = 5;
  int money = 100;
  int wantedLevel = 0;
  bool inCar = false;
  string carName = "Пешком";
};

void drawMap(const Player& p, int carX, int carY, int policeX, int policeY, int clubX, int clubY) {
  system("cls || clear");
  cout << "=== GTA: VICE CONSOLE EDITION ===" << endl;
  cout << "Деньги: $" << p.money << " | Розыск: " << p.wantedLevel << "* | Транспорт: " << p.carName << endl;
  cout << "Управление: W/A/S/D - ход, F - авто, E - ограбить, R - зайти в клуб (S), Q - выход" << endl;
  cout << "-----------------------------------------------------------------------------" << endl;

  for (int y = 0; y < SIZE; y++) {
    for (int x = 0; x < SIZE; x++) {
      if (x == p.x && y == p.y) {
        cout << (p.inCar ? "[C]" : "[P]");
      }
      else if (x == policeX && y == policeY && p.wantedLevel > 0) {
        cout << " ! "; // Полиция
      }
      else if (x == clubX && y == clubY) {
        cout << " S "; // Клуб (Strip Club)
      }
      else if (x == carX && y == carY && !p.inCar) {
        cout << " c "; // Свободная машина
      }
      else {
        cout << " . ";
      }
    }
    cout << endl;
  }
}

int main() {
  srand(static_cast<unsigned int>(time(0)));

  Player player;
  int carX = 2, carY = 2;
  int policeX = 0, policeY = 0;
  int clubX = 8, clubY = 8; // Координаты клуба
  char input;
  string message = "";

  while (true) {
    drawMap(player, carX, carY, policeX, policeY, clubX, clubY);

    if (!message.empty()) {
      cout << "\n[Событие]: " << message << endl;
      message = "";
    }

    // Проверка поимки полицией
    if (player.wantedLevel > 0 && player.x == policeX && player.y == policeY) {
      cout << "\nПОТРАЧЕНО (WASTED)! Вас поймала полиция. Штраф: $50." << endl;
      player.money = max(0, player.money - 50);
      player.wantedLevel = 0;
      player.inCar = false;
      player.carName = "Пешком";
      cout << "Введите любой символ и нажмите Enter, чтобы продолжить...";
      cin >> input;
      continue;
    }

    cout << "\nВаш ход: ";
    cin >> input;
    input = tolower(input);

    if (input == 'q') break;

    // Движение
    int speed = player.inCar ? 2 : 1;
    if (input == 'w' && player.y - speed >= 0) player.y -= speed;
    if (input == 's' && player.y + speed < SIZE) player.y += speed;
    if (input == 'a' && player.x - speed >= 0) player.x -= speed;
    if (input == 'd' && player.x + speed < SIZE) player.x += speed;

    // Угон / выход из авто
    if (input == 'f') {
      if (!player.inCar && player.x == carX && player.y == carY) {
        player.inCar = true;
        player.carName = "Спорткар";
        player.wantedLevel = min(5, player.wantedLevel + 1);
        message = "Вы угнали спорткар! Полиция на хвосте.";
      }
      else if (player.inCar) {
        player.inCar = false;
        player.carName = "Пешком";
        carX = player.x;
        carY = player.y;
        message = "Вы вышли из машины.";
      }
    }

    // Ограбление
    if (input == 'e' && !player.inCar) {
      int loot = rand() % 50 + 20;
      player.money += loot;
      player.wantedLevel = min(5, player.wantedLevel + 1);
      message = "Вы ограбили прохожего на $" + to_string(loot) + "!";
    }

    // Вход в клуб
    if (input == 'r' && player.x == clubX && player.y == clubY) {
      if (player.inCar) {
        message = "Сначала выйдите из машины (нажмите F)!";
      }
      else if (player.money >= 40) {
        player.money -= 40;
        player.wantedLevel = 0; // Полиция теряет след, пока вы внутри
        message = "Вы отлично отдохнули в клубе (-$40). Розыск полностью сброшен!";
      }
      else {
        message = "Охрана не пускает: нужно минимум $40 на вход!";
      }
    }

    // Движение полиции
    if (player.wantedLevel > 0) {
      if (policeX < player.x) policeX++;
      else if (policeX > player.x) policeX--;

      if (policeY < player.y) policeY++;
      else if (policeY > player.y) policeY--;
    }
  }

  cout << "Игра окончена. Итоговый счет: $" << player.money << endl;
  return 0;
}
