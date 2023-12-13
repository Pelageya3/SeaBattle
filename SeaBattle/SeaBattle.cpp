#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "windows.h"

void printTableString(char table[10])
{
	for (int j = 0; j < 10; j++)
		std::cout << std::setw(2) << table[j];
}

void printLetters()
{
	std::string letters = "abcdefghij";

	for (int k = 0; k < letters.length(); k++)
		std::cout << std::setw(2) << letters[k];
}

void printTables(char firstTable[10][10], char secondTable[10][10])
{
	std::cout << std::setw(4) << "";
	printLetters();
	std::cout << std::setw(7) << "";
	printLetters();
	std::cout << std::endl;

	std::cout << std::setw(4) << "" << "---------------------" <<
				 std::setw(6) << "" << "---------------------" << std::endl;

	for (int i = 0; i < 10; i++)
	{
		std::cout << std::setw(3) << i + 1 << '|';
		printTableString(firstTable[i]);
		std::cout << " | " << std::setw(3) << i + 1 << '|';
		printTableString(secondTable[i]);
		std::cout << " | " << std::endl;
	}

	std::cout << std::setw(4) << "" << "---------------------" <<
				 std::setw(6) << "" << "---------------------" << std::endl; 
}

void printDelay(std::string textForDelay)
{
	int delayPrint = 50;
	bool checkEnter = false;

	Sleep(delayPrint);

	for (int i = 0; i < textForDelay.length(); i++)
	{
		if (checkEnter == false)
		{
			if (GetAsyncKeyState(VK_RETURN) == -32767)
				checkEnter = true;
			else
				Sleep(delayPrint);
		}

		std::cout << textForDelay[i];
	}

	std::cout << std::endl;
}

void rulesPrint(int rulesCh)
{
	std::string rules = "ляляляля правила такие-то";

	if (rulesCh == 1)
	{
		printDelay(rules);
		system("pause");
	}
	else if (rulesCh == 2)
		return;
	else
	{
		system("cls");
		std::cout << "Неверный пункт меню. Хотите ознакомиться с правилами?\n" << std::endl;
	}
}

void tutorial()
{
	int rulesChoice;
	std::string rulesQuestion = "Добро пожаловать в игру Морской бой!\nХотите ознакомиться с правилами?\n";

	printDelay(rulesQuestion);

	do
	{	
		std::cout << "1 - Да\n\n";
		std::cout << "2 - Нет, хочу начать игру!\n\n";

		std::cin >> rulesChoice;
		system("cls");
		rulesPrint(rulesChoice);

	} while (rulesChoice < 1 || rulesChoice > 2);

	system("cls");
}

void gameModeChoice(std::string modeQuestion, std::string modeOne, std::string modeTwo)
{
	int choiceMode = 0;

	do
	{
		printDelay(modeQuestion);

		std::cout << modeOne << "\n" << modeTwo << std::endl;
		std::cin >> choiceMode;

		if (choiceMode == 1)
			std::cout << "1";
		else if (choiceMode == 2)
			std::cout << "2";
		else
		{
			system("cls");
			std::cout << "Неверный пункт меню\n" << std::endl;
		}

	} while (choiceMode < 1 || choiceMode > 2);

	system("cls");
}

bool allShipsDestroyed(char table[10][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (table[i][j] == '#')
				return false;
		}
	}

	return true;
}

void gameLoop()
{
	char tableOne[10][10]{};
	char tableTwo[10][10]{};

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			tableOne[i][j] = '#';
			tableTwo[i][j] = '~';
		}

	tableTwo[2][3] = '#';
	tableTwo[2][4] = '#';
	tableTwo[2][5] = '#';

	tableTwo[5][6] = '#';
	tableTwo[5][7] = '#';
	tableTwo[5][8] = '#';

	tableTwo[0][9] = '#';
	tableTwo[1][9] = '#';
	tableTwo[2][9] = '#';

	tableTwo[4][0] = '#';
	tableTwo[5][0] = '#';
	tableTwo[6][0] = '#';
	tableTwo[7][0] = '#';

	while (!allShipsDestroyed(tableOne) && !allShipsDestroyed(tableTwo))
	{
		system("cls");

		char hiddenTableTwo[10][10]{};

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (tableTwo[i][j] == '#')
					hiddenTableTwo[i][j] = '~';
				else
					hiddenTableTwo[i][j] = tableTwo[i][j];
			}
		}

		printTables(tableOne, hiddenTableTwo);
		std::cout << "Введите координаты выстрела: " << std::endl;

		char col;
		int row;

		//стреляет компьютер

		for (;;)
		{
			std::cin >> col >> row;

			if (std::cin.fail() || row < 1 || row > 10 || col < 'a' || col > 'j')
			{
				std::cout << "Некорректные координаты. Введите заново: " << std::endl;
				std::cin.clear();
				std::cin.ignore();
				continue;
			}
			break;
		}

		if (tableTwo[row - 1][col - 'a'] == '#')
			tableTwo[row - 1][col - 'a'] = 'X';
		else
			tableTwo[row - 1][col - 'a'] = '*';
	}

	if (allShipsDestroyed(tableOne))
		std::cout << "Игрок 2 победил!" << std::endl;
	else
		std::cout << "Игрок 1 победил!" << std::endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	system("color 3F");
	
	tutorial();

	std::string playerModeQuestion = "Выберите режим игры: \n";
	std::string playerModeHVSC = "1 - Человек против компьютера\n";
	std::string playerModeCVSC = "2 - Компьютер против компьютера\n";
	
	gameModeChoice(playerModeQuestion, playerModeHVSC, playerModeCVSC);

	std::string placementModeQuestion = "Выберите режим расстановки кораблей: \n";
	std::string placementModeAuto = "1 - Автоматически\n";
	std::string placementModeManually = "2 - Вручную\n";

	gameModeChoice(placementModeQuestion, placementModeAuto, placementModeManually);
	
	std::string intellectModeQuestion = "Выберите уровень интеллекта компьютера: \n";
	std::string intellectModeRandom = "1 - Случайные выстрелы\n";
	std::string intellectModeAI = "2 - Интеллектуальная игра\n";

	gameModeChoice(intellectModeQuestion, intellectModeRandom, intellectModeAI);

	gameLoop();

	return 0;
}