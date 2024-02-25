#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "windows.h"
#include <fstream>
#include <string>


struct sea_field {
	char table[10][10];
};

void ClearInput()
{
	std::cin.clear();
	std::cin.ignore();
}

void printTableString(const char table[10])
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

void printTables(const sea_field& firstTable, const sea_field& secondTable)
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
		printTableString(firstTable.table[i]);
		std::cout << " | " << std::setw(3) << i + 1 << '|';
		printTableString(secondTable.table[i]);
		std::cout << " | " << std::endl;
	}

	std::cout << std::setw(4) << "" << "---------------------" <<
				 std::setw(6) << "" << "---------------------" << std::endl; 
}

void printTable(const sea_field& firstTable)
{
	std::cout << std::setw(4) << "";
	printLetters();
	std::cout << std::endl;

	std::cout << std::setw(4) << "" << "---------------------" << std::endl;

	for (int i = 0; i < 10; i++)
	{
		std::cout << std::setw(3) << i + 1 << '|';
		printTableString(firstTable.table[i]);
		std::cout << " | " << std::endl;
	}

	std::cout << std::setw(4) << "" << "---------------------" << std::endl;
}

void printDelay(std::string textForDelay)
{
	int delayPrint = 50;
	bool checkEnter = false;

	Sleep(delayPrint);

	for (int i = 0; i < textForDelay.length(); i++)
	{
		if (checkEnter == false)
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
				checkEnter = true;
			else
				Sleep(delayPrint);

		std::cout << textForDelay[i];
	}

	std::cout << std::endl;
}

void rulesPrint(int rulesCh)
{
	std::ifstream rulesFile("rules.txt");
	std::string line, rules;
	
	while (std::getline(rulesFile, line))
		rules += line + '\n';
	rulesFile.close();

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
		ClearInput();
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

int gameModeChoice(std::string modeQuestion, std::string modeOne, std::string modeTwo)
{
	int choiceMode = 0;

	do
	{
		printDelay(modeQuestion);

		std::cout << modeOne << "\n" << modeTwo << std::endl;
		std::cin >> choiceMode;

		if (choiceMode == 1 || choiceMode == 2)
		{
			system("cls");
			return choiceMode;
		}
		else
		{
			system("cls");
			std::cout << "Неверный пункт меню\n" << std::endl;
			ClearInput();
		}

	} while (choiceMode < 1 || choiceMode > 2);

	system("cls");
}

bool allShipsDestroyed(const sea_field& field)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (field.table[i][j] == '#')
				return false;
		}

	return true;
}

bool isShipSegmentPlaceable(int i, int j, sea_field& field)
{
	if (i < 0 || i > 9 || j < 0 || j > 9)
		return false;
	for (int v = i - 1; v <= i + 1; v++)
		for (int b = j - 1; b <= j + 1; b++)
		{
			if (v < 0 || v > 9 || b < 0 || b > 9)
				continue;
			if (field.table[v][b] == '#')
				return false;
		}
	return true;
}

void directionalCoordinateMove(int& i, int& j, int dir)
{
	switch (dir)
	{
	case 0:
		i--;
		break;
	case 1:
		i++;
		break;
	case 2:
		j--;
		break;
	case 3:
		j++;
		break;
	}
}
	
bool isShipPlaceable(int i, int j, int dir, sea_field& field, int shipSize)
{
	for (int k = 0; k < shipSize; k++)
	{
		if (!isShipSegmentPlaceable(i, j, field))
			return false;
		directionalCoordinateMove(i, j, dir);
	}
	return true;
}

void shipsPrint(int i, int j, int dir, int shipSize, sea_field& field)
{
	for (int k = 0; k < shipSize; k++)
	{
		field.table[i][j] = '#';
		directionalCoordinateMove(i, j, dir);
	}
}

void checkCoordinates(char& col, int& row)
{
	while (true)
	{
		std::cin >> col >> row;

		if (std::cin.fail() || row < 1 || row > 10 || col < 'a' || col > 'j')
		{
			std::cout << "Некорректные координаты. Введите заново: " << std::endl;
			std::cin.clear();
			std::cin.sync();
			Sleep(1000);
			continue;
		}
		break;
	}
}

