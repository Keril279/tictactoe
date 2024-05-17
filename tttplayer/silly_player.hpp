#pragma once
#include "player.h"
#include <string>

class SimplePlayer : public Player {
  std::string _name;
  Point _last_move;
  Mark _my_mark;

  bool _has_winning_move(const GameView& game, Point& result);
  bool _can_block_last_move(const GameView& game, Point& result);
  Point _choose_random(const GameView& game);

public:
  SimplePlayer(const std::string &name) : _name(name) {}
  std::string get_name() const override { return _name; }
  Point play(const GameView &game) override;
  void assign_mark(Mark player_mark) override;
  void notify(const GameView& view, const Event& event) override;
};
