#include "optimizers/register_alloc.hpp"

#include "instructions.h"
#include "interpreter.h"


namespace sci {

int static inline get_register(int &reg) {
  reg = (reg + 1 < 32) ? reg + 1 : 1;
  return reg;
}

void static rename_uses(std::vector<Instruction> &instructions, size_t idx, int before, int after) {
  for(int i = idx + 1; i < instructions.size(); ++i) {
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
      case InsCode::loadi: {
        auto reg = std::get<int>(instructions[i].i1);
        if (reg >= reg_size) {
          // TODO:
          // Find use sites and rename them also
          instructions[i].i1 = get_register(creg);
          rename_uses(instructions, i, reg, std::get<int>(instructions[i].i1));
        }
        break;
      }
      case InsCode::rmov: {
        auto r0 = std::get<int>(instructions[i].i0);
        auto r1 = std::get<int>(instructions[i].i1);

        // // Source register (sreg)
        // // sreg can't be greater than 32
        // // sreg either less than 32 originally 
        // // or got renamed since it's a use site
        // if (r0 >= reg_size) {
        //   instructions[i].i0 = get_register(creg);
        // }
        if (r1 >= reg_size) {
          instructions[i].i1 = get_register(creg);
          rename_uses(instructions, i, r1, std::get<int>(instructions[i].i1));
        }
        break;
      }
      case InsCode::store:
      case InsCode::cjmp: {
        // Source register (sreg)
        // sreg can't be greater than 32
        // sreg either less than 32 originally 
        // or got renamed since it's a use site
        // auto reg = std::get<int>(instructions[i].i0);
        // if (reg >= reg_size) {
        //   instructions[i].i0 = get_register(creg);
        // }
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
        auto r0 = std::get<int>(instructions[i].i0);
        auto r1 = std::get<int>(instructions[i].i1);
        auto r2 = std::get<int>(instructions[i].i2);

        // // r0 and r1 source register
        // // again don't need to even do reg alloc
        // if (r0 >= reg_size) {
        //   instructions[i].i0 = get_register(creg);
        // }
        // if (r1 >= reg_size) {
        //   instructions[i].i1 = get_register(creg);
        // }
        if (r2 >= reg_size) {
          // TODO:
          // Find use site and rename also
          instructions[i].i2 = get_register(creg);
          rename_uses(instructions, i, r2, std::get<int>(instructions[i].i2));
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
