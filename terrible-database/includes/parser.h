#ifndef PARSER_H
#define PARSER_H

#include <cmath>
#include <math.h>
#include <unordered_set>
#include <vector>
#include <string>
#include "tokenizer.h"
#include <iostream>

namespace tdb {
    std::vector<std::string> ParseInputQuery(std::string input_query);

    enum Data_Type {
      DT_INT,
      DT_DOUBLE,
      DT_STRING
    };

    class CreateStateMachine {
        public:
            CreateStateMachine() {
                current_state = begin;
                expected_next_state.insert(create);
            }

            // end of parsing
            bool EOP() {
                return current_state == end;
            }

            bool check_transition(Token token, std::string word) {
                switch(token) {
                    case CREATE:
                        return check_create_state();
                    case TEXT:
                        if(current_state == create) {
                            return check_tbl_name_state(word);
                        } else {
                            return check_col_name_state(word);
                        }
                    case WITH:
                        return check_with_state();
                    case TYPE_INT:
                        return check_col_type_state(DT_INT);
                    case TYPE_DOUBLE:
                        return check_col_type_state(DT_DOUBLE);
                    case TYPE_STRING:
                        return check_col_type_state(DT_STRING);
                    case END:
                        return check_end_state();
                    default:
                        return false;
                }
            }

        std::string table_name;
        std::vector<std::string> col_names;
        std::vector<Data_Type> col_types;

        private:
        enum State {
            begin,
            create,
            tbl_name,
            with,
            col_name,
            col_type,
            end,
            error
        };

        enum State current_state;
        std::unordered_set<State> expected_next_state;

        bool check_create_state() {
           if(expected_next_state.size() == 1 && expected_next_state.contains(create)) {
               expected_next_state.clear();
               expected_next_state.insert(tbl_name);
               current_state = create;
               return true;
           }

           return false;
        }

        bool check_tbl_name_state(std::string word) {
            if(expected_next_state.size() == 1 && expected_next_state.contains(tbl_name)) {
               expected_next_state.clear();
               expected_next_state.insert(with);
               current_state = tbl_name;
               table_name = word;
               return true;
            }
            return false;
        }

        bool check_col_name_state(std::string word) {
            if((expected_next_state.size() == 1 || expected_next_state.size() == 2) && expected_next_state.contains(col_name)) {
               expected_next_state.clear();
               expected_next_state.insert(col_type);
               current_state = col_name;
               col_names.push_back(word);
               return true;
            }
            return false;
        }

        bool check_col_type_state(Data_Type type) {
            if(expected_next_state.size() == 1 && expected_next_state.contains(col_type)) {
               expected_next_state.clear();
               expected_next_state.insert(col_name);
               expected_next_state.insert(end);
               current_state = col_type;
               col_types.push_back(type);
               return true;
            }
            return false;
        }

        bool check_with_state() {
            if(expected_next_state.size() == 1 && expected_next_state.contains(with)) {
               expected_next_state.clear();
               expected_next_state.insert(col_name);
               current_state = with;
               return true;
            }
            return false;
        }

        bool check_end_state() {
            if(current_state == col_type && expected_next_state.contains(end)) {
                current_state = end;
                return true;
            }

            return false;
        }

    };
}

#endif
