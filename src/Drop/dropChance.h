#include <vector>
struct EventChance {
  int fight;
  int drop;
  int encounter;
  int noting;
};

class ChanceManager {
public:
  const std::vector<EventChance> firstArea{
      {70, 15, 5, 10}, {60, 20, 10, 10}, {65, 10, 5, 20}, {80, 10, 4, 6},
      {75, 12, 5, 8},  {68, 14, 8, 10},  {85, 5, 4, 6},   {72, 10, 8, 10},
      {90, 3, 2, 5},   {66, 12, 10, 12}};
};
