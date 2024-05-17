#include "silly_player.hpp"

Point SimplePlayer::play(const GameView &game) {
  const bool is_first_move = game.get_state().number_of_moves == 0;
  /* sleep(1); */
  if (is_first_move) {
    const Boundary &b = game.get_settings().field_size;
    return Point{
        .x = (b.min.x + b.max.x) / 2,
        .y = (b.min.y + b.max.y) / 2,
    };
  }
  Point result;
  if (_has_winning_move(game, result)) {
    return result;
  }
  if (_can_block_last_move(game, result)) {
    return result;
  }
  return _choose_random(game);
}

bool SimplePlayer::_has_winning_move(const GameView &game, Point &result) {
  const Boundary &b = game.get_settings().field_size;
  struct {
    int dx;
    int dy;
  } dirs[] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
  const int win_len = game.get_settings().win_length;
  for (Point p = b.min; p.y <= b.max.y; ++p.y) {
    for (p.x = b.min.x; p.x <= b.max.x; ++p.x) {
      if (game.get_state().field->get_value(p) != Mark::None) {
        continue;
      }
      for (int d_ix = 0; d_ix < 4; ++d_ix) {
        int dx = dirs[d_ix].dx, dy = dirs[d_ix].dy;
        Point p2 = Point{.x = p.x + dx * win_len, .y = p.y + dy * win_len};
        if (!b.is_within(p2))
          continue;
        int n_ours = 0, n_free = 0;
        Point only_free_point;
        for (Point p3 = p; p3.x <= p2.x && p3.y <= p2.y; ++p3.x, ++p3.y) {
          Mark val = game.get_state().field->get_value(p3);
          if (val == _my_mark) {
            ++n_ours;
          } else if (val == Mark::None) {
            ++n_free;
            only_free_point = p3;
          } else {
            break;
          }
        }
        if (n_ours == win_len - 1 && n_free == 1) {
          result = only_free_point;
          return true;
        }
      }
    }
  }
  return false;
}

bool SimplePlayer::_can_block_last_move(const GameView &game, Point &result) {
  const Boundary &b = game.get_settings().field_size;
  struct {
    int dx;
    int dy;
  } dirs[] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
  bool found_best_point = false;
  int max_len = 0;
  Point best_point;
  Mark other_mark = game.get_state().field->get_value(_last_move);
  for (int d_ix = 0; d_ix < 4; ++d_ix) {
    int dx = dirs[d_ix].dx, dy = dirs[d_ix].dy;
    int len_f = 0, len_b = 0;
    bool can_close = false;
    Point point_to_close;
    for (Point p = _last_move; b.is_within(p) && len_f <= 5;
         p.x += dx, p.y += dy) {
      Mark val = game.get_state().field->get_value(p);
      if (val == other_mark) {
        len_f++;
      } else if (val == Mark::None) {
        can_close = true;
        point_to_close = p;
        break;
      } else {
        break;
      }
    }
    for (Point p = _last_move; b.is_within(p) && len_b <= 5;
         p.x -= dx, p.y -= dy) {
      Mark val = game.get_state().field->get_value(p);
      if (val == other_mark) {
        len_b++;
      } else if (val == Mark::None) {
        can_close = true;
        point_to_close = p;
        break;
      } else {
        break;
      }
    }
    if (!can_close)
      continue;
    int len = len_f + len_b - 1;
    if (len > max_len) {
      max_len = len;
      best_point = point_to_close;
      found_best_point = true;
    }
  }
  if (found_best_point) {
    result = best_point;
    return true;
  }
  return false;
}

static field_index_t rand_int(field_index_t min, field_index_t max) {
  return min + rand() % (max - min + 1);
}

Point SimplePlayer::_choose_random(const GameView &game) {
  Boundary b = game.get_settings().field_size;
  Point result;
  do {
    result = {
        .x = rand_int(b.min.x, b.max.x),
        .y = rand_int(b.min.y, b.max.y),
    };
  } while (game.get_state().field->get_value(result) != Mark::None);
  return result;
}

void SimplePlayer::assign_mark(Mark player_mark) { _my_mark = player_mark; }

void SimplePlayer::notify(const GameView &view, const Event &event) {
  if (event.get_type() == MoveEvent::TYPE) {
    auto &data = get_data<MoveEvent>(event);
    if (data.mark != _my_mark) {
      _last_move = data.point;
    }
    return;
  }
}
