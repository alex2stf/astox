/*
 * compiler.h
 *
 *  Created on: 21 Apr 2017
 *      Author: alex
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#include "types.h"


namespace astox {

	/**
	 * scriptengine namespace
	 */
	namespace scriptengine {




		using namespace std;


		/**
		 * verify if a string is numeric format based on specified lexic.
		 * @param in
		 * @param lexic
		 * @return
		 */
		bool string_is_numeric(string in, const lexic &lexic);

		/**
		 * a node of commands, containing a number of statements and blocks
		 */
		struct command_node {
			std::vector<stxtr> stmts;  //()
			std::vector<stxtr> blcks;  //{}
			int type;
			command_node * next;
			command_node(){
				next = NULL;
				type = 0;
			}
			stxtr sat(int x){
				return stmts.at(x);
			}
			stxtr bat(int x){
				return blcks.at(x);
			}
		};

		/**
		 * an operation node, containing a Value and an ::Operations
		 */
		struct operation_node {
			int operation; // '
			Value * value;
			struct operation_node * next;
			struct operation_node * previous;
			bool has_next();
			bool has_previous();
			operation_node();
			~operation_node();
			struct operation_node * get_root();
		};

		/**
		 * string stat enum for stripping comments. @see #strip_comments
		 */
		enum strstats {
			COMMENT, COMMENT_BLOCK, NONE
		};

		struct string_stat {
			int stat;
			string data;
		};

		/**
		 * function_def struct, containing a Function and an optional name
		 */
		struct function_def {
			Function * func;
			std::string name;
		};


		int first_operation_index(string in, const lexic &l);
		/**
		 * return value only if assign
		 * @param in
		 * @param lexic
		 * @param head
		 * @param context
		 * @return
		 */
		Value * extract_operation_nodes(string in, const lexic &lexic,
				operation_node * head, Function * &context);

		Value * execute_operation_nodes(operation_node * head, Function * &context);
		void extract_case_statements(string input, const lexic &lexic, std::vector<string> &lefts, std::vector<string> &rights);
		vector<string> split_by_keyword(string input, const char * key, const lexic &lexic);
		/**
		 * executes a simple operation with no return type
		 * @param in
		 * @param lexic
		 * @param context
		 * @return
		 */
		Value * execute_operation(string in, const lexic &lexic, Function * &context);

		void operate(operation_node * right, operation_node * left, Function * &context);

		/**
		 *
		 * @param input
		 * @param allowed_chars
		 */
		bool string_starts_with(string &input, const char * start, char  allowed_char_after, const lexic &lexic);

		bool string_starts_with(string &input, const char * start, const lexic &lexic);

		/**
		 * verify if a string ends with a specified input, and before that input contains
		 * a character valided by lexic#wscnd
		 * @param input
		 * @param end
		 * @return true on success
		 */
		bool string_ends_with(string &input, const char * end, const lexic &lexic);
		void detect_instruction_type(string input, command_node * head, const lexic &lexic);
		Value * eval_command(string input, const lexic &lexic, Function * &context, int &cmdstat);
		Value * eval(string input, const lexic &lexic, Function * &context);
		Value * exec_script(string input, const lexic &lexic, Function * &context);
		Value * exec_script_file(string input, const lexic &lexic, Function * &context);

		/**
		 * execute a strict separated  command
		 * @param input
		 * @param index
		 * @param context
		 */
		Value * execute_command(string input, const lexic &lexic, Function *& context, int &cmdstat);
		Value * execute_command_node(command_node * head, const lexic &lexic, Function *& context, Array *& args, int &cmdstat);
		Value * execute_operation_command(string input, const lexic &lexic, Function *& context);
		Value * exectx_for(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);
		Value * exectx_ifelse(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);
		Value * exectx_switch(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);
		Value * exectx_while(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);
		Value * exectx_do_while(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);
		Value * exectx_try_catch(command_node * head, const lexic &lexic, Function *& context, int &cmdstat);

		vector<string> split_by_comma(string input, const lexic &l);
		bool string_is_func_format(string input, const lexic &l);
		RegExp * build_regex_from_string(string input, const lexic &l);
		stx_regex_flag flag_from_string(string in, int &validity, const lexic &l);
		/**
		 * returns 2 strings based on first found colon
		 * @param input
		 * @param l
		 * @return
		 */
		vector<string> pair_by_colon(string input, const lexic &l);
		stxtr get_expression_between(string input, char s, char e, int &lastPosition, const lexic &l);

		/**
		 * quota safe split by separator
		 * @param input
		 * @param lexic
		 * @return
		 */
		vector<string> split_by_separator(string input, const lexic &lexic);
		void trim_separator(string &input, const lexic &lexic);

		Value * get_member(string key, Function * &context, const lexic &l, Value * assignValue = NULL, bool includerget = true);

		Object * build_new_object(string key, Function * &context, const lexic &l);
		Value * build_new_array(string key, Function * &context, const lexic &l);


		/**
		 * creates a function_def datatype based on provided input and lexic.
		 * new created function will be added to context only if name is not empty, otherwise
		 * it will be considered created for anonymous usage
		 * @param in provided input string
		 * @param context context to create in
		 * @param l lexic
		 * @return a new function_def
		 */
		function_def build_new_function(string in, Function * &context, const lexic &l);

		/**
		 *
		 * @param in
		 * @param l
		 * @return
		 */
		string strip_comments(string in, const lexic &l);
		string strip_comments(string in, const lexic &l, vector<string_stat> * stats = NULL);
	}

	void var_dump(scriptengine::operation_node * operation_node, bool bth = false);
	void var_dump(scriptengine::operation_node * operation_node, char * fmt, ...);
	stxtr blurt(scriptengine::operation_node * operation_node);
	stxtr blurt(scriptengine::command_node * command_node);

}

#endif /* COMPILER_H_ */
