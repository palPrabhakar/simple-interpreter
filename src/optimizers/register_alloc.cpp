#include "optimizers/register_alloc.hpp"

#include "instructions.h"
#include "interpreter.h"


namespace sci {

int static inline get_register(int &reg) {
  reg = (reg + 1 < 32) ? reg + 1 : 1;
  return reg;
}

void static rename_uses(std::vector<Instruction> &instructions, size_t idx, int before, int after) {
  for(size_t i = idx + 1; i < instructions.size(); ++i) {
    switch(instructions[i].op) {
      case InsCode::rmov:
      case InsCode::store:
      case InsCode::cjmp: {
        if(std::get<int>(instructions[i].i0) == before) {
          instructions[i].i0 = after;
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
        if(std::get<int>(instructions[i].i0) == before) {
          instructions[i].i0 = after;
        }
        if(std::get<int>(instructions[i].i1) == before) {
          instructions[i].i1 = after;
        }
      break;
      }
      default:
      break;
    }
  }
}


std::vector<Instruction> do_register_alloc(
    std::vector<Instruction> instructions) {
  int creg = 0;
  for (size_t i = 0; i < instructions.size(); ++i) {
    switch (instructions[i].op) {
      case InsCode::load:
      case InsCode::loadi:
      case InsCode::rmov: {
        auto reg = std::get<int>(instructions[i].i1);
        if (reg >= reg_size) {
          instructions[i].i1 = get_register(creg);
          rename_uses(instructions, i, reg, std::get<int>(instructions[i].i1));
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
        auto reg = std::get<int>(instructions[i].i2);
        if (reg >= reg_size) {
          instructions[i].i2 = get_register(creg);
          rename_uses(instructions, i, reg, std::get<int>(instructions[i].i2));
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
