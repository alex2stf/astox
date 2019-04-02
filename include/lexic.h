/*
 * lexic.h
 *
 *  Created on: 27 Apr 2017
 *      Author: alex
 */

#ifndef LEXIC_H_
#define LEXIC_H_

#include "macros.h"

namespace astox {

	/**
	 * lexic namespace
	 */
	namespace scriptengine {
			/** method to return bool after verify a single character */
			typedef bool (*is_white_space_condition)(char);


			typedef struct char_stat{
				int nsq; // '
				int ndq; // "
				int p;   // ()
				int b;   // {}
				int s;   // []
				char_stat(){
					nsq = 0; ndq = 0; p = 0; b = 0; s = 0;
				}
			} char_stat;

			typedef struct quota_stat{
				int nsq; // '
				int ndq; // "
				quota_stat(){
					nsq = 0; ndq = 0;
				}
			} quota_stat;

			typedef struct environment {
				const char * undefined;
				const char * string;
				const char * object;
				const char * number;
				const char * boolean;
				const char * array;
				const char * native_func;
				const char * regexp;
				const char * func;
				const char * length;
				const char * reverse;
				const char * trim;
				const char * tojson;
				const char * charat;
				const char * replace;
				const char * split;
				const char * killwhitespace;
				const char * splitbywhitespace;
				const char * splitbychar;
				const char * sort;
				const char * indexof;
				const char * search;
				const char * slice;
				const char * substr;
				const char * lastindexof;
				const char * match;
				const char * tostring;
				const char * nanstr;
				const char * push;
				const char * printfnc;
				const char * array_construct;
				const char * string_construct;
				const char * number_construct;
				const char * object_construct;
				const char * boolean_construct;
				const char * regexp_construct;

			}environment;

			/*!
			 * basic structure for scripting support.
			 * Documentation will show javascript support
			 */
			typedef struct lexic {
				char delimiter; 	  /**< engine delimiter [;] */
				char plus;		 	  /**< addition operator [+], used for numeric formats and addition*/
				char minus;		  	  /**< substract operator [-]  used for numeric formats and substraction*/
				char q1;        	  /**< first quote type ["] */
				char q2;              /**< second quote type ['] */
				char r_parenthesis;  /**< right paranthesis [(] */
				char l_parenthesis;  /**< left paranthesis [(] */
				char l_bracket;      /**< left bracket [{] */
				char r_bracket;      /**< right bracket [}] */
				char l_sqbracket;    /**< left square bracket [[] */
				char r_sqbracket;    /**< right square bracket [[] */
				char colon;          /**< colon separator [:] */
				char negation;       /**< negation sign [!] */
				char dot;            /**< dot character. Used for numeric formats and property selections */
				char eschar;     	  /**< escape character for quotes. astox supports only quote escaping, any other char between quotes is considered valid */
				char equals;  // =
				char comma;
				char op_or;  // |
				char op_xor;  // ^
				char op_and; // &
				char multiply;  // *
				char divide;    // /
				char bigg; // '>'
				char less; // '<'
				char modulus;  // %
				char bitnot; //~
				char ternar;
				char wspace;
				char regex_delim; // /
				char regex_global;//g
				char regex_icase;//i
				char regex_multiline;//m
				char regex_unicode; //u
				char regex_sticky; //y
				char regex_nosub; //n
				const char * typeofdef;
				const char * inswitch;
				const char * inif;
				const char * inelse;
				const char * incase;
				const char * intry;
				const char * incatch;
				const char * infinally;
				const char * indo;
				const char * inwhile;
				const char * infor;
				const char * var;          // var
				const char * vtrue;
				const char * vfalse;
				const char * vundefined;
				const char * vthis;
				const char * vnew;
				const char * func;
				const char * instanceof;
				const char * insreturn;
				const char * insbreak;
				const char * inscontinue;
				const char * indefault;
				const char * insthrow;
				const char * insdelete;
				const char * op_in; // in
				char clinit[2];					 /**< comment line init */
				char clend;						 /**< comment line end */
				char cblkinit[2];				 /**< 2 chars comment block init */
				char cblkend[2];				 /**< 2 chars comment block end */
				is_white_space_condition wscnd;  /**< condition that checks if a character is whitespace */
				environment env;

				 bool operator <(const lexic & r) const {
				    return (bigg < r.bigg) && insthrow < r.insthrow;
				 }

			} lexic;



			int validate_lexic(lexic l);


			enum InstructionTypes {
					VAR_ASSIGN = 5,  /**< var (...) = (...), (...) = (...), (...) = (...); */
					FUNCTION_DEF, 	 /**< function (...) { ... } */
					TRY_CATCH, 		 /**< try { ... } catch (e) {...} finally { ... } */
					DO_WHILE, 		 /**< do { ... } while ( ... ) */
					WHILE_LOOP, 	 /**< while (...) { ... } */
					FOR_LOOP, 		 /**< for ( ... ) { ... } */
					IF_ELSE, 		 /**< if (...) { ... } else if (...) { ... } else { ... } */
					SWITCH_CASE, 	 /**< switch (...) case (...): ... ; case (...): ... ; default: ... ; */
					RETURN_VAL,		 /**< return (...); */
					BREAK_CTX,		 /**< break; */
					CONTINUE_CTX,	 /**< continue; */
					DELETE_CTX,	     /**< delete (...) */
					THROW_CTX,		 /**< throw (...) */
					OPERATION        /**< (...) */
				};


				enum Operations {
					NO_OPERATION = 1,  //!< NO_OPERATION
					PLUS,              //!< +
					MINUS,             //!< -
					MULTIPLY,          //!< *
					DIVIDE,            //!< /
					MODULUS,           //!< %
					PLUS_PLUS,         //!< ++
					MINUS_MINUS,       //!< --
					DIVIDE_DIVIDE,     //!< //
					MULTIPLY_MULTIPLY, //!< **
					GREATER,           //!< >
					LESSER,            //!< <
					GREATER_EQ,        //!< >=
					LESSER_EQ,         // <=
					NEGATE,            // !
					INSTANCEOF,        // instanceof
					SHIFT_RIGHT,       // >>
					SHIFT_RIGHT_UNSG,  // >>
					SHIFT_LEFT,        // <<
					EQUALS,            // ==
					EQUALS_STRICT,     // ===
					NOT_EQUALS,        // !=
					NOT_EQUALS_STRICT, // !=
					BIT_OR,            // |
					BIT_AND,           // &
					OR,                // ||
					AND,               // &&
					ASSIGN,            // =
					OP_IN,             // in
					BIT_XOR,           // ^
					BIT_NOT           // ~
				}; //see a *= b


				enum SelectionType {
					SIMPLE, SQBRACKET, PARANTHESIS
				};


				struct Syntax {
					 static const struct lexic JS;
					 static const struct lexic OBFUS;
				};


				/**
				 *
				 * @param charstat
				 * @param lexic
				 * @param c
				 * @param p previous character, to track escaped quopets like /' or /"
				 */
				void char_stat_read(char_stat &charstat, const lexic &lexic, const std::string &input, int index);
				bool char_stat_valid(char_stat &charstat);
				void quota_stat_read(quota_stat &charstat, const lexic &lexic, const std::string &input, int index);
				bool quota_stat_valid(quota_stat &charstat);

	}
}


#endif /* LEXIC_H_ */
