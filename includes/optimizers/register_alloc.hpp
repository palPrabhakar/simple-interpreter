#pragma once

#include <vector>
#include "instructions.h"

namespace sci {
std::vector<Instruction> do_register_alloc(std::vector<Instruction> instructions);
}

