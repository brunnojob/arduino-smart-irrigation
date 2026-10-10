#include "irrigation.hpp"
#include <cassert>
#include <iostream>
int main() {
  IrrigationController controller({2600,1900,12000,100,1});
  assert(controller.sample(3000,true,true,100).pump);
  auto stopped=controller.tick(5101);
  assert(!stopped.pump && stopped.state==IrrigationState::Fault);
  auto reset=controller.sample(2000,true,true,5102);
  assert(!reset.pump && reset.state==IrrigationState::Cooldown);
  controller.sample(2000,true,true,5103);
std::cout << "missed sensor deadline stops pump; controlled recovery enters cooldown\n";
}
