#include "players/human-player.h"
#include "set"

std::pair<int, int> human_player_t::make_move(const field_t &my_field, const field_t &enemy_field){

  this->output_stream << "Your field:" << std::endl << my_field << std::endl << "Enemy's field" << std::endl;
  this->output_stream << enemy_field << std::endl << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;
  while(true)
  {
    std::string s;
    int x = 0, y = 0;
    auto cur_coords = field_t::FIELD_SIZE;


    std::getline(this->input_stream, s);

    std::set <std::string> cur;
    std::string dict = "ABCDEFGHIJ";

    for (int i = 1; i <= field_t::FIELD_SIZE; i++){
      std::string coord_1 = std::to_string(i);
      x = i - 1;
      for (int j = 0; j < field_t::FIELD_SIZE; j++){
        std::string coord_2 = &dict[j];
        cur.insert(coord_1 + coord_2);
        y = j;
      }
    }

    if (cur.find(s) != cur.end())
      return {x, y};



/*
    if (s.size() == 3 || s.size() == 2) {
        if (s[0] > '0' && s[0] <= '9')
          x = s[0] - '0';
        if (s[s.size() - 1] >= 'A' && s[s.size() - 1] <= 'J')
          y = s[s.size() - 1] - 'A' + 1;
        if (s.size() == 3 && s[1] == '0' && s[0] == 1) {
          x = (int)cur_coords;
        } else {
          x = 0;
        }
    }

  if (x > 0 && x < cur_coords + 1 && y > 0 && y < cur_coords + 1)
      return {x - 1, y - 1};
*/

  this->output_stream << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;

  }
}

void human_player_t::on_duplicate_move(int x, int y)
{
  char second_coord = 'A' + y;
  this->output_stream << x+1 << second_coord << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int x, int y)
{
  char second_coord = 'A' + y;
  this->output_stream << x+1 << second_coord << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int x, int y)
{
  char second_coord = 'A' + y;
  this->output_stream << x+1 << second_coord << " HIT! Your next move!" << std::endl;

}

void human_player_t::on_kill(int x, int y)
{
  char second_coord = 'A' + y;
  this->output_stream << x+1 << second_coord << " KILL! Your next move!" << std::endl;

}

void human_player_t::on_win()
{
  this->output_stream << "You WIN!" << std::endl;

}

void human_player_t::on_lose()
{
  this->output_stream << "You LOSE!" << std::endl;
}