void autoShipsPlacement(sea_field& field)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			field.table[i][j] = '~';

	for (int shipSize = 4; shipSize >= 1; shipSize--)
		for (int shipNumber = 5 - shipSize; shipNumber > 0; shipNumber--)
		{
			int dir;
			int i, j;

			while (true)
			{
				i = rand() % 10;
				j = rand() % 10;
				dir = rand() % 4;

				if (isShipPlaceable(i, j, dir, field, shipSize))
					break;
			}
			shipsPrint(i, j, dir, shipSize, field);
		}
}

void manualShipsPlacement(sea_field& field)
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			field.table[i][j] = '~';

	for (int shipSize = 4; shipSize >= 1; shipSize--)
		for (int shipNumber = 5 - shipSize; shipNumber > 0; shipNumber--)
		{
			int i, j, dir;

			while (true)
			{
				system("cls");
				printTable(field);
				int num;
				char let;
				std::cout << "Введите начальную координату (" << shipSize << "-палубник):" << std::endl;
				checkCoordinates(let, num);
				int startLet = let;
				int startNum = num;
				std::cout << "Введите конечную координату (" << shipSize << "-палубник):" << std::endl;
				checkCoordinates(let, num);
				int endLet = let;
				int endNum = num;

				if (startLet == endLet)
					dir = 1;
				else
					dir = 3;

				if (endNum - startNum == shipSize - 1 && startLet == endLet || endLet - startLet == shipSize - 1 && startNum == endNum)
				{
					i = startNum - 1;
					j = startLet - 'a';
					if (isShipPlaceable(i, j, dir, field, shipSize))
						break;
				}

				std::cout << "Некорректные координаты. Введите заново: " << std::endl;
				std::cin.clear();
				std::cin.ignore();
				Sleep(1000);
			}
			shipsPrint(i, j, dir, shipSize, field);
		}
}

sea_field hideTable(sea_field& field)
{
	sea_field hiddenTable{};

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (field.table[i][j] == '#')
				hiddenTable.table[i][j] = '~';
			else
				hiddenTable.table[i][j] = field.table[i][j];
		}
	return hiddenTable;
}

void replaceWithMiss(int i, int j, sea_field& table)
{
	if (i < 0 || i > 9 || j < 0 || j > 9)
		return ;
	for (int v = i - 1; v <= i + 1; v++)
		for (int b = j - 1; b <= j + 1; b++)
		{
			if (v < 0 || v > 9 || b < 0 || b > 9)
				continue;
			if (table.table[v][b] == '~')
				table.table[v][b] = '*';
		}
}

void destroyShipIfPossible(int i, int j, sea_field& table)
{
	int up = i;
	for (up = i; up >= 0; up--)
	{
		if (table.table[up][j] == '#')
			return;
		if (table.table[up][j] == '*' || table.table[up][j] == '~')
			break;
	}

	int down = i;
	for (down = i; down < 10; down++)
	{
		if (table.table[down][j] == '#')
			return;
		if (table.table[down][j] == '*' || table.table[down][j] == '~')
			break;
	}

	int left = j;
	for (left = j; left >= 0; left--)
	{
		if (table.table[i][left] == '#')
			return;
		if (table.table[i][left] == '*' || table.table[i][left] == '~')
			break;
	}

	int right = j;
	for (right = j; right < 10; right++)
	{
		if (table.table[i][right] == '#')
			return;
		if (table.table[i][right] == '*' || table.table[i][right] == '~')
			break;
	}

	for (int trav_up = i; trav_up > up; trav_up--)
		replaceWithMiss(trav_up, j, table);
	for (int trav_down = i; trav_down < down; trav_down++)
		replaceWithMiss(trav_down, j, table);
	for (int trav_left = j; trav_left > left; trav_left--)
		replaceWithMiss(i, trav_left, table);
	for (int trav_right = j; trav_right < right; trav_right++)
		replaceWithMiss(i, trav_right, table);
}

bool playerMove(sea_field& tableTwo)
{
	char col;
	int row;

	std::cout << "Введите координаты выстрела: " << std::endl;

	checkCoordinates(col, row);

	if (tableTwo.table[row - 1][col - 'a'] == '#')
	{
		tableTwo.table[row - 1][col - 'a'] = 'X';
		destroyShipIfPossible(row - 1, col - 'a', tableTwo);
		return true;
	}
	else
	{
		tableTwo.table[row - 1][col - 'a'] = '*';
		return false;
	}
}

