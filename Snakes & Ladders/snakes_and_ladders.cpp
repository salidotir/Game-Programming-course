#include <iostream>
#include <stdlib.h>

using namespace std;

namespace SnakesLaddersGame
{
#pragma region [Dice]
    class Dice {
    private:
        int value;
    
    public:
        Dice() {
            value = 0;
        }

        int get_value() {
            return this->value;
        }

        int roll_dice() {
            return rand() % 6 + 1;
        }
    };
#pragma endregion

#pragma region [Board]
    class Board {
        public:
            Board() {}
    
            int next_place(int curr_place) {
                switch (curr_place) {
                    case  4:  return 14;
                    case  9:  return 31;
                    case 17:  return  7;
                    case 20:  return 38;
                    case 28:  return 84;
                    case 40:  return 59;
                    case 51:  return 67;
                    case 54:  return 34;
                    case 62:  return 19;
                    case 63:  return 81;
                    case 64:  return 60;
                    case 71:  return 91;
                    case 87:  return 24;
                    case 93:  return 73;
                    case 95:  return 75;
                    case 99:  return 78;
                    default: return curr_place;
                }
            }
    };
#pragma endregion

#pragma region [Game]
    class Game {
        private:
            Dice* dice;
            Board* board;
        
        public:
            Game() {
                dice = new Dice();
                board = new Board();
            }

            int play_turn(int player_number, int player_curr_place) {
                int dice_value, next_place;
                while (true) {
                    dice_value = this->dice->roll_dice();
                    printf("player number %d is on %d, dice-value is %d", player_number, player_curr_place, dice_value);
                    if (player_curr_place + dice_value > 100) {
                        printf(", but cannot move.\n");
                    } else {
                        player_curr_place += dice_value;
                        printf(", moves to %d.\n", player_curr_place);
                        if (player_curr_place == 100) return 100;
                        next_place = this->board->next_place(player_curr_place);
                        if (player_curr_place < next_place) {
                            printf("climb up a ladder to %d.\n", next_place);
                            player_curr_place = next_place;
                        } else if (next_place < player_curr_place) {
                            printf("climb down a snake to %d.\n", next_place);
                            player_curr_place = next_place;
                        }
                    }
                    // 6 has a reward.
                    if (dice_value < 6) return player_curr_place;
                    printf("roll again.\n");
                }
            }

            void loop() {
                // indexes are players numbers.
                // values are players current places.
                int players[3] = { 1, 1, 1 };
                int index, next_place;

                while (true) {
                    for (index = 0; index < sizeof(players) / sizeof(int); index++) {
                        next_place = this->play_turn(index + 1, players[index]);
                        if (next_place == 100) {
                            printf("player number %d wins!\n", index + 1);
                            return;
                        }
                        players[index] = next_place;
                        printf("\n");
                    }
                }
            }
    };
#pragma endregion

}

int main() {
    SnakesLaddersGame::Game game;
    game.loop();
    return 0;
}