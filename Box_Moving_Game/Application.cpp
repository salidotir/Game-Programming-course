/**
  **********************************************************************************************
  *								    	BOX MOVING GAME										   *
  **********************************************************************************************
  **********************************************************************************************
  Homework of game programming of Shiraz University
  Author: Sara Limooee (salidotir)
  STD number: 9632421
  **********************************************************************************************
  
  ***************************************
  *	    	  Documentation			    *
  ***************************************
  ***************************************
  *	    	   How To Play			    *
  ***************************************
  Try to put boxes, "#", in the holes "_" by as least as possible moves.
  If finished, press ESC button to save your job.
  Enjoy!
 A text file containing of levels is given to the game as input to create the game screen.
 

  ***************************************
  *	    	   Game Structure		    *
  ***************************************
  The structure of the file is as below:
 
  ggwggggggggggggggggg
  ggwggggggg#ggggggggg
  ggwggggggggggggggggg
  ggwggggggggggggggggg
  ggggggggggggwwwwwggg
  ggggggggggggggg_gggg
  gggggggggggggggggggg
  
  g -> is the ground showing by '.' in the screen.
  w -> is the wall showing by '|' in the screen.
  # -> is the box that must be put in the holes showing by '#' in the screen.
  _ -> is the hole the boxes must be put in showing by '_' in the screen.


  ***************************************
  *	    	   Implementation		    *
  ***************************************
  Below parts are implemented individually for this game:
  Actor:
	* which can be any objects in the game such as:
		* Ground
		* Actor
		* Box
		* Hole
		* Wall
		that all inherits GameObject.
	
  Input Handler:
	* Gets user input and pass it to Command executer.

  Command:
	* It has a MoveCommand class which inherits Command and implements virtual function execute.
	* execute():
		* Get the actor and the move and execute the move and change the game-screen.
	* undo():
		* It will undo only the actor move, '*'.

  Game:
	* It contains the game-screen, number of moves, name of player and loop function of game.
	* It starts the game by calling:
		* start_game() to get player name
		* fill_game_screen_array() to create game-screen from input file.

  **********************************************************************************************
  **********************************************************************************************
*/


#include <iostream>
#include <stdlib.h>
#include <stack>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define KEY_UP		72		// arrow up
#define KEY_DOWN	80		// arrow down
#define KEY_LEFT	77		// arrow left
#define KEY_RIGHT	75		// arrow right
#define KEY_ESC		27		// escape
#define KEY_SPACE	32		// space

namespace BoxMovingGame
{
#pragma region [Actor]

	class GameObject {
	private:
		char symbol;
		int x = 0;
		int y = 0;

	public:
		GameObject() {}

		GameObject(char symbol, int x, int y) {
			this->symbol = symbol;
			this->x = x;
			this->y = y;
		}

		char get_symbol() {
			return this->symbol;
		}

		char get_x() {
			return this->x;
		}

		char get_y() {
			return this->y;
		}

		bool x_add_1() {
			this->x += 1;
			return true;
		}

		bool x_minus_1() {
			this->x -= 1;
			return true;
		}

		bool y_add_1() {
			this->y += 1;
			return true;
		}

		bool y_minus_1() {
			this->y -= 1;
			return true;
		}
	};

	class GameGround : public GameObject {
	public:
		GameGround(int x, int y)
			:GameObject{ '.', x, y } { }
	};

	class GameActor : public GameObject{
	public:
		GameActor(int x, int y)
			:GameObject{ '*', x, y } { }
	};

	class GameBox : public GameObject {
	public:
		GameBox(int x, int y)
			:GameObject{ '#', x, y } { }
	};

	class GameHole : public GameObject {
	public:
		GameHole(int x, int y)
			:GameObject{ '_', x, y } { }
	};

	class GameWall : public GameObject {
	public:
		GameWall(int x, int y)
			:GameObject{ '|', x, y } { }
	};

#pragma endregion


#pragma region [Input Handlers]

	class InputHandler {
	public:
		// returns the number of move it should do
		static int handle_input() {
			
			char choice = _getch();				// first char read is -1!!
			choice = _getch();

			switch (choice)
			{
			case KEY_UP:
				return KEY_UP;
				break;

			case KEY_DOWN:
				return KEY_DOWN;
				break;

			case KEY_LEFT:
				return KEY_LEFT;
				break;

			case KEY_RIGHT:
				return KEY_RIGHT;
				break;

			case KEY_SPACE:
				return KEY_SPACE;
				break;

			case KEY_ESC:
				return KEY_ESC;
				break;

			default:
				return handle_input();
				break;
			}
		}
	};

#pragma endregion


#pragma region [Command]

