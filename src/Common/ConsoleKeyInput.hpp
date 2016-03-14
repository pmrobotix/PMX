/*
 * KeyboardInput.hpp
 *
 *  Created on: 7 mars 2015
 *      Author: pmx
 */

#ifndef COMMON_CONSOLEKEYINPUT_HPP_
#define COMMON_CONSOLEKEYINPUT_HPP_

#include <termios.h>
#include <unistd.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

class ConsoleKeyInput //TODO rename utils
{
public:


//http://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux
	static char mygetch()
	{
		/*#include <unistd.h>   //_getch*/
		/*#include <termios.h>  //_getch*/
		char buf = 0;
		struct termios old = {0};
		fflush(stdout);
		if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
		if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
		if (read(0, &buf, 1) < 0)
			perror("read()");
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror("tcsetattr ~ICANON");
		//printf("%d\n", buf);
		return buf;
	}

	static void clearScreen()
	{
		std::cout << "\033[2J\033[1;1H";
		//autre solution : http://stackoverflow.com/questions/4062045/clearing-terminal-in-linux-with-c-code
		//write(1,"\E[H\E[2J",7);
	}

//to move the cursor to a specific row and column (where the top left corner is {1,1}).
	static void setPrintPos(int row, int col)
	{
		printf("\033[%d;%dH", row, col);
	}

	static void setPos(std::ostream& _os, const std::streamsize& _x, const std::streamsize& _y)
	{
		char tmp = _os.fill();

		if (_y > 0)
		{
			_os.fill('\n');
			_os.width(_y);
			_os << '\n';
		}
		if (_x > 0)
		{
			_os.fill(' ');
			_os.width(_x);
			_os << ' ';
		}
		_os.flush();
		_os.fill(tmp);
	}

//convert integer to a string
//Doesn't work for a<=0
	static std::string itoa(int a)
	{
		std::string ss = "";   //create empty string
		while (a)
		{
			int x = a % 10;
			a /= 10;
			char i = '0';
			i = i + x;
			ss = i + ss;      //append new character at the front of the string!
		}
		return ss;
	}

//COLOR CONSOLE
	/*
	 I just wanted to help people in this matter since I too went through this and had some problems with good answers related to this problem.

	 (and I also wanted to help Duoas for I've seen him struggling to tell people to stop using "system" calls. Good Job Duoas)

	 This is a list of codes used in C++ to change the text color:

	 black - 30
	 red - 31
	 green - 32
	 brown - 33
	 blue - 34
	 magenta - 35
	 cyan - 36
	 lightgray - 37

	 Now these are the basic colours.

	 Usage of "\033[":
	 This is to handle the console cursor. I do not have a complete reference so I ask people who know about it to comment with what I do not have.

	 * 'm' character at the end of each of the following sentences is used as a stop character, where the system should stop and parse the \033[ sintax.

	 \033[0m - is the default color for the console
	 \033[0;#m - is the color of the text, where # is one of the codes mentioned above
	 \033[1m - makes text bold
	 \033[1;#m - makes colored text bold**
	 \033[2;#m - colors text according to # but a bit darker
	 \033[4;#m - colors text in # and underlines
	 \033[7;#m - colors the background according to #
	 \033[9;#m - colors text and strikes it
	 \033[A - moves cursor one line above (carfull: it does not erase the previously written line)
	 \033[B - moves cursor one line under
	 \033[C - moves cursor one spacing to the right
	 \033[D - moves cursor one spacing to the left
	 \033[E - don't know yet
	 \033[F - don't know yet

	 \033[2K - erases everything written on line before this.

	 **(this could be usefull for those who want more colors. The efect given by bold will give a color change effect)

	 EXAMPLE:
	 std::string default_console = "\033[" + itoa(0) + "m";
	 for (int i = 30; i <= 37; i++)
	 {
	 std::string color = "\033[0;" + itoa(i) + "m";

	 std::cout << color << "test " << i << std::endl;

	 color = "\033[7;" + itoa(i) + "m";

	 std::cout << color << "test " << i << " bold" << std::endl;

	 std::cout << default_console << std::endl;

	 }

	 std::cout << "Done" << std::endl;
	 */
};

#endif /* COMMON_CONSOLEKEYINPUT_HPP_ */
