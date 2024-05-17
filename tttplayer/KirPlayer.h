#pragma once
#include "player.h"
#include <string>
#include <iostream>

class PointDanger {
    Point coord;
    unsigned int danger_level = 0;
public:
    PointDanger() : coord(Point(0, 0)), danger_level(0) {};
    void SetPoint(const Point& point) { coord = point; }
    void SetDanger(const unsigned int danger) { danger_level = danger; }
    Point GetPoint() const { return coord; }
    unsigned int GetDanger() const { return danger_level; }
    void check_danger(const Mark, const GameView& game);
    friend Point compare(const PointDanger& point_my, const PointDanger& point_enemy);
    void check(const int iter_x, const int iter_y, const Mark mark, const GameView& game);
};

Point compare(const PointDanger& point_my, const PointDanger& point_enemy);
int distance(const Point&, const Point&);


class KirPlayer : public Player {
    std::string _name;
    Point last_moves[2];

public:
    KirPlayer(const std::string& name) : _name(name) {
        last_moves[0] = Point(0, 0);
        last_moves[1] = Point(0, 0);
    }
    std::string get_name() const override { return _name; }
    Point play(const GameView& game) override;
    void assign_mark(Mark player_mark) override { /*does nothing*/ }
    void notify(const GameView&, const Event&) override;
private:
    void add_last_move(const MoveEvent&);
    PointDanger calculate_move(const Point&, const GameView&) const;
};