	class Command {
	public:
		virtual bool execute(GameObject** game_screen, GameObject* game_object, int move, string name, int number_of_moves) = 0;
	};

	class MoveCommand : public Command {
	private:
		stack<int> prev_moves;

		void end_game(string name, int number_of_moves) {
			std::ofstream score_file;
			// save recored of name & its number_of_moves in the "score.txt" file
			score_file.open("score.txt", std::ios_base::app); // append instead of overwrite
			score_file << name << "  |  " << number_of_moves << endl;
			score_file.close();
		}

	public:
		MoveCommand() {
			this->prev_moves.push(-1);
		}

		bool is_move_valid(GameObject** game_screen, GameObject game_object, int move) {
			switch (move)
			{
			case KEY_UP:
				if (game_object.get_x() > 0) {
					switch (game_screen[game_object.get_x()-1][game_object.get_y()].get_symbol())
					{
					case '#':		// if it was a box above game_actor
						if (is_move_valid(game_screen, game_screen[game_object.get_x() - 1][game_object.get_y()], move) == true) {
							game_screen[game_object.get_x() - 1][game_object.get_y()].x_minus_1();
							GameObject tmp = game_screen[game_object.get_x() - 1][game_object.get_y()];
							game_screen[game_object.get_x() - 2][game_object.get_y()] = game_screen[game_object.get_x() - 1][game_object.get_y()];
							game_screen[game_object.get_x() - 1][game_object.get_y()] = GameGround(game_object.get_x() - 1, game_object.get_y());
							return true;
						}
						else
							return false;
						break;

					case '|':		// if it was a wall above game_actor
						return false;
						break;

					default:
						return true;
						break;
					}
				}
				else
					return false;
				break;

			case KEY_DOWN:
				if (game_object.get_x() < 19) {
					switch (game_screen[game_object.get_x() + 1][game_object.get_y()].get_symbol())
					{
					case '#':		// if it was a box below game_actor
						if (is_move_valid(game_screen, game_screen[game_object.get_x() + 1][game_object.get_y()], move) == true) {
							game_screen[game_object.get_x() + 1][game_object.get_y()].x_add_1();
							GameObject tmp = game_screen[game_object.get_x() + 1][game_object.get_y()];
							game_screen[game_object.get_x() + 2][game_object.get_y()] = game_screen[game_object.get_x() + 1][game_object.get_y()];
							game_screen[game_object.get_x() + 1][game_object.get_y()] = GameGround(game_object.get_x() + 1, game_object.get_y());
							return true;
						}
						else
							return false;
						break;

					case '|':		// if it was a wall below game_actor
						return false;
						break;

					default:
						return true;
						break;
					}
				}
				else
					return false;
				break;

			case KEY_LEFT:
				if (game_object.get_y() < 19) {
					switch (game_screen[game_object.get_x()][game_object.get_y() + 1].get_symbol())
					{
					case '#':		// if it was a box left of game_actor
						if (is_move_valid(game_screen, game_screen[game_object.get_x()][game_object.get_y() + 1], move) == true) {
							game_screen[game_object.get_x()][game_object.get_y() + 1].y_add_1();
							GameObject tmp = game_screen[game_object.get_x()][game_object.get_y() + 1];
							game_screen[game_object.get_x()][game_object.get_y() + 2] = game_screen[game_object.get_x()][game_object.get_y() + 1];
							game_screen[game_object.get_x()][game_object.get_y() + 1] = GameGround(game_object.get_x(), game_object.get_y() + 1);
							return true;
						}
						else
							return false;
						break;

					case '|':		// if it was a wall left of game_actor
						return false;
						break;

					default:
						return true;
						break;
					}
				}
				else
					return false;
				break;

			case KEY_RIGHT:
				if (game_object.get_y() > 0) {
					switch (game_screen[game_object.get_x()][game_object.get_y() - 1].get_symbol())
					{
					case '#':		// if it was a box right of game_actor
						if (is_move_valid(game_screen, game_screen[game_object.get_x()][game_object.get_y() - 1], move) == true) {
							game_screen[game_object.get_x()][game_object.get_y() - 1].y_minus_1();
							GameObject tmp = game_screen[game_object.get_x()][game_object.get_y() - 1];
							game_screen[game_object.get_x()][game_object.get_y() - 2] = game_screen[game_object.get_x()][game_object.get_y() - 1];
							game_screen[game_object.get_x()][game_object.get_y() - 1] = GameGround(game_object.get_x(), game_object.get_y() - 1);
							return true;
						}
						else
							return false;
						break;

					case '|':		// if it was a wall right of game_actor
						return false;
						break;

					default:
						return true;
						break;
					}
				}
				else
					return false;
				break;

			default:
				return false;
				break;
			}
		}