bool aiMove(sea_field& table, int intellectMode)
{
	int i, j;

	while (true)
	{
		i = rand() % 10;
		j = rand() % 10;

		if (table.table[i][j] != 'X' && table.table[i][j] != '*')
			break;
	}

	if (intellectMode == 1)
	{
		if (table.table[i][j] == '#')
		{
			table.table[i][j] = 'X';
			destroyShipIfPossible(i, j, table);
			return true;
		}
		else
		{
			table.table[i][j] = '*';
			return false;
		}
	}
	else
		while (true)
			if (table.table[i][j] == '#')
			{
				table.table[i][j] = 'X';
				int dir = rand() % 4;
				destroyShipIfPossible(i, j, table);
				directionalCoordinateMove(i, j, dir);
			}
			else
				if (table.table[i][j] = '*')
					return false;
}

void printModTables(sea_field& tableOne, sea_field& tableTwo, int playerMode)
{
	if (playerMode == 1)
		printTables(tableOne, hideTable(tableTwo));
	else
		printTables(hideTable(tableOne), hideTable(tableTwo));
}

void gameLoop(int playerMode, int placementMode, int intellectMode)
{
	sea_field tableOne{};
	sea_field tableTwo{};

	if (playerMode == 1 && placementMode == 2)
		manualShipsPlacement(tableOne);
	else
		autoShipsPlacement(tableOne);

	autoShipsPlacement(tableTwo);

	while (!allShipsDestroyed(tableOne) && !allShipsDestroyed(tableTwo))
	{
		system("cls");

		printModTables(tableOne, tableTwo, playerMode);

		if (playerMode == 1)
		{
			while (playerMove(tableTwo))
			{
				system("cls");
				printModTables(tableOne, tableTwo, playerMode);
			}
		}
		else
		{
			std::cout << "Компьютер просчитывает ход";
			Sleep(300);
			std::cout << '.';
			Sleep(300);
			std::cout << '.';
			Sleep(300);
			std::cout << '.';

			while (aiMove(tableTwo, intellectMode))
			{
				system("cls");
				printModTables(tableOne, tableTwo, playerMode);

				std::cout << "Компьютер просчитывает ход";
				Sleep(300);
				std::cout << '.';
				Sleep(300);
				std::cout << '.';
				Sleep(300);
				std::cout << '.';
			}
		}

		system("cls");
		printModTables(tableOne, tableTwo, playerMode);

		std::cout << "Компьютер просчитывает ход";
		Sleep(300);
		std::cout << '.';
		Sleep(300);
		std::cout << '.';
		Sleep(300);
		std::cout << '.';

		while (aiMove(tableOne, intellectMode))
		{
			system("cls");
			printModTables(tableOne, tableTwo, playerMode);
			
			std::cout << "Компьютер просчитывает ход";
			Sleep(300);
			std::cout << '.';
			Sleep(300);
			std::cout << '.';
			Sleep(300);
			std::cout << '.';
		}
		
		if (allShipsDestroyed(tableOne))
		{
			std::cout << std::endl << "Игрок 2 победил!" << std::endl;
			break;
		}
		else if (allShipsDestroyed(tableTwo))
		{
			std::cout << std::endl << "Игрок 1 победил!" << std::endl;
			break;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(static_cast<unsigned int>(time(NULL)));
	system("color 3F");
	
	tutorial();

	std::string playerModeQuestion = "Выберите режим игры: \n";
	std::string playerModeHVSC = "1 - Человек против компьютера\n";
	std::string playerModeCVSC = "2 - Компьютер против компьютера\n";
	
	int playerMode = gameModeChoice(playerModeQuestion, playerModeHVSC, playerModeCVSC);

	std::string placementModeQuestion = "Выберите режим расстановки кораблей: \n";
	std::string placementModeAuto = "1 - Автоматически\n";
	std::string placementModeManually = "2 - Вручную\n";
	std::string none = " ";

	int placementMode;

	if (playerMode == 1)
		placementMode = gameModeChoice(placementModeQuestion, placementModeAuto, placementModeManually);
	else
		placementMode = gameModeChoice(placementModeQuestion, placementModeAuto, none);
	
	std::string intellectModeQuestion = "Выберите уровень интеллекта компьютера: \n";
	std::string intellectModeRandom = "1 - Случайные выстрелы\n";
	std::string intellectModeAI = "2 - Интеллектуальная игра\n";

	int intellectMode = gameModeChoice(intellectModeQuestion, intellectModeRandom, intellectModeAI);

	gameLoop(playerMode, placementMode, intellectMode);

	return 0;
}