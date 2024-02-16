#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "windows.h"

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

void printDelay(std::string textForDelay)
{
	int delayPrint = 50;
	bool checkEnter = false;

	Sleep(delayPrint);

	for (int i = 0; i < textForDelay.length(); i++)
	{
		if (checkEnter == false)
		{
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
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

void gameModeChoice(std::string modeQuestion, std::string modeOne, std::string modeTwo)
{
	int choiceMode = 0;

	do
	{
		printDelay(modeQuestion);

		std::cout << modeOne << "\n" << modeTwo << std::endl;
		std::cin >> choiceMode;

		if (choiceMode == 1)
			//func();
			std::cout << "ок";
		else if (choiceMode == 2)
			//func2();
			std::cout << "ок";
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
	{
		for (int j = 0; j < 10; j++)
		{
			if (field.table[i][j] == '#')
				return false;
		}
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
			ClearInput();

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
	{
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
}

void manualShipsPlacement(sea_field& field)
{
	int i, j, num, shipSize;
	char let;
	
	for (shipSize = 4; shipSize >= 1; shipSize--)
	{
		for (int shipNumber = 5 - shipSize; shipNumber > 0; shipNumber--)
		{
			std::cout << "Введите начальную и конечную координату (" << shipSize << "-палубник):" << std::endl;
			checkCoordinates(let, num);
			int startLet = let;
			int startNum = num;
			checkCoordinates(let, num);
			int endLet = let;
			int endNum = num;

			if (startLet == endLet)
				if (endNum - startNum == shipSize - 1 && startLet == endLet || endLet - startLet == shipSize - 1 && startNum == endNum)
					for (i = startNum, j = startLet; i < endNum && j < endLet; i++, j++)
						if (isShipPlaceable(i, j, dir, field, shipSize))
							break;
		
			shipsPrint(i, j, dir, shipSize, field);
		}
	}
}


void gameLoop()
{
	sea_field tableOne{};
	sea_field tableTwo{};

	manualShipsPlacement(tableOne);
	autoShipsPlacement(tableTwo);


	while (!allShipsDestroyed(tableOne) || !allShipsDestroyed(tableTwo))
	{
		system("cls");

		sea_field hiddenTableTwo{};

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (tableTwo.table[i][j] == '#')
					hiddenTableTwo.table[i][j] = '~';
				else
					hiddenTableTwo.table[i][j] = tableTwo.table[i][j];
			}
		}

		
		printTables(tableOne, tableTwo); //hidden
		std::cout << "Введите координаты выстрела: " << std::endl;

		char col;
		int row;

		//стреляет компьютер

		checkCoordinates(col, row);

		if (tableTwo.table[row - 1][col - 'a'] == '#')
			tableTwo.table[row - 1][col - 'a'] = 'X';
		else
			tableTwo.table[row - 1][col - 'a'] = '*';
	}

	if (allShipsDestroyed(tableOne))
		std::cout << "Игрок 2 победил!" << std::endl;
	else if (allShipsDestroyed(tableTwo))
		std::cout << "Игрок 1 победил!" << std::endl;
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