		virtual bool execute(GameObject** game_screen, GameObject* game_object, int move, string name, int number_of_moves) {
			this->prev_moves.push(move);
			
			switch (move)
			{
			case KEY_UP:
				if(is_move_valid(game_screen, *game_object, KEY_UP))
					return game_object->x_minus_1();
				break;

			case KEY_DOWN:
				if (is_move_valid(game_screen, *game_object, KEY_DOWN))
					return game_object->x_add_1();
				break;

			case KEY_LEFT:
				if (is_move_valid(game_screen, *game_object, KEY_LEFT))
					return game_object->y_add_1();
				break;

			case KEY_RIGHT:
				if (is_move_valid(game_screen, *game_object, KEY_RIGHT))
					return game_object->y_minus_1();
				break;

			case KEY_SPACE:
				cout << "undo pressed!" << endl;
				this->prev_moves.pop();
				this->undo(game_object);
				return true;
				break;

			case KEY_ESC:
				// end_game and save score
				end_game(name, number_of_moves);
				cout << "Game terminated..." << endl;
				exit(2);
				break;

			default:
				return false;
				break;
			}
		}
		
		// undo only undos game_actor move!
		virtual void undo(GameObject* game_object) {
			// pop previos move and check it
			int prev_move = this->prev_moves.top();
			if (prev_move != -1) {
				this->prev_moves.pop();
				switch (prev_move)
				{
				case KEY_UP:
					game_object->x_add_1();
					break;

				case KEY_DOWN:
					game_object->x_minus_1();
					break;

				case KEY_LEFT:
					game_object->y_minus_1();
					break;

				case KEY_RIGHT:
					game_object->y_add_1();
					break;

				default:
					break;
				}
			}
			else
				cout << "No previous move." << endl;
		}
	};

#pragma endregion

#pragma region [Game]

	class Game {
	private:
		string name = "";
		int number_of_moves = 0;
		GameObject** game_screen;
		int x_size = 20, y_size = 20;

		void fill_game_screen_array(string game_addr) {
			ifstream file(game_addr);
			int line_cnt = 0;
			if (file.is_open()) {
				string line;
				line_cnt = 0;
				while (std::getline(file, line)) {
					for (int i = 0; i < strlen(line.c_str()); i++) {
						switch (line.c_str()[i])
						{
						case 'g':
							this->game_screen[line_cnt][i] = GameGround(line_cnt, i);
							break;

						case 'w':
							this->game_screen[line_cnt][i] = GameWall(line_cnt, i);
							break;

						case '#':
							this->game_screen[line_cnt][i] = GameBox(line_cnt, i);
							break;

						case '_':
							this->game_screen[line_cnt][i] = GameHole(line_cnt, i);
							break;

						default:
							break;
						}
					}
					line_cnt++;
				}
				file.close();
			}
		}

	public:
		Game(string game_addr) {
			game_screen = new GameObject * [20];
			for (int i = 0; i < 20; i++)
				game_screen[i] = new GameObject[20];
			fill_game_screen_array(game_addr);
		}

		~Game() {
			//delete game_screen;
		}

		void start_game() {
			string i_name;
			cout << "Enter your name: ";
			cin >> i_name;
			this->name = i_name;
			this->number_of_moves = 0;
		}

		void show_screen(GameActor game_actor) {
			system("cls");
			
			printf("\n");

			cout << "Name: " << this->name << endl;
			cout << "number of moves: " << this->number_of_moves << endl;
			
			for (int i = 0; i < this->x_size; i++)
			{
				for (int j = 0; j < this->y_size; j++)
				{
					if (i == game_actor.get_x() && j == game_actor.get_y())
						printf("\x1B[31m%c\033[0m", game_actor.get_symbol());
					else {
						switch (this->game_screen[i][j].get_symbol())
						{
						case '#':
							printf("\x1B[96m%c\033[0m", this->game_screen[i][j].get_symbol());
							break;

						default:
							printf("%c", this->game_screen[i][j].get_symbol());
							break;
						}
					}
				}
				putchar('\n');
			}
		}

		void loop(GameActor* game_actor) {
			InputHandler input_handler;
			MoveCommand move_command;

			bool if_moved = true;

			while (1) {
				show_screen(*game_actor);

				int move = input_handler.handle_input();

				if_moved = move_command.execute(game_screen, game_actor, move, name, number_of_moves);
				if(if_moved == true)
					this->number_of_moves++;
			}

		}
	};

#pragma endregion

}


int main() {
	BoxMovingGame::Game game("level1.txt");
	BoxMovingGame::GameActor game_actor(0, 0);
	game.start_game();
	game.loop(&game_actor);

	return 0;
}