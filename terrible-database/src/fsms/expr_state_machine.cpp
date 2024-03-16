#include "fsms/expr_state_machine.h"
#include "tokenizer.h"
#include <cmath>
#include <math.h>

namespace tdb {
bool ExprStateMachine::CheckTransition(Token token, std::string word) {
  if (current_state == right_paren)
    return false;

  switch (token) {
  case LEFT_PAREN:
    return check_left_paren_state();
  case RIGHT_PAREN:
    return check_right_paren_state();
  case TEXT:
    if (current_state == left_paren) {
      return check_col_name_state(word);
    } else {
      return check_col_val_state(word);
    }
  case OP_EQ:
  case OP_NEQ:
  case OP_LT:
  case OP_GT:
  case OP_EQLT:
  case OP_EQGT:
    if (check_op_state(token)) {
      str_op = word;
      return true;
    }
  default:
    current_state = undefined;
    return false;
  }
}

std::string ExprStateMachine::GetErrorMsg() {
  if (current_state == error || current_state == undefined) {
    err_msg = "Failed to parse expression.";
    for (auto val : expected_next_state) {
      switch (val) {
      case col_name:
        err_msg += " Expected column name.";
        break;
      case op:
        err_msg += " Expected a binary operator.";
        break;
      case col_value:
        err_msg += " Expected column value.";
        break;
      case right_paren:
        err_msg += " Expected right parenthesis.";
        break;
      case left_paren:
        err_msg += " Expected left parenthesis.";
        break;
      default:
        break;
      }
    }
  }
  return err_msg;
}

bool ExprStateMachine::check_col_name_state(std::string word) {
  if (expected_next_state.size() == 1 &&
      expected_next_state.contains(col_name)) {
    current_state = col_name;
    expected_next_state.clear();
    expected_next_state.insert(op);
    column_name = word;
    return true;
  }
  current_state = error;
  return false;
}

bool ExprStateMachine::check_op_state(Token token) {
  if (expected_next_state.size() == 1 && expected_next_state.contains(op)) {
    current_state = op;
    expected_next_state.clear();
    expected_next_state.insert(col_value);
    return true;
  }
  current_state = error;
  return false;
}

bool ExprStateMachine::check_col_val_state(std::string word) {
  if (expected_next_state.size() == 1 &&
      expected_next_state.contains(col_value)) {
    current_state = col_value;
    expected_next_state.clear();
    expected_next_state.insert(right_paren);
    column_val = word;
    return true;
  }
  current_state = error;
  return false;
}

bool ExprStateMachine::check_left_paren_state() {
  if (expected_next_state.contains(left_paren)) {
    current_state = left_paren;
    expected_next_state.clear();
    expected_next_state.insert(col_name);
    return true;
  }
  current_state = error;
  return false;
}

bool ExprStateMachine::check_right_paren_state() {
  if (expected_next_state.contains(right_paren)) {
    current_state = right_paren;
    expected_next_state.clear();
    return true;
  }
  current_state = error;
  return false;
}

} // namespace tdb
