/**
 *  This is example of game, that you can use for debugging.
 */
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "my_player.h"
#include "KirPlayer.h"
#include "game_engine.h"
#include "silly_player.hpp"


int main() {
    srand(time(NULL));

    /// Creates observer to log every event to std::cout
    BasicObserver obs(std::cout);

    /// Create setting for game
    GameSettings settings = {
        .field_size = {
            .min = {.x = -10, .y = -10},
            .max = {.x = 10, .y = 10},
        },
        .max_moves = 0,
        .win_length = 5,
        .move_timelimit_ms = 0,
    };
    int count1 = 0;
    int count2 = 0;
    int N = 1;
    for (int i = 0; i < N; i++) {
        /// Creates game engine with given settings
        GameEngine game(settings);

        /// Adds observer to the view of this game engine
        game.get_view().add_observer(obs);
        /// Creates first player
        KirPlayer player1("player 1");
        /// Adds it to play Xs
        game.set_player(player1);
        /// Create second player
        KirPlayer player2("player 2");
        /// Adds it to play Os
        game.set_player(player2);
        /// Starts game until someone wins.

        Mark win = game.play_game();
        if (win == Mark::Zero) {
            std::cout << i + 1 << ") Zero" << std::endl;
            count1++;
        }
        else if (win == Mark::Cross) {
            std::cout << i + 1 << ") Cross" << std::endl;
            count2++;
        }
    }
    std::cout << "X: " << count2 << ", " << float(count2) / float(N) *100 << "% \n" << "O: " << count1 << ", " << float(count1) / float(N) * 100 << "% \n" << "Draws: " << N - count1 - count2 << ", " << float(N - count1 - count2) / float(N) * 100 << "%" << std::endl;
    return 0;
}
