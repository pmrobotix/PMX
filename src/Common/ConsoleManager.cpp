/*!
 * \file
 * \brief Implementation de la classe ConsoleManager.
 */

#include "ConsoleManager.hpp"

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ConsoleKeyInput.hpp"

using namespace std;

ConsoleManager::ConsoleManager()
		: tests_()
{
}

void ConsoleManager::add(FunctionalTest * test)
{
	this->tests_.push_back(test);

	//add args from func test.
	//test->configureConsoleArgs();
}

std::string * ConsoleManager::displayAvailableTests(std::string color, int selected)
{
	//int lindex = 0;
	//std::string default_console = "\033[0m";
	std::string* tab = new std::string[tests_.size() + 1];

	//display unit tests
	for (data_type::size_type i = 0; i < tests_.size(); i++)
	{
		std::cout << std::flush;
		/*
		 if (lindex == (int) i)
		 std::cout << color << std::flush;
		 else
		 std::cout << default_console << std::flush;
		 */
		if ((int) i <= selected && color != "")
			std::cout << color << std::flush;
		else
			std::cout << "\033[0m" << std::flush;
		std::cout << "  " << i + 1 << ". " << tests_[i]->name() << std::endl;
		ostringstream out;
		out << "  " << i + 1 << ". " << tests_[i]->name();
		tab[i + 1] = out.str();
	}
	return tab;
}

void ConsoleManager::displayMenuFunctionalTestsAndRun(int argc, char** argv)
{
	char cInput;

	ConsoleKeyInput::clearScreen();
	ConsoleKeyInput::setPrintPos(1, 1);

	std::string color = "\033[7;32m";
	std::string default_console = "\033[0m";
	int lindex = 0;

	std::string *tab = displayAvailableTests(color, 0);
	/*
	 *
	 std::string tab[tests_.size() + 1];
	 //display unit tests
	 for (data_type::size_type i = 0; i < tests_.size(); i++)
	 {
	 std::cout << std::flush;
	 if (lindex == (int) i)
	 std::cout << color << std::flush;
	 else
	 std::cout << default_console << std::flush;

	 std::cout << "  " << i + 1 << ". " << tests_[i]->name() << std::endl;
	 ostringstream out;
	 out << "  " << i + 1 << ". " << tests_[i]->name();
	 tab[i + 1] = out.str();
	 }
	 */

	do
	{
		cInput = ConsoleKeyInput::mygetch();
		switch (cInput)
		{
		case 65:
			//printf("Up arrow key!\n");
			if (lindex > 0)
			{
				ConsoleKeyInput::setPrintPos(lindex + 1, 1);
				std::cout << default_console << tab[lindex + 1] << "  " << std::flush;
				lindex--;
				ConsoleKeyInput::setPrintPos(lindex + 1, 1);
				std::cout << color << "> " << tab[lindex + 1] << std::flush;
			}
			break;
		case 66:
			if (lindex < (int) tests_.size() - 1)
			{
				//printf("Down arrow key!\n");
				ConsoleKeyInput::setPrintPos(lindex + 1, 1);
				std::cout << default_console << tab[lindex + 1] << "  " << std::flush;
				lindex++;
				ConsoleKeyInput::setPrintPos(lindex + 1, 1);
				std::cout << color << "> " << tab[lindex + 1] << std::flush;
			}
			break;
			/*
			 case 67:
			 printf("Right arrow key!\n");
			 break;
			 case 68:
			 printf("Left arrow key!\n");
			 break;*/
		case 10:
			//printf("Enter key!\n");
			break;
		case 127:
			//printf("BACK key!\n");
			cout << default_console << endl;
			exit(0);
			break;
		}

		usleep(1000);
	} while (cInput != 10);

	cout << default_console << flush;
	ConsoleKeyInput::clearScreen();

	executeTest(lindex + 1, argc, argv);
}

string ConsoleManager::displayMenuFirstArgu()
{
	string select;
	char cInput;
	std::string color = "\033[7;32m";
	std::string default_console = "\033[0m";

	//Request input parameters
	ConsoleKeyInput::clearScreen();
	ConsoleKeyInput::setPrintPos(3, 1);
	cout << "  (M)ATCHES" << flush;
	ConsoleKeyInput::setPrintPos(5, 1);
	cout << "  (T)ESTS" << flush;

	do
	{
		cInput = ConsoleKeyInput::mygetch();
		switch (cInput)
		{
		case 65:
			//printf("Up arrow key!\n");
			ConsoleKeyInput::setPrintPos(3, 1);
			cout << color << "> (M)ATCHES" << flush;
			ConsoleKeyInput::setPrintPos(5, 1);
			cout << default_console << "  (T)ESTS  " << flush;
			select = "m";
			break;
		case 66:
			//printf("Down arrow key!\n");
			ConsoleKeyInput::setPrintPos(3, 1);
			cout << default_console << "  (M)ATCHES" << flush;
			ConsoleKeyInput::setPrintPos(5, 1);
			cout << color << "> (T)ESTS  " << flush;
			select = "t";
			break;
			/*case 67:
			 printf("Right arrow key!\n");
			 break;
			 case 68:
			 printf("Left arrow key!\n");
			 break;*/
		case 10:
			//printf("Enter key!\n");
			break;
		case 127:
			//printf("BACK key!\n");
			cout << default_console << endl;
			cout << "Exit !\n" << endl;
			exit(0);
			break;
		}
		usleep(1000);
	} while (cInput != 10);
	cout << default_console << endl;
	ConsoleKeyInput::clearScreen();
	return select;
}

void ConsoleManager::run(uint nTest, int argc, char** argv)
{
	executeTest(nTest, argc, argv);
}

void ConsoleManager::executeTest(uint nTest, int argc, char** argv)
{

	if (nTest > 0 && nTest <= tests_.size())
	{
		tests_[nTest - 1]->run(argc, argv);
	}
	else
	{
		std::cout << "The N° must be between 0 and " << tests_.size() << std::endl;
	}
}
