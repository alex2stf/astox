/*
 * compiler.cpp
 *
 *  Created on: 21 Apr 2017
 *      Author: alex
 */

#include "astox.h"

namespace astox {

	namespace scriptengine {
		using namespace stringAlg;

		operation_node::operation_node(){
			next = NULL;
			value = NULL;
			previous = NULL;
			operation = NO_OPERATION;
		}

		bool operation_node::has_next(){
			return next != NULL;
		};

		bool operation_node::has_previous(){
			return previous != NULL;
		};

		operation_node::~operation_node(){
		}

		operation_node * operation_node::get_root(){
			if(previous != NULL){
				return previous->get_root();
			}
			return this;
		};

		void trim_separator(string &in, const lexic &l){
			if(in.empty()){
				return;
			}
 		    trim(in);
			char u = in.at(0);
			while(u == l.delimiter){
				in = in.substr(1, in.size());
				u = in.at(0);
			}
			u = *in.rbegin();
		    while(u == l.delimiter){
			  in = in.substr(0, in.size()-1);
			  u = *in.rbegin();
		    }
		    trim(in);
		};

		bool string_is_numeric(string in, const lexic &l){
			if(in.empty()){ return false; }
			for(int i = 0; i < in.size(); i++){
				char c = in.at(i);
				bool iv = (c >= '0' && c <= '9') || c == l.dot || c == l.plus || c == l.minus;
				if(!iv){
					return false;
				}
			}
			return true;
		};

		Value * eval(string input, const lexic &lexic, Function * &context){
			int stat = 0;
			return eval_command(input, lexic, context, stat);
		};

		/**
		 * use execute_command for each separated line
		 * @param in
		 * @param l
		 * @param ctx
		 * @return
		 */
		Value * eval_command(string in, const lexic &l, Function * &ctx, int &cmdstat) {

			log_stack("%s", in.c_str());
			vector<string> vs = split_by_separator(in, l);

			if(vs.size() == 1){
				string n = trim(vs.at(0));
				if(n == l.insbreak){
					log_stack("BREAK_CTX");
					cmdstat = BREAK_CTX;
					return NULL;
				}
				if(n == l.inscontinue){
					log_stack("CONTINUE_CTX");
					cmdstat = CONTINUE_CTX;
					return NULL;
				}

				if(n == l.insreturn){
					log_stack("RETURN_VAL");
					cmdstat = RETURN_VAL;
					return NULL;
				}
				log_stack("single command [%s]", n.c_str());
				return execute_command(vs.at(0), l, ctx, cmdstat);
			}

			Array * r = Array::New(l); //TODO modify
			for(int i = 0; i < vs.size(); i++){
				Value * v = (execute_command(vs.at(i), l, ctx, cmdstat));

				if(cmdstat == RETURN_VAL){
					if(r){
						delete r;
					}


					log_stack("return v = %s", blurt(v).c_str());
					return v;
				}
				else if(cmdstat == BREAK_CTX){
					log_stack("break context");
					break;
				}
				else if(cmdstat == CONTINUE_CTX){
					log_stack("continue context");
					break;
				}
				else {
					r->push(v);
				}
			}
			return r;
		};

		static inline bool char_is_slct(char c, const lexic &l){
			return c == l.q1 ||
					c == l.q2 ||
					c == l.l_parenthesis ||
					c == l.r_parenthesis ||
					c == l.l_bracket ||
					c == l.r_bracket ||
					c == l.l_sqbracket ||
					c == l.r_sqbracket;

		}

		vector<string> split_by_keyword(string in, const char * key,
				const lexic &l){
			char_stat cst;
			vector<string> vs;
			string bf, tmp;
//			log_stack("in === %s", in.c_str());
			for(signed int i = 0; i < in.size(); i++){
				char_stat_read(cst, l, in, i);
				char c = in.at(i);
				if(char_stat_valid(cst) && !char_is_slct(c, l)){
					bf += c;
//					log_stack("valid [%c] bf=[%s]", c, bf.c_str());
					if(bf == key){
						if(!tmp.empty()){
//							log_stack("                  push %s", tmp.c_str());
							vs.push_back(tmp);
							tmp.clear();
						}
						bf.clear();
					}
					else if(bf.size() >= strlen(key)){
						tmp+=bf.at(0);
						bf.erase(0, 1);
//						log_stack("  fill tmp = %s", tmp.c_str());
					}
				} else {
					if(!bf.empty()){
						tmp.append(bf);
						bf.clear();
					}
//					log_stack("in_valid [%c] tmp=[%s]", c, tmp.c_str());
					tmp+=c;
				}
			}

			if(!bf.empty()){
				tmp.append(bf);
			}
			if(!tmp.empty()){
				vs.push_back(tmp);
//				log_stack("--end -----%s", tmp.c_str());
			}
			return vs;
		};

		void extract_case_statements(string in, const lexic &l,
				 std::vector<string> &lefts, std::vector<string> &rights){
			vector<string> tmp;
			char_stat cst;
			string cb, buf;

			tmp = split_by_keyword(in, l.incase, l);
			for(int i = 0; i < tmp.size(); i++){
				vector<string> pair = pair_by_colon(tmp.at(i), l);
				if(pair.empty()){
					return;
				}
				trim(pair.at(0));
				trim(pair.at(1));
				log_stack("for case: %s", pair.at(0).c_str());
				lefts.push_back(pair.at(0));

				vector<string> fdef = split_by_keyword(pair.at(1), l.indefault, l);

				if(fdef.size() > 2) {
					throw_error("cannot have more than 1 default statement at [%s]", in.c_str());
					return;
				}

				if(fdef.size() == 2){
					log_stack("    exec: %s", fdef.at(0).c_str());
					trim(fdef.at(0));
					rights.push_back(fdef.at(0));
					string def = fdef.at(1);
					trim(def);
					if(def.at(0) == l.colon){
						def = def.substr(1, def.size());
						trim(def);
					}
					if(def.empty()){
						throw_error("default statement cannot be empty");
						return;
					}
					lefts.push_back(l.indefault);
					rights.push_back(def);
				}

				else if(fdef.size() == 1){
					log_stack("  fdef.size() == 1  exec: %s", pair.at(1).c_str());
					rights.push_back(pair.at(1));
				}

				else {
					rights.push_back(l.incase);
				}

			}

		};

		void detect_instruction_type(string in, command_node * cmd, const lexic &l) {
			  cmd->stmts.clear();
			  cmd->blcks.clear();
			  if(cmd->next){
				  delete cmd->next;
				  cmd->next = NULL;
			  }

			  in = astox::replaceWhiteSpace(in);
			  in = astox::trim(in);
			  int lpoz = 0;

			  //return
			  if(string_starts_with(in, l.insreturn, l)){
				  cmd->type = RETURN_VAL;
				  in = in.substr(strlen(l.insreturn), in.size());
				  in = trim(in);
				  cmd->stmts.push_back(in);
				  return;
			  }

			  // break
			  if(in == l.insbreak || string_starts_with(in, l.insbreak, l.delimiter, l)){
				  cmd->type = BREAK_CTX;
				  cmd->stmts.push_back(in);
				  return;
			  }

			  //continue
			  if(in == l.inscontinue || string_starts_with(in, l.inscontinue, l.delimiter, l)){
				  cmd->type = CONTINUE_CTX;
				  cmd->stmts.push_back(in);
				  return;
			  }

			  //try {
			  if(string_starts_with(in, l.intry, l.l_bracket, l)){

				  trim_separator(in, l);
				  if(in.size() < (strlen(l.intry) + strlen(l.incatch) + 4)){
					 goto exit_default;
				  };

				  cmd->type = TRY_CATCH;
				  in = in.substr(strlen(l.intry), in.size());
				  //tryblock
				  cmd->stmts.push_back(get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l));
				  in = in.substr(lpoz, in.size());
				  trim(in);

				  bool c = false;
				  if(string_starts_with(in, l.incatch, l.l_parenthesis, l)){
					  //exception
					  cmd->blcks.push_back(get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l));
					  in = in.substr(lpoz, in.size());
					  if(in.empty()){
						  throw_error("invalid catch statement at [%s]", in.c_str());
						  return;
					  }
					  //catch block
					  cmd->stmts.push_back(get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l));
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  c = true;
				  }

