#include "game/sea-battle.h"


sea_battle_t::sea_battle_t(std::shared_ptr<player_interface_t> player1, const field_t &field1, std::shared_ptr<player_interface_t> player2, const field_t &field2)
{
  for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
  {
    this->fields[FIRST_PLAYER].push_back(field1[i]);
  }
  for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
  {
    this->fields[SECOND_PLAYER].push_back(field2[i]);
  }
  this->players[FIRST_PLAYER] = player1;
  this->players[SECOND_PLAYER] = player2;

}

sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}

void sea_battle_t::play()
{

  turn_t player = FIRST_PLAYER;
  std::pair<int, int> coords;
  std::array <int, 2> player_ships = {10, 10};

  std::array<field_t, 2> field = {field_t(this->fields[FIRST_PLAYER]), field_t(this->fields[SECOND_PLAYER])};
  field_t field_tmp(this->fields[FIRST_PLAYER]);

  while(true)
  {
    player_ships[!player] = 0;
    for (size_t i = 0; i < field_t::FIELD_SIZE; i++)
      for (size_t j = 0; j < field_t::FIELD_SIZE; j++) {
        if (field[!player][i][j] == field_t::SHIP_CELL) {
          player_ships[!player]++;
          field_tmp[i][j] = field_t::EMPTY_CELL;
        } else {
          field_tmp[i][j] = field[!player][i][j];
        }
      }
    if (player_ships[!player] == 0) {
      this->players[player]->on_win();
      this->players[!player]->on_lose();
      return;
    }
    coords = this->players[player]->make_move(field[player], field_tmp);
    if (coords.first < 0 || coords.first >= field_t::FIELD_SIZE || coords.second < 0 ||
        coords.second >= field_t::FIELD_SIZE) {
      this->players[player]->on_incorrect_move(coords.first, coords.second);
      continue;
    }
    if (field[!player][coords.first][coords.second] == field_t::HIT_CELL ||
        field[!player][coords.first][coords.second] == field_t::MISS_CELL) {
      this->players[player]->on_duplicate_move(coords.first, coords.second);
      continue;
    }
    if (field[!player][coords.first][coords.second] == field_t::EMPTY_CELL) {
      this->players[player]->on_miss(coords.first, coords.second);
      field[!player][coords.first][coords.second] = field_t::MISS_CELL;
      player = change_turn(player);
      continue;
    }
    if (field[!player][coords.first][coords.second] == field_t::SHIP_CELL) {

      field[!player][coords.first][coords.second] = field_t::HIT_CELL;
      int flag = coords.first;
      int neighbour = 0;
      flag--;
      while (flag >= 0 && (field[!player][flag][coords.second] == field_t::SHIP_CELL ||
                           field[!player][flag][coords.second] == field_t::HIT_CELL)) {
        if (field[!player][flag][coords.second] == field_t::SHIP_CELL)
          neighbour++;
        flag--;
      }
      flag = coords.first;
      flag++;
      while (flag < field_t::FIELD_SIZE && (field[!player][flag][coords.second] == field_t::SHIP_CELL ||
                                            field[!player][flag][coords.second] == field_t::HIT_CELL)) {
        if (field[!player][flag][coords.second] == field_t::SHIP_CELL)
          neighbour++;
        flag++;
      }
      flag = coords.second;
      flag--;
      while (flag >= 0 && (field[!player][coords.first][flag] == field_t::SHIP_CELL ||
                           field[!player][coords.first][flag] == field_t::HIT_CELL)) {

        if (field[!player][coords.first][flag] == field_t::SHIP_CELL)
          neighbour++;
        flag--;
      }
      flag = coords.second;
      flag++;
      while (flag < field_t::FIELD_SIZE && (field[!player][coords.first][flag] == field_t::SHIP_CELL ||
                                            field[!player][coords.first][flag] == field_t::HIT_CELL)) {

        if (field[!player][coords.first][flag] == field_t::SHIP_CELL)
          neighbour++;
        flag++;
      }

      if (neighbour == 0) {
        this->players[player]->on_kill(coords.first, coords.second);
      } else {
        this->players[player]->on_hit(coords.first, coords.second);
      }
    }
  }
}
