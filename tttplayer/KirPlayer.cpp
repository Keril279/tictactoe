#include "KirPlayer.h"
#include <cstdlib>
#include <iostream>

static field_index_t rand_int(field_index_t min, field_index_t max) {
    return min + rand() % (max - min + 1);
}

int distance(const Point& point1, const Point& point2) {
    int numx = abs(point1.x - point2.x);
    int numy = abs(point1.y - point2.y);
    return numx > numy ? numx : numy;
}

Point KirPlayer::play(const GameView& game) {
    Boundary boundary = game.get_settings().field_size;
    int N = game.get_settings().win_length;
    int num_of_moves = game.get_state().number_of_moves;
    Point enemy_move = last_moves[0];
    Point my_move = last_moves[1];
    Point result;
    if (num_of_moves < 2) {
        if (num_of_moves == 0) {
            result = {
            .x = rand_int(boundary.min.x + N / 2, boundary.max.x - N / 2),
            .y = rand_int(boundary.min.y + N / 2, boundary.max.y - N / 2),
            };
            return result;
        }
        else {
            result = calculate_move(enemy_move, game).GetPoint();
            return result;
        }
    }
    PointDanger point_enemy = calculate_move(enemy_move, game);
    PointDanger point_my = calculate_move(my_move, game);
    result = compare(point_my, point_enemy);
    return result;
}

Point compare(const PointDanger& point_my, const PointDanger& point_enemy) {
    int i = 1;
    while (point_my.GetDanger() / i != 0 and point_enemy.GetDanger() / i != 0) {
        i *= 10;
    }
    i /= 10;
    if (i == 0) {
        i = 1;
    }
    if (point_my.GetDanger() / i >= point_enemy.GetDanger() / i) {
        return point_my.GetPoint();
    }
    else {
        return point_enemy.GetPoint();
    }
}