				  if(string_starts_with(in, l.infinally, l.l_bracket, l)){
					  //finally block
					  cmd->stmts.push_back(get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l));
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  c = true;
				  }

				  if(!c){
					  throw_error("invalid [try{T}catch(E){C}] syntax");
				  }
				  //next command
				  if(!in.empty()){
					  cmd->next = new command_node;
					  detect_instruction_type(in, cmd->next, l);
					  log_stack("%s", blurt(cmd).c_str());
				  }

				  return;
			  }

			  // var
			  if(string_starts_with(in, l.var, l.wspace, l)){
				  //exit default for input like if(){}
				  trim_separator(in, l);
				  if(in.size() < strlen(l.var) + 1){
					 goto exit_default;
				  };

				  cmd->type = VAR_ASSIGN;
				  in = in.substr(strlen(l.var), in.size());
				  trim(in);
				  cmd->stmts.push_back(in);

				  return;
			  }
			  //function <name>( or function(){}
			  if(string_is_func_format(in, l)){

				  trim_separator(in, l);
				  if(in.size() < strlen(l.func) + 4){
					 goto exit_default;
				  };
				  cmd->type = FUNCTION_DEF;
				  get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
				  cmd->stmts.push_back(in.substr(0, lpoz));

				  if(lpoz < in.size() - 1){
					  in = in.substr(lpoz, in.size());
					  trim_separator(in, l);
					  if(!in.empty()){

						  log_stack("next = %s", in.c_str());
						  cmd->next = new command_node;
						  detect_instruction_type(in, cmd->next, l);
					  }
				  }
				  return;
			  }

			  //delete
			  if(string_starts_with(in, l.insdelete, l.wspace, l)){
				  in = in.substr(strlen(l.insdelete), in.size());
				  trim(in);
				  cmd->type = DELETE_CTX;
				  cmd->stmts.push_back(in);
				  return;
			  }

			  //throw
			  if(string_starts_with(in, l.insthrow, l.wspace, l)){
				  in = in.substr(strlen(l.insthrow), in.size());
				  trim(in);
				  cmd->stmts.push_back(in);
				  cmd->type = THROW_CTX;
				  cmd->stmts.push_back(in);
				  return;
			  }

			  //if (..) else if
			  if(string_starts_with(in, l.inif, l.l_parenthesis, l)){
				  //exit default for input like if(){}
				  if(in.size() < strlen(l.inif) + 4){
					 goto exit_default;
				  };

				  cmd->type = IF_ELSE;
				  in = in.substr(strlen(l.inif), in.size());
				  string stmt = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l);
				  cmd->stmts.push_back(stmt);
				  in = in.substr(lpoz, in.size());
				  string blk =  get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
				  cmd->blcks.push_back(blk);
				  in = in.substr(lpoz, in.size());
				  trim(in);

				  while(string_starts_with(in, l.inelse, l)){
					  in = in.substr(strlen(l.inelse), in.size());
					  trim(in);
					  if(string_starts_with(in, l.inif, l.l_parenthesis, l)){
						  stmt = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l);
						  cmd->stmts.push_back(stmt);
						  in = in.substr(lpoz, in.size());
					  }
					  blk =  get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
					  cmd->blcks.push_back(blk);
					  in = in.substr(lpoz, in.size());
				  }

				  trim(in);
				  if(string_starts_with(in, l.inelse, l.l_bracket, l)){
					  in = in.substr(strlen(l.inelse), in.size());
					  blk =  get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
					  cmd->blcks.push_back(blk);
					  in = in.substr(lpoz, in.size());
				  }

				  trim(in);
				  if(!in.empty()){
					  cmd->next = new command_node;
					  detect_instruction_type(in, cmd->next, l);
				  }
				  return;
			  }



			  //do {
			  if(string_starts_with(in, l.indo, l.l_bracket, l)){
				  //exit default for input like if(){}
				  if(in.size() < strlen(l.indo) + 4){
					 goto exit_default;
				  };
				  cmd->type = DO_WHILE;
				  in = in.substr(strlen(l.indo), in.size());
				  log_stack("in = %s", in.c_str());
				  trim(in);
				  log_stack("in = %s", in.c_str());
				  cmd->blcks.push_back(get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l));
				  in = in.substr(lpoz, in.size());
				  trim(in);
				  log_stack("in = %s", in.c_str());
				  if(string_starts_with(in, l.inwhile, l.l_parenthesis, l)){
					  in = in.substr(strlen(l.inwhile), in.size());
					  trim(in);
					  cmd->stmts.push_back(get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l));
				  } else {
					  throw_error("malformed do {D} while {W} statement at [%s]", in.c_str());
				  }
				  if(lpoz < in.size() - 1){
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  if(!in.empty()){
						  cmd->next = new command_node;
						  detect_instruction_type(in, cmd->next, l);
					  }
				  }
				  return;
			  }

			  //while (
			  if(string_starts_with(in, l.inwhile, l.l_parenthesis, l)){
				  if(in.size() < strlen(l.inwhile) + 4){
					 goto exit_default;
				  };
				  cmd->type = WHILE_LOOP;
				  in = in.substr(strlen(l.inwhile), in.size());
				  trim(in);
				  cmd->stmts.push_back(get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l));
				  in = in.substr(lpoz, in.size());
				  trim(in);
				  if(string_starts_with(in, l.indo, l.l_bracket, l)){
					  in = in.substr(strlen(l.indo), in.size());
				  }
				  cmd->blcks.push_back(get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l));


				  if(lpoz < in.size() - 1){
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  if(!in.empty()){
						  cmd->next = new command_node;
						  detect_instruction_type(in, cmd->next, l);
					  }
				  }
				  return;
			  }

			  //for
			  if(string_starts_with(in, l.infor, l.l_parenthesis, l)){
				  cmd->type = FOR_LOOP;
				  trim_separator(in, l);
				  if(in.size() < strlen(l.infor) + 4){
					 goto exit_default;
				  }

				  in = in.substr(strlen(l.infor), in.size());
				  trim(in);
				  string cond = get_expression_between(in, l.l_parenthesis, l.r_parenthesis,lpoz, l);
				  in = in.substr(lpoz, in.size());

				  string block = get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);

				  cmd->stmts.push_back(cond);
				  cmd->blcks.push_back(block);

				  if(lpoz < in.size() - 1){
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  if(!in.empty()){
						  cmd->next = new command_node;
						  detect_instruction_type(in, cmd->next, l);
					  }
				  }

				  return;
			  }

			  //switch
			  if(string_starts_with(in, l.inswitch, l.l_parenthesis, l)){
				  trim_separator(in, l);
				  if(in.size() < strlen(l.inswitch) + 4){
					 goto exit_default;
				  }
				  cmd->type = SWITCH_CASE;
				  in = in.substr(strlen(l.inswitch), in.size());
				  log_stack("1. %s", in.c_str());
				  string stmt = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lpoz, l);
				  log_stack("2. swicth statement [%s]", stmt.c_str());
				  cmd->stmts.push_back(stmt);
				  cmd->blcks.push_back(l.inswitch);
				  in = in.substr(lpoz, in.size());
				  trim(in);
				  log_stack("3. %s", in.c_str());

				  string blk = get_expression_between(in, l.l_bracket, l.r_bracket, lpoz, l);
				  log_stack("4. block statement [%s]", blk.c_str());
				  trim(blk);
				  extract_case_statements(blk, l, cmd->stmts, cmd->blcks);

				  if(lpoz < in.size() - 1){
					  in = in.substr(lpoz, in.size());
					  trim(in);
					  if(!in.empty()){
						  cmd->next = new command_node;
						  detect_instruction_type(in, cmd->next, l);
					  }
				  }
				  return;
			  }

			  exit_default:
				  cmd->type = OPERATION;
				  cmd->stmts.push_back(in);
		};


		int first_operation_index(string in, const lexic &l){
			if(!in.empty() && in.size() > 1){
				//++
				if(in.at(0) == l.plus && in.at(1) == l.plus  && in.size() > 2){ return 2; }
				//--
				if(in.at(0) == l.minus && in.at(1) == l.minus  && in.size() > 2){ return 2; }
				//+
				if(in.at(0) == l.plus ){ return 1; }
				//-
				if(in.at(0) == l.minus ){ return 1; }
				//!a
				if(in.size() > 1 && in.at(0) == l.negation &&
						in.at(1) != l.plus  && in.at(1) != l.minus){ return 1; }
				//~a
				if(in.size() > 1 && in.at(0) == l.bitnot &&
						in.at(1) != l.plus  && in.at(1) != l.minus){ return 1; }

				//!++
				if(in.size() > 2 && in.at(0) == l.negation &&
						in.at(1) == l.plus  && in.at(2) == l.plus){ return 3; }
				//~++
				if(in.size() > 2 && in.at(0) == l.bitnot &&
						in.at(1) == l.plus  && in.at(2) == l.plus){ return 3; }
				//!--
				if(in.size() > 2 && in.at(0) == l.negation &&
						in.at(1) == l.minus  && in.at(2) == l.minus){ return 3; }
				//~--
				if(in.size() > 2 && in.at(0) == l.bitnot &&
						in.at(1) == l.minus  && in.at(2) == l.minus){ return 3; }
				//!+
				if(in.size() > 2 && in.at(0) == l.negation &&
						in.at(1) == l.plus  && in.at(2) != l.plus){ return 2; }
				//~+
				if(in.size() > 2 && in.at(0) == l.bitnot &&
						in.at(1) == l.plus  && in.at(2) != l.plus){ return 2; }
				//!-a
				if(in.size() > 2 && in.at(0) == l.negation &&
						in.at(1) == l.minus  && in.at(2) != l.minus){ return 2; }
				//~-a
				if(in.size() > 2 && in.at(0) == l.bitnot &&
						in.at(1) == l.minus  && in.at(2) != l.minus){ return 2; }
			}
			return 0;
		}//first_operation_index

		/**
		 * extract_operation_nodes and execute_operation_nodes
		 * @param in
		 * @param lexic
		 * @param context
		 * @return
		 */
		Value * execute_operation(string in, const lexic &lexic, Function * &context){

			log_stack("%s", in.c_str());
			operation_node head;

			Value * response = extract_operation_nodes(in, lexic, &head, context);
			log_stack("head = %s", blurt(&head).c_str());
//			log_stack("response = %s", blurt(response));
			if(response == NULL){ //if no assign || ternary flow was generated, operate
				return execute_operation_nodes(&head, context);
			}
			return response;
		};







		void operate(operation_node * left, operation_node * right, Function * &ctx){


			stack::getInstance()->add(ASTX_CURRENT_FUNCTION, ASTX_LOCATION, "%s(%s) %s(%s)",
										blurt(left->previous).c_str(), blurt(left).c_str(),
										blurt(right->previous).c_str(), blurt(right).c_str());
		//	Value * n;
			switch(left->operation){
				case PLUS:
					left->value = left->value->increment(right->value);
					break;
				case MINUS:
					left->value= left->value->substract(right->value);
					break;
				case MULTIPLY:
					left->value = left->value->multiply(right->value);
					break;
				case DIVIDE:
					left->value = left->value->divide(right->value);
					break;
				case MODULUS:
					left->value = left->value->modulus(right->value);
					break;
				case EQUALS:
					left->value = left->value->equals(right->value);
					break;
				case EQUALS_STRICT:
					left->value = left->value->equalsStrict(right->value);
					break;
				case NOT_EQUALS:
					left->value = left->value->notEquals(right->value);
					break;
				case NOT_EQUALS_STRICT:
					left->value = left->value->notEqualsStrict(right->value);
					break;
				case SHIFT_LEFT:
					left->value = left->value->shiftLeft(right->value);
					break;
				case SHIFT_RIGHT_UNSG:
					left->value = left->value->shiftRightUnsigned(right->value);
					break;
				case SHIFT_RIGHT:
					left->value = left->value->shiftRight(right->value);
					break;
				case BIT_OR:
					left->value = left->value->orBit(right->value);
					break;
				case BIT_AND:
					left->value = left->value->andBit(right->value);
					break;
				case GREATER:
					left->value = left->value->isBiggerThan(right->value);
					break;
				case LESSER:
					left->value = left->value->isLessThan(right->value);
					break;
				case LESSER_EQ:
					left->value = left->value->isLessEqThan(right->value);
					break;
				case GREATER_EQ:
					left->value = left->value->isBiggerEqThan(right->value);
					break;
				case BIT_XOR:
					left->value = left->value->xorBit(right->value);
					break;
				case OR:
					left->value = left->value->orStrict(right->value);
					break;
				case AND:
					left->value = left->value->andStrict(right->value);
					break;
				case INSTANCEOF:
					left->value = left->value->isInstanceOf(right->value);
					break;
				case OP_IN:
					left->value = left->value->existsIn(right->value);
					break;
				default:
					method_not_implemented();
			}
			left->operation = right->operation;
			if(right->has_next()){
				left->next = right->next;
			} else {
				left->next = NULL;
			}

		}//operate

		static inline bool is_tier1(int op){
			return op == MULTIPLY || op == DIVIDE || op == MODULUS;
		}

		static inline bool is_tier2(int op){
			return op == PLUS || op == MINUS;
		}

		static inline bool is_tier3(int op){
			return op == SHIFT_LEFT || op == SHIFT_RIGHT || op == SHIFT_RIGHT_UNSG;
		}

		static inline bool is_conditional(int op){
			return op == GREATER ||
					op == LESSER || op == LESSER_EQ ||
					op == EQUALS || op == EQUALS_STRICT ||
					op== NOT_EQUALS ||
					op == GREATER_EQ;
		}


		static inline void detect_precedence(operation_node *& cur, int op, Function *& ctx){
			if(cur->next == NULL) return;
			while(cur->next != NULL){
				if( (cur->operation) == op ){
					operate(cur, cur->next, ctx);
				} else {
					cur = cur->next;
				}
			}
			cur = cur->get_root();
		}

		/**
		 * 1 a*b a/b a%b
		 * 2 a+b a-b
		 * 3 << >>
		 * 4 conditional
		 * @param head
		 * @param context
		 * @return
		 */
		Value * execute_operation_nodes(operation_node * head, Function * &context){
			//log_stack("%s", blurt(head).c_str());

			if(!head->has_next() || head->operation == NO_OPERATION){
				return head->value;
			}

			operation_node * cur = head;
			detect_precedence(cur, INSTANCEOF, context);
			//tier 1
			while(cur->next != NULL){
				if( is_tier1(cur->operation) ) {
					operate(cur, cur->next, context);
				} else {
					cur = cur->next;
				}
			}
			cur = cur->get_root();
			if(!cur->has_next()){ goto retval; }


			while(cur->next != NULL){
				if( is_tier2(cur->operation) ){
					operate(cur, cur->next, context);
				} else {
					cur = cur->next;
				}
			}
			cur = cur->get_root();
			if(!cur->has_next()){ goto retval; }


			while(cur->next != NULL){
				if( is_tier3(cur->operation) ){
					operate(cur, cur->next, context);
				} else {
					cur = cur->next;
				}
			}
			cur = cur->get_root();
			if(!cur->has_next()){ goto retval; }


			while(cur->next != NULL){
				if( is_conditional(cur->operation) ){
					operate(cur, cur->next, context);
				} else {
					cur = cur->next;
				}
			}
			cur = cur->get_root();
			if(!cur->has_next()){ goto retval; }

			/**
			 * &	Bitwise AND	Left to right	8
				^	Bitwise XOR	Left to right	9
				|	Bitwise OR	Left to right	10
				&&	Logical AND	Left to right	11
				||	Logical OR	Left to right	12
				?:
			 */
			detect_precedence(cur, BIT_AND, context);
			detect_precedence(cur, BIT_XOR, context);



//			printf("root....%s\n", blurt(cur));

			while(cur->next != NULL){
				operate(cur, cur->next, context);
			}

			retval:
				return cur->value;
		}//execute_operation_nodes




		Value * extract_operation_nodes(string in, const lexic &l,
				operation_node * node, Function * &context){
			string buf, rest;
			char_stat cst;
			trim(in);

			RegExp * r = build_regex_from_string(in, l);
			if(r != NULL){
				return r;
			}
			signed int length = (int) in.size();
			int fi = first_operation_index(in, l);


			for(int i = 0; i < fi; i++){
				buf+=in.at(i);
			}


			log_stack("in = %s", in.c_str());
			char n = '\0', n2 = '\0', p = '\0';
			int operation = NO_OPERATION;
			int ofs = 0, i;

			for(i = fi; i < length; i++){

				char_stat_read(cst, l, in, i);
				char c = in.at(i);

				if(!char_stat_valid(cst)){
					buf+=c;
					continue;
				}
				if (i > 0) { p = in.at(i-1); }
				if( i < length - 1) { n = in.at(i + 1); }
				if( i < length - 2) { n2 = in.at(i + 2); }

//				log_stack("n = [%c], opin = %s", n, l.op_in);
				 // assign v=
				if(c == l.equals && n != l.equals && n!= l.bigg && n!=l.less) {
					operation = ASSIGN;
					rest = in.substr(buf.size() + 1, in.size());
					rest = trim(rest);
					log_stack("assign buf [%s] with rest[%s]", buf.c_str(), rest.c_str());
					Value * assign = execute_operation(rest, l, context);
                    context->rget(buf, l, context, VAR_ASSIGN, assign);
					return assign;
				}
				// v |=
				else if(c == l.op_or && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), BIT_OR);
				}
				// v &=
				else if(c == l.op_and && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), BIT_AND);
				}
				// v+=
				else if(c == l.plus && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), PLUS);
				}
				// v-=
				else if(c == l.minus && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), MINUS);
				}
				// v /=
				else if(c == l.divide && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), DIVIDE);
				}
				// v %=
				else if(c == l.modulus && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), MODULUS);
				}
				// v *=
				else if(c == l.multiply && n == l.equals) {
					rest = in.substr(buf.size() + 2, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), MULTIPLY);
				}
				// v <<=
				else if(c == l.less && n == l.less && n2 == l.equals) {
					rest = in.substr(buf.size() + 3, in.size());
					rest = trim(rest);
					printf("\t<<= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), SHIFT_LEFT);
				}
				// v >>=
				else if(c == l.bigg && n == l.bigg && n2 == l.equals) {
					rest = in.substr(buf.size() + 3, in.size());
					rest = trim(rest);
					printf("\t+= buf[%s] rest[%s] %s\n",
						buf.c_str(), rest.c_str(), ASTX_LOCATION);
					return get_member(buf, context, l)
							->modifyBy(execute_operation(rest, l, context), SHIFT_RIGHT);
				}
				// v >>>
				else if( (c == l.bigg && n == l.bigg && n2 != l.equals && n2 == l.bigg) ) {
					//>>>=
					if(i+3 >= in.size()){
						Error n("Invalid operator >>> ", ASTX_LOCATION, ASTX_CURRENT_FUNCTION);
						throw n;
					}
					char n3 = in.at(i+3);
					if(n3 == l.equals){
						rest = in.substr(buf.size() + 4, in.size());
						rest = trim(rest);
						printf("\t+= buf[%s] rest[%s] %s\n",
							buf.c_str(), rest.c_str(), ASTX_LOCATION);
						return get_member(buf, context, l)
								->modifyBy(execute_operation(rest, l, context), SHIFT_RIGHT_UNSG);
					}
					operation = SHIFT_RIGHT_UNSG; ofs = 3;
					break;
				}


				// v ? ternary
				else if(c == l.ternar) {
					rest = in.substr(buf.size() + 1, in.size());
					rest = trim(rest);

					vector<string> pair = pair_by_colon(rest, l);
					node->value = get_member(buf, context, l);
					node->operation = NO_OPERATION;
					node->next = NULL;

					Value * condition = execute_operation_nodes(node->get_root(), context);
					printf("if : cond[%s=%i] then[%s] else[%s]\n",
							condition->str().c_str(),
							condition->bol(),
							pair.at(0).c_str(),
							pair.at(1).c_str()
					);

					if(condition->bol()){
						return  execute_operation(pair.at(0), l, context);
					}
					return execute_operation(pair.at(1), l, context);
				}

				// v==
				else if(c == l.equals && n == l.equals && n2 != l.equals) {
					operation = EQUALS; ofs = 2;
					break;
				}
				// v===
				else if(c == l.equals && n == l.equals && n2 == l.equals) {
					operation = EQUALS_STRICT; ofs = 3;
					break;
				}
				// v+
				else if(c == l.plus && n != l.plus && p != l.plus) {
					operation = PLUS; ofs = 1;
					break;
				}
				// v>
				else if(c == l.bigg && n != l.bigg && n != l.equals) {
					operation = GREATER; ofs = 1;
					break;
				}
				// v<
				else if(c == l.less && n != l.less && n != l.equals) {
					operation = LESSER; ofs = 1;
					break;
				}
				// v<= =<
				else if( (c == l.less && n == l.equals) || ( c == l.equals && n == l.less ) ) {
					operation = LESSER_EQ; ofs = 2;
					break;
				}
				// v>= =>
				else if( (c == l.bigg && n == l.equals) || ( c == l.equals && n == l.bigg ) ) {
					operation = GREATER_EQ; ofs = 2;
					break;
				}
				// v <<
				else if( (c == l.less && n == l.less && n2 != l.equals) ) {
					operation = SHIFT_LEFT; ofs = 2;
					break;
				}

				// v >>
				else if( (c == l.bigg && n == l.bigg && n2 != l.equals && n2 != l.bigg) ) {
					operation = SHIFT_RIGHT; ofs = 2;
					break;
				}
				//v-
				else if(c == l.minus && n != l.minus && p != l.minus && n != l.equals) {
					operation = MINUS; ofs = 1;
					break;
				}
				//v|
				else if(c == l.op_or && n != l.op_or) {
					operation = BIT_OR; ofs = 1;
					break;
				}
				//v||
				else if(c == l.op_or && n == l.op_or) {
					operation = OR; ofs = 2;
					break;
				}
				//v&
				else if(c == l.op_and && n != l.op_and) {
					operation = BIT_AND; ofs = 1;
					break;
				}
				//v&
				else if(c == l.op_and && n == l.op_and) {
					operation = AND; ofs = 2;
					break;
				}
				//v !=
				else if(c == l.negation && n == l.equals && n2 != l.equals) {
					operation = NOT_EQUALS; ofs = 2;
					break;
				}
				//v !==
				else if(c == l.negation && n == l.equals && n2 == l.equals) {
					operation = NOT_EQUALS_STRICT; ofs = 3;
					break;
				}
				//v%
				else if(c == l.modulus&& n != l.equals) {
					operation = MODULUS; ofs = 1;
					break;
				}
				//v*
				else if(c == l.multiply && n != l.equals) {
					operation = MULTIPLY; ofs = 1;
					break;
				}
				// v/
				else if(c == l.divide && n != l.equals) {
					operation = DIVIDE; ofs = 1;
					break;
				}
				// v ^
				else if(c == l.op_xor ) {
					operation = BIT_XOR; ofs = 1;
					break;
				}

				//in
				else if(string_ends_with(buf, l.op_in, l) && l.wscnd(c)){
					buf = buf.substr(0, buf.size() - strlen(l.op_in));
					operation = OP_IN; ofs = 1;
					break;
				}

				//instanceof
				else if(string_ends_with(buf, l.instanceof, l) && l.wscnd(c) ){
					buf = buf.substr(0, buf.size() - strlen(l.instanceof));
					operation = INSTANCEOF; ofs = 1;
					break;
				}

				else {
					buf +=c;
				}
			}//exit for
			//do not trim buf!!!
			int bufsz = (int) buf.size();

			log_stack("buf = %s", buf.c_str());
