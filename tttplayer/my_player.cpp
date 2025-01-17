#include "my_player.h"
#include <cstdlib>
#include <iostream>

static field_index_t rand_int(field_index_t min, field_index_t max) {
    return min + rand() % (max - min + 1);
}

Point RandomPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Point result;
    do {
        result = {
            .x = rand_int(b.min.x, b.max.x),
            .y = rand_int(b.min.y, b.max.y),
        };
    } while(game.get_state().field->get_value(result) != Mark::None);
    return result;
}

Point HandPlayer::play(const GameView& game) {
    Boundary b = game.get_settings().field_size;
    Point result;
    int x;
    int y;
    do {
        std::cout << "Enter your move" << std::endl;
        std::cin >> x;
        std::cin >> y;
        result = {
            .x = x,
            .y = y,
        };
    } while (game.get_state().field->get_value(result) != Mark::None);
    return result;
}

void BasicObserver::notify(const GameView& game, const Event& event) {
    if (event.get_type() == MoveEvent::TYPE) {
        auto &data = get_data<MoveEvent>(event);
        _out << "Move:\tx = " <<  data.point.x 
            << ",\ty = " << data.point.y << ":\t";
        _print_mark(data.mark) << '\n';
        Boundary boundary = game.get_settings().field_size;
        for (int y = boundary.min.y; y < boundary.max.y + 1; y++) {
            for (int x = boundary.min.x; x < boundary.max.x + 1; x++) {
                Mark mark = game.get_state().field->get_value(Point(x, y));            std::cout << "|_" << (mark == Mark::None ? '_' : (mark == Mark::Zero ? 'O' : 'X'));
            } std::cout << std::endl;
        }

        return;
    }
    if (event.get_type() == PlayerJoinedEvent::TYPE) {
        auto &data = get_data<PlayerJoinedEvent>(event);
        _out << "Player '" << data.name << "' joined as ";
        _print_mark(data.mark) << '\n';
        return;
    }
    if (event.get_type() == GameStartedEvent::TYPE) {
        _out << "Game started\n";
        return;
    }
    if (event.get_type() == WinEvent::TYPE) {
        auto &data = get_data<WinEvent>(event);
        _out << "Player playing ";
        _print_mark(data.winner_mark) << " has won\n";
        return;
    }
    if (event.get_type() == DrawEvent::TYPE) {
        auto &data = get_data<DrawEvent>(event);
        _out << "Draw happened, reason: '" << data.reason << "'\n";
        return;
    }
    if (event.get_type() == DisqualificationEvent::TYPE) {
        auto &data = get_data<DisqualificationEvent>(event);
        _out << "Player playing ";
        _print_mark(data.player_mark) << " was disqualified, reason: '" 
            << data.reason << "'\n";
        return;
    }
}

std::ostream& BasicObserver::_print_mark(Mark m) {
    if (m == Mark::Cross) return _out << "X";
    if (m == Mark::Zero) return _out << "O";
    return _out << "?";
}


