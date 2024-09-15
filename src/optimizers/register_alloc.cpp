#include "optimizers/register_alloc.hpp"

#include "instructions.h"
#include "interpreter.h"

namespace sci {

int static inline get_register(int &reg) {
  reg = (reg + 1 < 32) ? reg++ : 1;
  return reg;
}

std::vector<Instruction> do_register_alloc(
    std::vector<Instruction> instructions) {
  int creg = 0;
  for (auto &i : instructions) {
    switch (i.op) {
      case InsCode::load:
      case InsCode::loadi: {
        auto reg = std::get<int>(i.i1);
        if (reg >= reg_size) {
          i.i1 = get_register(creg);
        }
        break;
      }
      case InsCode::rmov: {
        auto r0 = std::get<int>(i.i0);
        auto r1 = std::get<int>(i.i1);

        if (r0 >= reg_size) {
          i.i0 = get_register(creg);
        }
        if (r1 >= reg_size) {
          i.i1 = get_register(creg);
        }
        break;
      }
      case InsCode::store:
      case InsCode::cjmp: {
        auto reg = std::get<int>(i.i0);
        if (reg >= reg_size) {
          i.i0 = get_register(creg);
        }
        break;
      }
      case InsCode::add:
      case InsCode::sub:
      case InsCode::mul:
      case InsCode::div:
      case InsCode::gt:
      case InsCode::lt:
      case InsCode::gteq:
      case InsCode::lteq: {
        auto r0 = std::get<int>(i.i0);
        auto r1 = std::get<int>(i.i1);
        auto r2 = std::get<int>(i.i2);

        if (r0 >= reg_size) {
          i.i0 = get_register(creg);
        }
        if (r1 >= reg_size) {
          i.i1 = get_register(creg);
        }
        if (r2 >= reg_size) {
          i.i2 = get_register(creg);
        }
        break;
      }
      default:
        break;
    }
  }
  return instructions;
}

}  // namespace sci