//			printf("\tbuf[%s][%i] %s [op%i] i=%i\n", buf.c_str(), bufsz, ASTX_LOCATION, operation, i);

			node->value = get_member(buf, context, l);
			node->operation = operation;
			if(i + ofs < length){
				rest = in.substr(i + ofs, in.size());
//				printf("\trest[%s] size[%i] i+ofs=%i %s\n", rest.c_str(), rest.size(), i+ofs, ASTX_LOCATION);
				if(!rest.empty()){
					node->next = new operation_node;
					node->next->previous = node;
					return extract_operation_nodes(rest, l, node->next, context);
				}
			}
			return NULL;
		}//extract_operation_nodes

		bool string_ends_with(string &input, const char * end,  const lexic &l){
//			printf("end [%s] input [%s] sz=%i\n", end, input.c_str(), input.size());
			if(input.empty()){
//				printf("empty input \n");
				return false;
			}
			size_t esz = strlen(end),
			max = input.size();
			if(esz >= max){
				return false;
			}
			size_t i, d;
			for (i = 0; i < esz; i++){
				d = (max - esz + i);
//				printf("%i %c == %i %c\n", i, end[i], d, input.at(d));
				if(input.at(d) != end[i]){
					return false;
				}
			}
			if(i >= max){
				return false;
			}
//			printf("ws %i %c\n", max-i-1, input.at(max-i-1));
			if(!l.wscnd(input.at(max-i-1))){
				return false;
			}

			return true;
		}

		bool string_starts_with(string &input, const char * start, const lexic &l){
			if(input.empty()){
				return false;
			}

			size_t last = strlen(start), i;
			if( last > input.size() - 1){ return false; }
			// check if var!=vas
			for (i = 0; i < last; i++){
				if(input.at(i) != start[i]){
					return false;
				}
			}
			if(l.wscnd(input.at(last))){
				return true;
			}
			return false;
		};//string_starts_with

		bool string_starts_with(string &input, const char * start, char aca1, const lexic &l){
//			printf("%s\n\t%s sw %s aca %c %s\n", ASTX_CURRENT_FUNCTION, input.c_str(), start, aca1, ASTX_LOCATION);
			if(input.empty()){
				return false;
			}

			size_t last = strlen(start), i;
			if( last > input.size() - 1){ return false; }
			// check if var!=vas
			for (i = 0; i < last; i++){
				if(input.at(i) != start[i]){
					return false;
				}
			}
			if(l.wscnd(input.at(last)) || input.at(last) == aca1){
				return true;
			}
			return false;
		}//string_starts_with



		Value * exectx_switch(command_node * head, const lexic &l, Function *&ctx, int &cmdstat){

			if(head->stmts.size() != head->blcks.size()){
				throw_error("syntax exception %d statements and %d blocks",
                            (int)head->stmts.size(), (int)head->blcks.size());
				return NULL;
			}

			cmdstat = head->type;
			log_stack("init, get head0 %s", head->sat(0).c_str());
			Value * switchValue = execute_operation(head->sat(0), l, ctx);
			log_stack(" swicth de (%s)\n", switchValue->str().c_str());

			Value * retval = NULL;
			Value * caseValue = NULL;

			bool exct = false;
			int lstat = -1;
			for(int i = 1; i < head->stmts.size(); i++){

				if(head->sat(i) == l.indefault){
					retval = eval_command(head->bat(i), l, ctx, lstat);
					log_stack(".................. for  default (%s)", blurt(switchValue).c_str());
					exct = true;
				}

				else if(exct){
					log_stack("..... defined as true, exec [%s]", head->bat(i).c_str());
					retval = eval_command(head->bat(i), l, ctx, lstat);
				}
				else {
					log_stack("............... execute_operation(%s)", head->sat(i).c_str());
					caseValue = (execute_operation(head->sat(i), l, ctx));
					log_stack("............... for case(%s)", blurt(caseValue).c_str());

					if(switchValue->equalsStrict(caseValue)->bol()){
						exct = true;
						log_stack(".................found true, exec [%s] ", head->bat(i).c_str());
						retval = eval_command(head->bat(i), l, ctx, lstat);
					} else {
						log_stack(".................skip [%s] ", head->bat(i).c_str());
					}


				}



//

				if(lstat == BREAK_CTX){
					log_stack(".............%i lstat BREAK", lstat);
					break;
				}
				if(lstat == RETURN_VAL){
					log_stack(".............%i lstat RETURN_VAL", lstat);
					return retval;
				}

			}


			//method_not_implemented();
			return retval;
		};//exectx_switch


		Value * exectx_ifelse(command_node * head, const lexic &l, Function *& ctx, int &cmdstat){
			Value * response = NULL;
			cmdstat = head->type;
			int i;
			for(i = 0; i < head->stmts.size(); i++){
				if(execute_operation(head->sat(i), l, ctx)->bol()){
					response = eval_command(head->bat(i), l, ctx, cmdstat);
					log_stack("'%s' evaluated as true, return %s", head->sat(i).c_str(), blurt(response).c_str());
					goto exit;
				}
			}
			//else statement
			if( i <= head->blcks.size() - 1){
				response = execute_command(head->bat(i), l, ctx, cmdstat);
//				log_stack("else statement executed: %s", response->c_str());
			}
			exit:
			    return response;
		};

		Value * exectx_for(command_node * head, const lexic &l,
				Function *& ctx, int &cmdstat){

//			printf("%s\n\t%s,  (%s)\n", ASTX_CURRENT_FUNCTION, head->sat(0).c_str(), ASTX_LOCATION);

			log_stack("FOR LOOOP");
			Value * res = Value::npos;
			vector<string> parts = split_by_separator(head->sat(0), l);
			int localCmdStat = 0;
			if(parts.size() == 3){
				//assignment:
				execute_command(parts.at(0), l, ctx, cmdstat);


				bool allow = true;
				while(execute_operation_command(parts.at(1), l, ctx)->bol()){

					res = eval_command(head->bat(0), l, ctx, localCmdStat);
					if(localCmdStat == BREAK_CTX){
						log_stack("BREAK_CTX in for");
						break;
					}
					if(localCmdStat == RETURN_VAL){
						cmdstat = localCmdStat;
						log_stack("RETURN_VAL in for");
						return res;
					}
					execute_operation_command(parts.at(2), l, ctx);
				}
			}
			else if(parts.size() == 1){
				parts = split_by_keyword(head->sat(0), l.op_in, l);

				if(parts.size() == 2){
					string sn = parts.at(0);
					trim(sn);
					trim(parts.at(1));
					if(string_starts_with(sn, l.var, l)){
						sn = sn.substr(strlen(l.var), sn.size());
						trim(sn);
					}

					log_stack("p0 = %s",sn.c_str());
					log_stack("p1 = %s", parts.at(1).c_str());

					Value * right = get_member(parts.at(1), ctx, l, NULL);

					ctx->setContextKey(sn);

					set<stxtr> names = right->key_names();
					std::set<stxtr>::iterator sit;

					for(sit = names.begin(); sit != names.end(); ++sit){
						log_stack("it = %s", sit->c_str());
						ctx->addKey(sn, String::New(*sit, l));

						log_stack("....................... ctxkey it = %s",ctx->getContextKey().c_str());
						res = eval_command(head->bat(0), l, ctx, localCmdStat);
						if(localCmdStat == BREAK_CTX){
							log_stack("BREAK_CTX in for");
							break;
						}
						if(localCmdStat == RETURN_VAL){
							cmdstat = localCmdStat;
							log_stack("RETURN_VAL in for");
							return res;
						}
					}

					ctx->clearContextKey();
					//log_stack("p1 = %s", blurt(right));
					//method_not_implemented();
				}
				else if(parts.size() == 1){
					method_not_implemented();
				}
				else {
					throw_error("invalid for statement, could not detect type");
				}
			}

			else {
				throw_error("invalid for statement");
			}

			return res;
		};

		Value * exectx_try_catch(command_node * cmd, const lexic &l,
				Function *& ctx, int &cmdstat){
				Value * r = NULL; Value * tmp = NULL;

				cmdstat = cmd->type;

				try {
					r = eval_command(cmd->sat(0), l, ctx, cmdstat);
				}
				catch (Error &e) {

					if(cmd->stmts.size() > 1 && !cmd->sat(1).empty()){
//						Function * lctx = ctx->clone();
						if(!cmd->blcks.empty() && !cmd->bat(0).empty()){
							log_stack("cmd->bat(0) %s", cmd->bat(0).c_str());
							Error * ecp = new Error(e.getMessage(), e.getSource(),
									e.getMethod(), e.getLine());
							tmp = ctx->getKey(cmd->bat(0));
							ctx->addKey(cmd->bat(0), ecp);
						}
						r = eval_command(cmd->sat(1), l, ctx, cmdstat);
						if(tmp != NULL){
							ctx->addKey(cmd->bat(0), tmp);
						}
					}

				}

				if(cmd->stmts.size() == 3 && !cmd->sat(2).empty()){
					r = eval_command(cmd->sat(2), l, ctx, cmdstat);
				}

				return r;
		};

		Value * exectx_do_while(command_node * head, const lexic &l,
				Function *& ctx, int &cmdstat){
				Value * r = NULL;
				cmdstat = head->type;
				int lstat = -1;
				do {
					r = eval_command(head->bat(0), l, ctx, lstat);
					if(lstat == CONTINUE_CTX){ continue; }
					if(lstat == BREAK_CTX){ cmdstat = lstat; break; }
					if(lstat == RETURN_VAL){ cmdstat = lstat; return r; }
				} while(execute_operation(head->sat(0), l, ctx)->bol());

				return r;
		};

		Value * exectx_while(command_node * head, const lexic &l,
				Function *& ctx, int &cmdstat){

			Value * r = NULL;
			cmdstat = head->type;
			int lstat = -1;
			while(execute_operation(head->sat(0), l, ctx)->bol()){
				log_stack("exec while");
				r = eval_command(head->bat(0), l, ctx, lstat);
				if(lstat == CONTINUE_CTX){ continue; }
				if(lstat == BREAK_CTX){ cmdstat = lstat; break; }
				if(lstat == RETURN_VAL){ cmdstat = lstat; return r; }
			}
			return r;

		};

		Value * execute_operation_command(string input, const lexic &l, Function *& ctx){
			vector<string> cvs = split_by_comma(input, l);

			if(cvs.size() == 1){
				return execute_operation(cvs.at(0), l, ctx);
			}
			Array * args = Array::New(l); //TODO modify

			for(int i = 0; i < cvs.size(); i++){
				args->push(execute_operation(cvs.at(i), l, ctx));
				//v = (execute_operation(cvs.at(i), l, ctx));
			}
			return args;
		};
		/**
		 *
		 * @param head
		 * @param l
		 * @param context
		 * @param args
		 * @return
		 */
		Value * execute_command_node(command_node * head, const lexic &l,
				Function *& context, Array *& args, int &cmdstat){
			cmdstat = head->type; //command stat is defined by cmd type

			log_stack("execute_command_node <%s>, %i\n", head->sat(0).c_str(), head->type);

			Value * tmp = NULL;
			function_def fdef;
			switch(head->type) {
				case VAR_ASSIGN:
				case OPERATION:
					tmp = execute_operation_command(head->sat(0), l, context);
					if(tmp->isArray()){
						args = Array::cast(tmp);
					} else {
						args->push(tmp);
					}
					break;
				case IF_ELSE:
					tmp = exectx_ifelse(head, l, context, cmdstat);
					if(tmp != NULL){
						args->push(tmp);
					}
					break;
				case RETURN_VAL:
//					log_stack("111 ............. return context %s", head->sat(0).c_str());
					tmp = execute_operation(head->sat(0), l, context);
					log_stack("............. return context  [%s] has result %s", head->sat(0).c_str(), blurt(tmp).c_str());
					return tmp;
				case BREAK_CTX:
					cmdstat = BREAK_CTX;
//					printf("execute_command_node BREAK.....\n");
//					printf("......execute_command_node cmdstat = %i\n", cmdstat);
					return NULL;
				case FOR_LOOP:
					args->push(exectx_for(head, l, context, cmdstat));
					break;
				case CONTINUE_CTX:
					cmdstat = CONTINUE_CTX;
					log_stack("CONTINUE_CTX");
					break;
				case SWITCH_CASE:
					args->push(exectx_switch(head, l, context, cmdstat));
					break;
				case FUNCTION_DEF:
					fdef = build_new_function(head->sat(0), context, l);
					if(fdef.name.empty()){
						args->push(fdef.func);
					}
					break;
				case WHILE_LOOP:
					args->push(exectx_while(head, l, context, cmdstat));
					break;

				case DO_WHILE:
					args->push(exectx_do_while(head, l, context, cmdstat));
					break;

				case TRY_CATCH:
					tmp = exectx_try_catch(head, l, context, cmdstat);
					log_stack("tryu r = %s ", blurt(tmp).c_str());
					args->push(tmp);
					break;

				case DELETE_CTX:
					tmp = context->rget(head->sat(0), l, context, DELETE_CTX, Value::npos);
					break;

				case THROW_CTX:
					log_stack("throw error %s", head->sat(0).c_str());
					tmp = get_member(head->sat(0), context, l);
					log_stack("throw obj %s", blurt(tmp).c_str());
					Error::throwValue(tmp);
					break;

				default:
					log_stack(",.................head %s", blurt(head).c_str());
					method_not_implemented();
			}


			//check next if exist, and stat is ok
			if(head->next != NULL &&
					cmdstat != BREAK_CTX &&
					cmdstat != RETURN_VAL &&
					cmdstat != THROW_CTX &&
					cmdstat != CONTINUE_CTX) {
					return execute_command_node(head->next, l, context, args, cmdstat);
			}


			return tmp;
		};//execute_command_node
		/**
		 * detects instruction type,
		 * converts strings to command nodes,
		 * execute_command_node(nodes)
		 * @param in
		 * @param l
		 * @param context
		 * @return
		 */
		Value * execute_command(string in, const lexic &l, Function *& context, int &cmdstat) {
			trim(in);
			log_stack("%s", in.c_str());

			command_node cmd;
			detect_instruction_type(in, &cmd, l);
			Array * args = Array::New(l); //TODO modify
			Value * ret = execute_command_node(&cmd, l, context, args, cmdstat);

			if(ret != NULL){
				return ret;
			}

			//BUT WHY???
			if(args->length() == 1 && !args->at(0)->isArray()){
				return args->at(0);
			}
			return args;
		}//execute_command


		static inline void trim_chars(string &key){
			key = key.substr(1, key.size()); // scot primul
			key = key.substr(0, key.size() - 1); //scot ultimul
		}

		Value * build_new_array(stxtr in, Function * &context, const lexic &l){
			log_stack("build new array | %s |", in.c_str());
			Array * xa = new Array(l);
			if (in.empty()) {
				return xa;
			}

			int sct = 0;
			stxtr al;
			stxtr rs;
			for(size_t i = 0; i < in.size(); i++){
				quota_stat qs;
				quota_stat_read(qs, l, in, i);
				char c = in.at(i);
				if(quota_stat_valid(qs)){
					if(c == l.l_sqbracket){
						sct++;
					}
					if(c == l.r_sqbracket){
						sct--;
					}

					if(sct == -1){
						rs = in.substr(i + 1, in.size());
						log_stack("array line = <%s>  rest = <%s>" , al.c_str(), rs.c_str());
						break;
					}
				}
				al+=c;
			}



			vector<string> p = split_by_comma(al, l);
			for(int j = 0; j < p.size(); j++){
				//log_stack("parsing %s", p.at(j).c_str());
				Value * s = execute_operation(p.at(j), l, context);
				//log_stack("2n parsing %s", blurt(s).c_str());
				xa->push(s);
			}

			rs = trim(rs);
			if(!rs.empty()){
				if(rs.at(0) != l.l_sqbracket){
					throw_error("invalid input here boy %s!!!", rs.c_str());
				}
				rs += l.r_sqbracket;
				log_stack("getRecursive %s", rs.c_str());
				return xa->getRecursive(rs, l, context, 1, RETURN_VAL, NULL);
			}

			return xa;
		}

		Object * build_new_object(string in, Function * &context, const lexic &l){
			log_stack("init with %s", in.c_str());

			Object * response = Object::New(l);
			vector<string> parts = split_by_comma(in, l);
			log_stack("start for");
			for(int j = 0; j < parts.size(); j++){
				string s = parts.at(j);
				log_stack("for string %s", s.c_str());
				
					vector<string> p = pair_by_colon(s, l);

					string kn = trim(p.at(0));
					log_stack(".getContextKey............................. %s", context->getContextKey().c_str());
					bool n = context->getContextKey() == (kn);
					if(n){
						log_stack("--- rget --- %s", kn.c_str());
						kn = context->rget(kn, l, context, RETURN_VAL, NULL)->str();
					}
					if (kn == "e") {
						printf("dbg");
					}
					log_stack("now %s", kn.c_str());
					Value * tmpval =  execute_operation(p.at(1), l, context);
					//log_stack("%s", blurt(tmpval));
					response->addKey(kn, tmpval);
				
			}
			return response;
		}



		function_def build_new_function(string in, Function * &context, const lexic &l){
			trim(in);
			printf("%s\n\t%s %s\n", ASTX_CURRENT_FUNCTION, in.c_str(), ASTX_LOCATION);

			in = in.substr(strlen(l.func), in.size());
			printf("\trem %s\n", in.c_str());
			string fn;
			for(int i = 0; i < in.size(); i++){
				char c = in.at(i);
				if(c == l.l_parenthesis){
					break;
				}
				fn+=c;
			}
			printf("\tfn %s\n", fn.c_str());
			int lp = 0;
			string args = get_expression_between(in, l.l_parenthesis, l.r_parenthesis, lp, l);
			printf("\targs %s\n", args.c_str());
			in = in.substr(lp, in.size());
			printf("\trem %s\n", in.c_str());
			string body = get_expression_between(in, l.l_bracket, l.r_bracket, lp, l);
			printf("\tbody %s\n", body.c_str());
			function_def fd;

			fd.func = new Function(body, args, context, NULL, NULL, NULL, l);

			fd.name = trim(fn);
			if(!fd.name.empty()){
				context->addKey(fd.name, fd.func);
			}
			fd.func->setParent(context);
			return fd;
		};

		bool string_is_func_format(string in, const lexic &l){
			return string_starts_with(in, l.func, l.l_parenthesis, l);
		};

		stx_regex_flag flag_from_string(string af, int &v, const lexic &l){
			log_stack("af = %s", af.c_str());
			 stx_regex_flag cf = JSREG_NONE;
			 v = -12;
			 if(af.at(0) == l.regex_icase){  cf = JSREG_IGNORECASE; v = 1;}
			 if(af.at(0) == l.regex_global){  cf = JSREG_GLOBAL;  v = 1;}
			 if(af.at(0) == l.regex_multiline) {  cf = JSREG_MULTILINE;  v = 1; }
			 if(af.at(0) == l.regex_unicode){  cf = JSREG_UNICODE;  v = 1;}
			 if(af.at(0) == l.regex_sticky) {  cf = JSREG_STICKY;  v = 1;}
			 if(af.at(0) == l.regex_nosub) { cf = JSREG_NOSUBS;  v = 1; }

			 if(af.size() > 1){
				 af = af.substr(1, af.size());
				 return cf | flag_from_string(af, v, l);
			 }
			 return cf;
		}

		RegExp * build_regex_from_string(string in, const lexic &l){
			trim(in);
			if(in.empty() || in.size() < 3){
				return NULL;
			}


			char p = in.at(0);
			if(p != l.regex_delim){
				return NULL;
			}

			char u = in.at(in.size() - 1);


			if(u == l.regex_delim){
				if(in.size() == 2) {return NULL; }
				trim_chars(in);
				return new RegExp(in.c_str(), JSREG_BASIC, l);
			}

			size_t lpoz = in.find_last_of(l.regex_delim);

			if (lpoz != string::npos && lpoz != 0){
				 string af = in.substr(lpoz + 1, in.size());
				 in = in.substr(1, lpoz - 1);
				 log_stack("in = %s, af = %s", in.c_str(), af.c_str());
				 int val = 0;
				 stx_regex_flag f = flag_from_string(af, val, l);
				 log_stack("val = %i", val);
				 if(val > 0){
					 return new RegExp(in.c_str(), f, l);
				 }

		 	}

			return NULL;
		}

		Value * get_member(string key, Function * &context, const lexic &l, Value * av, bool includerget){

			trim(key);
			log_stack("in = %s", key.c_str());

			int ks = (int) key.size();
			if (key.empty()) {
				return Value::npos;
			}
			char p = key.at(0);
			if (key == l.vthis) { return context->thisInstance(); }
			if (key == l.vundefined) { return Value::npos; }
			if (key == l.vtrue) { return Boolean::bTrue(l);  }
			if (key == l.vfalse) {  return Boolean::bFalse(l); }
			if (ks == 1 && p == l.q1) { return String::New(l.q1, l); }
			if (ks == 1 && p == l.q2) { return String::New(l.q2, l); }

			// ++var
			if (ks > 1 && p == l.plus && key.at(1) == l.plus) {
				key = key.substr(2, ks);
				printf("\tpp_v %s\n", key.c_str());
				return  get_member(key, context, l, av)->plusplus_v();
			}
			// var++
			if (ks > 1 &&  key.at(ks - 1) == l.plus && key.at(ks - 2) == l.plus) {
				key = key.substr(0, ks - 2);
//				printf("\tv_pp %s\n", key.c_str());
				return  get_member(key, context, l, av)->v_plusplus();
			}
			// --var
			if (ks > 1 && p == l.minus && key.at(1) == l.minus) {
				key = key.substr(2, ks);
//				printf("\tpp_v %s\n", key.c_str());
				return  get_member(key, context, l, av)->minmin_v();
			}
			// var--
			if (ks > 1 &&  key.at(ks - 1) == l.minus && key.at(ks - 2) == l.minus) {
				key = key.substr(0, ks - 2);
				printf("\tv_mm %s\n", key.c_str());
				return get_member(key, context, l, av)->v_minmin();
			}
			// !var
			if(ks > 0 && p == l.negation){
				key = key.substr(1, key.size());
				return get_member(key, context, l, av)->negate();
			}
			// ~var
			if(ks > 0 && p == l.bitnot){
				key = key.substr(1, key.size());
				return get_member(key, context, l, av)->negateBit();
			}

			//first check for numeric types, then for +var or -var
			if (string_is_numeric(key, l)) { return Number::New(key, l); }

			// +var
			if(ks > 0 && p == l.plus){
				key = key.substr(1, key.size());
				return get_member(key, context, l, av)->v_plus();
			}
			// -var
			if(ks > 0 && p == l.minus){
				key = key.substr(1, key.size());
				return get_member(key, context, l, av)->v_min();
			}





		    char u = *key.rbegin();
		    if(p == l.q1 && u == l.q1){  //  " ... "
				trim_chars(key);
				return String::New(key, l);
			}
		    if(p == l.q2 && u == l.q2){  //  ' ... '
				trim_chars(key);
				log_stack("new string %s", key.c_str())
				return String::New(key, l);
			}
		    if(p == l.l_bracket && u == l.r_bracket){  //  { ... }
		    	trim_chars(key);
		    	return build_new_object(key, context, l);
		    }
		    if(p == l.l_sqbracket && u == l.r_sqbracket){  //  [ ... ]
				trim_chars(key);
				return build_new_array(key, context, l);
			}

		    if(string_is_func_format(key, l)) {
		    	return build_new_function(key, context, l).func;
		    }

		    if(string_starts_with(key, l.typeofdef, ' ', l)) {
		    	key = key.substr(strlen(l.typeofdef), key.size());
				return get_member(key, context, l, av)->typeOf();
			}
		    RegExp * r = build_regex_from_string(key, l);
		    if(r != NULL) {
		    	log_stack("return regex %s", blurt(r).c_str());
		    	return r;
		    }

		    if(includerget){
		    	 return context->rget(key, l, context, RETURN_VAL, av);
		    }
		    return Value::npos;
		};

		bool char_is_operator(char c,  const lexic &l){
			return (c == l.plus ||
				   c == l.minus ||
				   c == l.divide ||
				   c == l.multiply ||
				   c == l.op_and ||
				   c == l.op_and ||
				   c == l.less ||
				   c == l.bigg);
		};


		void quota_stat_read(quota_stat &cst, const lexic &l, const string &input, int i){
			char b = input.at(i);
			char p = i > 0 ? input.at(i - 1) : '\0';
			if(b == l.q1 && p != l.eschar){ cst.ndq++; }
			if(b == l.q2 && p != l.eschar){ cst.nsq++; }
		}//quota_stat_read

		void char_stat_read(char_stat &cst, const lexic &l, const string &input, int i){
			char b = input.at(i);
			char p = i > 0 ? input.at(i - 1) : '\0';
			if(b == l.q1 && p != l.eschar){ cst.ndq++; }
			if(b == l.q2 && p != l.eschar){ cst.nsq++; }

			if(b == l.l_parenthesis){ cst.p++; }
			if(b == l.r_parenthesis){ cst.p--; }

			if(b == l.l_bracket){ cst.b++; }
			if(b == l.r_bracket){ cst.b--; }

			if(b == l.l_sqbracket){ cst.s++; }
			if(b == l.r_sqbracket){ cst.s--; }

			if(cst.p < 0 || cst.b < 0 || cst.s < 0) {
				Error n("invalid input" + input, ASTX_LOCATION, ASTX_CURRENT_FUNCTION);
				throw n;
			}
		}//char_stat_read

		bool char_stat_valid(char_stat &cs){
			return cs.ndq % 2 == 0 && cs.nsq % 2 == 0 && cs.p == 0 && cs.b == 0 && cs.s == 0;
		}//char_stat_valid

		bool quota_stat_valid(quota_stat &cs){
			return cs.ndq % 2 == 0 && cs.nsq % 2 == 0;
		}//char_stat_valid

		vector<string> split_by_separator(string input, const lexic &l) {
			string temp;
			char_stat cst;
			vector<string> r;
			using namespace stringAlg;

			for (unsigned int i = 0; i < input.size(); i++) {
				char b = input.at(i);
				char_stat_read(cst, l, input, i);
				if(char_stat_valid(cst) && b == l.delimiter) {
					trim(temp);
					if(!temp.empty()){
						r.push_back(temp);
						temp.clear();
					}
				} else {
					temp += b;
				}
			}
			trim(temp);
			if (!temp.empty()) {
				r.push_back(temp);
				temp.clear();
			}
			return r;
		}//split_by_separator

		vector<string> pair_by_colon(string input, const lexic &l){
			string temp;
			char_stat cst;
			vector<string> response;
			int i = 0;
			for (i = 0; i < (int) input.size(); i++) {
				char b = input.at(i);
				char_stat_read(cst, l, input, i);
				if(char_stat_valid(cst) &&
				   input.at(i) == l.colon) {
				   response.push_back(temp);
				   break;
				} else {
					temp+=b;
				}
			}

			if(i+1 >= input.size()){
				throw_error("invalid pair input [%s]", input.c_str())
				response.clear();
				return response;
			}
			string rest = input.substr(i+1, input.size());
			response.push_back(rest);
			return response;
		};

		vector<string> split_by_comma(string input, const lexic &l) {
				using namespace stringAlg;
				string temp;
				char_stat cst;
				vector<string> response;

				for (unsigned int i = 0; i < input.size(); i++) {
					char b = input.at(i);
					char_stat_read(cst, l, input, i);

					if(char_stat_valid(cst) && input.at(i) == l.comma) {
						trim(temp);
						if(!temp.empty()){
							response.push_back(temp);
							temp.clear();
						}
					} else {
						temp += b;
					}
				}

				trim(temp);
				if (!temp.empty()) {
					response.push_back(temp);
					temp.clear();
				}
				return response;
		}//split_by_comma


		stxtr get_expression_between(string input, char s, char e, int &lastPosition, const lexic &l) {
			size_t position = input.find(s);
			string answer;
			int numInnerP = 0;
			int nd = 0;
			int ns = 0;
			quota_stat qst;
			for (size_t i = position + 1; i < input.size(); i++) {
				char c = input.at(i);
				char p = (i > 0) ? input.at(i - 1) : '\0';
				quota_stat_read(qst, l, input, (int)i);
				if (quota_stat_valid(qst)) {
					if (c == s) {
						numInnerP++;
					}
					if (c == e) {
						if (numInnerP == 0) {
							lastPosition = (int)(position + ((int)answer.size()) + 2);
							return answer;
						}
						numInnerP--;
					}
					answer += c;
				} else {
					answer += c;
				}
			}
			lastPosition = (int)(position + ((int)answer.size()) + 2);
			return answer;
		}//get_expression_between

		/**
		 * use eval
		 * @param input
		 * @param lexic
		 * @param context
		 * @return
		 */
		Value * exec_script(string input, const lexic &lexic, Function * &context){
			return eval(input, lexic, context);
		};

		/**
		 * uses exec_script
		 * @param fname
		 * @param lexic
		 * @param context
		 * @return
		 */
		Value * exec_script_file(string fname, const lexic &lexic, Function * &context){
			Path p(fname);
			fname = p.resolve(FileSystem::getCWD());
			FileInfo f(fname);

			if(!f.exist()){
				Error n("script path [" + fname + "] not found",
						ASTX_LOCATION, ASTX_CURRENT_FUNCTION);
				throw n;
			}
			ifstream o;
			string line, input;
			o.open(fname.c_str(), ios::in);
			if (o.is_open()) {
				while (getline(o, line)) {
					input.append(line);
				}
				o.close();
			} else {
				Error n("cannot open path [" + fname + "]",
						ASTX_LOCATION, ASTX_CURRENT_FUNCTION);
				//throw n;
				log_stack("%s does not exist", fname.c_str());
				system("pause");
				std::exit(-1);
			}


			return exec_script(input, lexic, context);
		};




		string strip_comments(string in, const lexic &l){
			vector<string_stat> s;
			return strip_comments(in, l, &s);
		}

		string strip_comments(string in, const lexic &l,  vector<string_stat> * sts){
				sts->clear();
				quota_stat cst;
				char c = '\0', n = '\0',  p = '\0';
				bool cblock = false, cline = false;
				string  buf, lbuf;
				for(int i = 0; i < in.size(); i++){
					c = in.at(i);
					n = (i < in.size() - 1) ? in.at(i+1) : '\0';
					p = (i > 0) ? in.at(i - 1) : '\0';

//					log_stack("p = %c, c = %c, n = %c", p, c, n);

					quota_stat_read(cst, l, in, i);

					if(quota_stat_valid(cst)){
						if (c == l.cblkinit[0] && n == l.cblkinit[1] && !cline) { // /*
							if(sts != NULL && !lbuf.empty()){
//								log_stack("cblock init text = %s", lbuf.c_str());
								string_stat e = {NONE, lbuf};
								sts->push_back(e);
								lbuf.clear();
							}
							cblock = true;
						}

						else if (c == l.cblkend[1] && p ==l.cblkend[0] && cblock) { // */
							if(sts != NULL && !lbuf.empty()){
//								log_stack("cblock end comment block = %s", lbuf.c_str());
								string_stat e = {COMMENT_BLOCK, lbuf};
								sts->push_back(e);
								lbuf.clear();
							}
							cblock = false;
							continue;
						}
						else if (c == l.clinit[0] && n == l.clinit[1]  && !cblock) { //
							if(sts != NULL && !lbuf.empty()){
//								log_stack("cline init text = %s", lbuf.c_str());
								string_stat e = {NONE, lbuf};
								sts->push_back(e);
								lbuf.clear();
							}
							cline = true;
						}
						else if (c == l.clend && cline) { // \n
							cline = false;
							if(sts != NULL && !lbuf.empty()){
//								log_stack("cline end cline = %s", lbuf.c_str());
								string_stat e = {COMMENT, lbuf};
								sts->push_back(e);
								lbuf.clear();
							}
							continue;
						}

					}



					if(!cline && !cblock){
						buf+=c;
					}
					lbuf+=c;
				}
				if(sts != NULL && !lbuf.empty()){
//					log_stack("after for = %s", lbuf.c_str());
					string_stat e = {NONE, lbuf};
					sts->push_back(e);
					lbuf.clear();
				}
//				log_stack("buf = %s", buf.c_str());

				return buf;
		};//strip_comments

	}//exit namespace scriptengine



	stxtr blurt(scriptengine::command_node * c){
		using namespace scriptengine;
		if(!c){
			return "NULL";
		}
		const char * t;
		switch(c->type){
			case SWITCH_CASE: t = "SWITCH_CASE"; break;
			case BREAK_CTX: t = "BREAK_CTX"; break;
			case TRY_CATCH: t = "TRY_CATCH"; break;
			case FUNCTION_DEF: t = "FUNCTION_DEF"; break;
			default: t = "UNSET";
		}
		stxtr r;
		r.append(t);

		return r;
	};

	stxtr blurt(scriptengine::operation_node * op){
		using namespace scriptengine;
		stxtr r = "";

		if(!op){
			return "NULL";
		}

		
		if(op->value != NULL){
			r.append(op->value->str());
		} else {
			r.append("NULL");
		}
		const char * x;
		switch(op->operation){
			case PLUS: x = "PLUS"; break;
			case PLUS_PLUS: x = "PLUS_PLUS"; break;
			case NO_OPERATION: x = "NO_OPERATION"; break;
			case MINUS: x = "MINUS"; break;
			case MULTIPLY: x = "MULTIPLY"; break;
			case DIVIDE: x = "DIVIDE"; break;
			case GREATER:x = "GREATER"; break;
			case LESSER: x = "LESSER"; break;
			case LESSER_EQ: x = "LESSER_EQ"; break;
			case GREATER_EQ: x = "GREATER_EQ"; break;
			case SHIFT_LEFT: x = "SHIFT_LEFT"; break;
			case SHIFT_RIGHT: x = "SHIFT_RIGHT"; break;
			case EQUALS: x = "EQUALS"; break;
			case EQUALS_STRICT: x = "EQUALS_STRICT"; break;
			case NOT_EQUALS: x = "NOT_EQUALS"; break;
			case NOT_EQUALS_STRICT: x = "NOT_EQUALS_STRICT"; break;
			case BIT_OR: x = "BIT_OR"; break;
			case BIT_AND: x = "BIT_AND"; break;
			case OR: x = "OR"; break;
			case AND: x = "AND"; break;
			case INSTANCEOF: x = "INSTANCEOF"; break;
			case OP_IN: x = "OP_IN"; break;

			default:
				x = "undefined_index_";
				break;
		}
		r.append(x);

	
		
		return r;
	}//stringify operation_node




	void var_dump(scriptengine::operation_node * op,  bool bth) {
		if(bth){
			ASTX_BLURT_THIS_FUNC;
			printf("%s", blurt(op).c_str());
		} else {
			printf("scriptengine::operation_node %s ", blurt(op).c_str());
		}
	}//blurt operation_node

	void var_dump(scriptengine::operation_node * op, char * fmt, ...){
		 printf("%s", blurt(op).c_str());
		 va_list args;
		 va_start(args,fmt);
		 vprintf(fmt,args);
		 va_end(args);
	};
}//exit namespace astox