PointDanger KirPlayer::calculate_move(const Point& point, const GameView& game) const {
    int N = game.get_settings().win_length;
    Mark mark = game.get_state().field.get()->get_value(point);
    Mark enemy_mark = mark == Mark::Cross ? Mark::Zero : Mark::Cross;
    Boundary boundary = game.get_settings().field_size;
    PointDanger result;
    PointDanger current_point;
    Point center((boundary.min.x + boundary.max.x) / 2, (boundary.min.y + boundary.max.y) / 2);
    bool turn_was_made = 0;
    for (int i = point.x - (N - 1); i <= point.x + (N - 1); i++) {
        for (int j = point.y - (N - 1); j <= point.y + (N - 1); j++) {
            current_point.SetPoint(Point(i, j));
            current_point.SetDanger(0);
            if ((boundary.is_within(current_point.GetPoint())) and (game.get_state().field.get()->get_value(current_point.GetPoint()) == Mark::None)) {
                current_point.check_danger(mark, game);
                current_point.check_danger(enemy_mark, game);
                if (current_point.GetDanger() > result.GetDanger()) {
                    turn_was_made = 1;
                    result.SetDanger(current_point.GetDanger());
                    result.SetPoint(current_point.GetPoint());
                }
                else if (current_point.GetDanger() == result.GetDanger()) {
                    if (distance(current_point.GetPoint(), point) < distance(result.GetPoint(), point)) {
                        turn_was_made = 1;
                        result.SetDanger(current_point.GetDanger());
                        result.SetPoint(current_point.GetPoint());
                    }
                    else if (distance(current_point.GetPoint(), point) == distance(result.GetPoint(), point)) {
                        if (distance(current_point.GetPoint(), center) < distance(result.GetPoint(), center)) {
                            turn_was_made = 1;
                            result.SetDanger(current_point.GetDanger());
                            result.SetPoint(current_point.GetPoint());
                        }
                    }
                }
            }
        }
    }
    if (turn_was_made == 0) {
        for (int i = game.get_settings().field_size.min.x; i < game.get_settings().field_size.max.x; i++) {
            for (int j = game.get_settings().field_size.min.y; j < game.get_settings().field_size.max.y; j++) {
                current_point.SetPoint(Point(i, j));
                current_point.SetDanger(0);
                if ((boundary.is_within(current_point.GetPoint())) and (game.get_state().field.get()->get_value(current_point.GetPoint()) == Mark::None)) {
                    current_point.check_danger(mark, game);
                    current_point.check_danger(enemy_mark, game);
                    if (current_point.GetDanger() > result.GetDanger()) {
                        turn_was_made = 1;
                        result.SetDanger(current_point.GetDanger());
                        result.SetPoint(current_point.GetPoint());
                    }
                    else if (current_point.GetDanger() == result.GetDanger()) {
                        if (distance(current_point.GetPoint(), point) < distance(result.GetPoint(), point)) {
                            turn_was_made = 1;
                            result.SetDanger(current_point.GetDanger());
                            result.SetPoint(current_point.GetPoint());
                        }
                        else if (distance(current_point.GetPoint(), point) == distance(result.GetPoint(), point)) {
                            if (distance(current_point.GetPoint(), center) < distance(result.GetPoint(), center)) {
                                turn_was_made = 1;
                                result.SetDanger(current_point.GetDanger());
                                result.SetPoint(current_point.GetPoint());
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}
void PointDanger::check(const int iter_x, const int iter_y, const Mark mark, const GameView& game) {
    Boundary boundary = game.get_settings().field_size;
    int N = game.get_settings().win_length;
    Mark enemy_mark = mark == Mark::Cross ? Mark::Zero : Mark::Cross;
    Point point_a;
    Point point_b;
    Point* a = &point_a;
    Point* b = &point_b;
    int i;
    bool flag_a = 1, flag_b = 1;
    int tmp = 1;
    point_a.x = this->GetPoint().x + iter_x;
    point_a.y = this->GetPoint().y + iter_y;
    point_b.x = this->GetPoint().x - iter_x;
    point_b.y = this->GetPoint().y - iter_y;
    i = 0;
    while (boundary.is_within(*a) and i < N) {
        if (game.get_state().field.get()->get_value(*a) != enemy_mark) {
            i++;
            point_a.x += iter_x;
            point_a.y += iter_y;
        }
        else {
            break;
        }
    }
    while (boundary.is_within(*b) and i < N) {
        if (game.get_state().field.get()->get_value(*b) != enemy_mark) {
            i++;
            point_b.x -= iter_x;
            point_b.y -= iter_y;
        }
        else {
            break;
        }
    }
    if (i >= N) {
        point_a.x = this->GetPoint().x + iter_x;
        point_a.y = this->GetPoint().y + iter_y;
        point_b.x = this->GetPoint().x - iter_x;
        point_b.y = this->GetPoint().y - iter_y;
        if (not boundary.is_within(*a)) {
            flag_a = 0;
        }
        else if (game.get_state().field.get()->get_value(*a) == enemy_mark) {
            flag_a = 0;
        }
        if (not boundary.is_within(*b)) {
            flag_b = 0;
        }
        else if (game.get_state().field.get()->get_value(*b) == enemy_mark) {
            flag_b = 0;
        }
        while (flag_a or flag_b) {
            if (flag_a) {
                if (game.get_state().field.get()->get_value(*a) == mark) {
                    tmp *= 10;
                }
                point_a.x += iter_x;
                point_a.y += iter_y;
                if (not boundary.is_within(*a)) {
                    flag_a = 0;
                }
                else if (game.get_state().field.get()->get_value(*a) == enemy_mark) {
                    flag_a = 0;
                }
                else if (distance(*a, GetPoint()) > N - 1) {
                    flag_a = 0;
                }
            }
            if (flag_b) {
                if (game.get_state().field.get()->get_value(*b) == mark) {
                    tmp *= 10;
                }
                point_b.x -= iter_x;
                point_b.y -= iter_y;
                if (not boundary.is_within(*b)) {
                    flag_b = 0;
                }
                else if (game.get_state().field.get()->get_value(*b) == enemy_mark) {
                    flag_b = 0;
                }
                else if (distance(*b, GetPoint()) > N - 1) {
                    flag_b = 0;
                }
            }
        }
        danger_level += (tmp / 10) * 10;
        tmp = 1;
        while (flag_a) {
            if (game.get_state().field.get()->get_value(*a) == mark) {
                tmp *= 10;
            }
            point_a.x++;
            if (not boundary.is_within(*a)) {
                flag_a = 0;
            }
            else if (game.get_state().field.get()->get_value(*a) == enemy_mark) {
                flag_a = 0;
            }
            else if (distance(*a, GetPoint()) > N - 1) {
                flag_a = 0;
            }
        }
        danger_level += (tmp / 10) * 10;
        tmp = 1;
        while (flag_b) {
            if (game.get_state().field.get()->get_value(*b) == mark) {
                tmp *= 10;
            }
            point_b.x--;
            if (not boundary.is_within(*b)) {
                flag_b = 0;
            }
            else if (game.get_state().field.get()->get_value(*b) == enemy_mark) {
                flag_b = 0;
            }
            else if (distance(*b, GetPoint()) > N - 1) {
                flag_b = 0;
            }
        }
        danger_level += (tmp / 10) * 10;
        tmp = 1;
    }
}
void PointDanger::check_danger(const Mark mark, const GameView& game) {
    this->check(1, 0, mark, game);
    this->check(0, 1, mark, game);
    this->check(1, 1, mark, game);
    this->check(1, -1, mark, game);
    return;
}
void KirPlayer::notify(const GameView&, const Event& event) {
    if (event.get_type() == MoveEvent::TYPE) {
        auto& data = get_data<MoveEvent>(event);
        add_last_move(data);
        return;
    }
}
void KirPlayer::add_last_move(const MoveEvent& data) {
    last_moves[1] = last_moves[0];
    last_moves[0] = data.point;
}