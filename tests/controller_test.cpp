#include "irrigation.hpp"
#include <cassert>
int main() {
  IrrigationController c({2600, 1900, 3000, 5000, 3});
  assert(!c.sample(3000, true, true, 1000).pump);
  c.sample(3000, true, false, 2000);
  c.sample(3000, true, false, 3000);
  c.sample(3000, true, false, 4000);
  assert(c.sample(3000, true, false, 5000).pump);
  assert(!c.sample(3000, false, false, 6000).pump);
  assert(c.snapshot().state == IrrigationState::Fault);
  c.sample(3000, true, true, 7000);
  assert(c.snapshot().state == IrrigationState::Cooldown);
  c.sample(3000, true, false, 8000);
  c.sample(3000, true, false, 9000);
  c.sample(3000, true, false, 10000);
  c.sample(3000, true, false, 11000);
  assert(c.sample(3000, true, false, 12000).pump);
  c.sample(3000, true, false, 13000);
  c.sample(3000, true, false, 14000);
  assert(!c.sample(3000, true, false, 15000).pump);
  assert(c.snapshot().state == IrrigationState::Cooldown);
}
