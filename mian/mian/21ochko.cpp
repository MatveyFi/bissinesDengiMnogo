// ============================================================
//  Модуль 5: «21 (Очко)»
//
//  Игрок играет против дилера. Побеждает тот, у кого больше
//  очков, но не больше 21. Если очков больше 21 — это
//  «перебор», и такая рука сразу проигрывает.
//
//  Наружу модуль отдаёт одну функцию:  int playBlackjack();
//  Она проводит одну партию и возвращает очки:
//  победа — 100, ничья — 50, поражение — 0.
// ============================================================

#include <iostream>
#include <string>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <clocale>   // setlocale
using namespace std;

// ---------- Константы ----------

const int SCORE_WIN = 100;   // очков за победу
const int SCORE_DRAW = 50;    // очков за ничью
const int MAX_CARDS = 30;    // максимум карт в руке (с запасом)

// ---------- Карты ----------

// Случайная карта: 1 = туз, 2..10 = числа,
// 11 = валет, 12 = дама, 13 = король.
// Колоды нет: каждая карта выбирается случайно (так проще).
int drawCard() {
	return rand() % 13 + 1;
}

// Сколько очков даёт карта.
int cardValue(int card) {
	if (card == 1) return 11;    // туз пока считаем за 11
	if (card >= 10) return 10;   // 10, валет, дама, король = 10
	return card;                 // остальные — по номиналу
}

// Название карты для вывода на экран.
string cardName(int card) {
	if (card == 1) return "Туз";
	if (card == 11) return "Валет";
	if (card == 12) return "Дама";
	if (card == 13) return "Король";
	return to_string(card);      // 2..10 — просто число
}

// ---------- Рука (набор карт) ----------

// Считает очки руки.
// cards — массив карт, n — сколько карт в руке.
int handScore(int cards[], int n) {
	int sum = 0;     // сумма очков
	int aces = 0;    // сколько тузов в руке

	for (int i = 0; i < n; i++) {
		sum += cardValue(cards[i]);
		if (cards[i] == 1) aces++;
	}

	// Хитрость с тузом: при переборе туз можно посчитать
	// не за 11, а за 1, то есть отнять 10 очков.
	// Делаем это для каждого туза, пока перебор не пропадёт.
	while (sum > 21 && aces > 0) {
		sum -= 10;
		aces--;
	}
	return sum;
}

// Печатает карты и очки, например: Туз, 5 (очков: 16)
void printHand(int cards[], int n) {
	for (int i = 0; i < n; i++) {
		cout << cardName(cards[i]);
		if (i < n - 1) cout << ", ";
	}
	cout << " (очков: " << handScore(cards, n) << ")" << endl;
}

// ---------- Ввод ----------

// Задаёт вопрос, пока не получит 1 (да) или 0 (нет).
// Читаем строку, а не число: если ввести буквы, программа
// не зависнет, а просто спросит ещё раз.
bool askYesNo(string question) {
	string answer;
	while (true) {
		cout << question << " (1 - да, 0 - нет): ";

		// Ввод закончился (например, Ctrl+Z) — считаем, что «нет».
		if (!(cin >> answer)) return false;

		if (answer == "1") return true;
		if (answer == "0") return false;
		cout << "Введите 1 или 0.\n";
	}
}

// ---------- Сама игра ----------

int playBlackjack() {
	int player[MAX_CARDS];   // карты игрока
	int dealer[MAX_CARDS];   // карты дилера
	int playerCount = 0;     // сколько карт у игрока
	int dealerCount = 0;     // сколько карт у дилера

	cout << "\n===== ОЧКО (21) =====\n";

	// 1. Раздача: каждому по две карты.
	// playerCount++ кладёт карту в первую свободную ячейку
	// и сразу увеличивает счётчик.
	player[playerCount++] = drawCard();
	player[playerCount++] = drawCard();
	dealer[dealerCount++] = drawCard();
	dealer[dealerCount++] = drawCard();

	// Одну карту дилера игрок пока не видит.
	cout << "Карты дилера: " << cardName(dealer[0])
		<< ", [закрыта]\n";

	// 2. Ход игрока: берёт карты, пока не скажет «хватит»,
	// не наберёт 21 или не получит перебор.
	int playerScore = 0;
	while (true) {
		cout << "\nВаши карты: ";
		printHand(player, playerCount);
		playerScore = handScore(player, playerCount);

		// 21 или перебор — брать карты больше нельзя.
		if (playerScore >= 21) break;

		// Спрашиваем, нужна ли ещё карта.
		if (!askYesNo("Взять ещё карту?")) break;

		player[playerCount++] = drawCard();
	}

	if (playerScore > 21) {
		cout << "Перебор! Вы проиграли.\n";
		return 0;
	}
	if (playerScore == 21) {
		cout << "У вас 21!\n";
	}

	// 3. Ход дилера. Правило казино: берёт карты,
	// пока у него меньше 17 очков.
	cout << "\nДилер открывает карты: ";
	printHand(dealer, dealerCount);

	while (handScore(dealer, dealerCount) < 17) {
		dealer[dealerCount++] = drawCard();
		cout << "Дилер берёт карту: ";
		printHand(dealer, dealerCount);
	}
	int dealerScore = handScore(dealer, dealerCount);

	// 4. Подводим итог.
	cout << "\nВы: " << playerScore
		<< ", дилер: " << dealerScore << endl;

	if (dealerScore > 21) {
		cout << "У дилера перебор. Вы выиграли!\n";
		return SCORE_WIN;
	}
	if (playerScore > dealerScore) {
		cout << "Вы выиграли!\n";
		return SCORE_WIN;
	}
	if (playerScore == dealerScore) {
		cout << "Ничья.\n";
		return SCORE_DRAW;
	}
	cout << "Дилер выиграл.\n";
	return 0;
}

// ---------- Запуск модуля отдельно ----------
//
// Эта main нужна только чтобы проверить игру самостоятельно.
// В общем проекте её нужно убрать: setlocale и srand будут
// вызываться один раз в главном меню (модуль 1).

void ochko() {
	setlocale(LC_ALL, ".UTF-8");   // русские буквы в консоли
	srand((unsigned)time(0));       // карты каждый раз разные

	do {
		int score = playBlackjack();
		cout << "Заработано очков: " << score << endl;
	} while (askYesNo("\nСыграть ещё раз?"));

}