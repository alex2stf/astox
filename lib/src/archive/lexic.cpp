#include "astox.h"


namespace astox {
	namespace scriptengine {

			using namespace std;



			int validate_lexic(lexic l){

                if(l.cblkinit[0] == l.cblkinit[1]){
						throw_error("comment block init invalid");
					}
					if(l.cblkend[0] == l.cblkend[1]){
						throw_error("comment block end invalid");
					}
					if(l.cblkinit[0] == l.cblkend[0]){
						throw_error("comment block end vs init invalid");
					}
					if(l.cblkinit[1] == l.cblkend[1]){
						throw_error("comment block init vs end invalid");
					}
					return 0;
				}



			const struct lexic Syntax::JS =  { ';', '+', '-', '"', '\'', ')', '(', '{', '}', '[', ']', ':', '!', '.', '\\', '=', ',', '|', '^', '&',
					'*', '/', '>', '<', '%', '~', '?', ' ', '/', 'g', 'i', 'm', 'u', 'y', 'n', "typeof",
					"switch", "if", "else", "case", "try", "catch", "finally", "do", "while", "for", "var" , "true", "false",
					"undefined", "this", "new", "function", "instanceof", "return", "break", "continue", "default", "throw", "delete",
					"in", {'/', '/'}, '\n', {'/', '*'}, {'*', '/'}, util::char_is_whitespace,

					{
							"undefined", "string", "object", "number", "boolean", "array",
							"[native function]", "regexp", "function", "length", "reverse", "trim", "toJSON", "charAt", "replace",
							"split", "killWhiteSpace", "splitByWhiteSpace", "splitByChar", "sort", "indexOf", "search", "slice", "substr",
							"lastIndexOf", "match", "toString", "NaN", "push", "print", "Array", "String", "Number", "Object", "Boolean",
							"RegExp"

					}
			};

			const struct lexic Syntax::OBFUS =  { ';', '+', '-', '"', '\'', ')', '(', '{', '}', '[', ']', ':', '!', '.', '\\', '=', ',', '|', '^', '&',
							'*', '/', '>', '<', '%', '~', '?', ' ', '/', 'g', 'i', 'm', 'u', 'y', 'n', "typeof",
							"switch", "if", "else", "case", "try", "catch", "finally", "do", "while", "for", "var" , "true", "false",
							"undefined", "this", "new", "function", "instanceof", "return", "break", "continue", "default", "throw", "delete",
							"in", {'-', '-'}, '\n', {'#', '='}, {'=', '#'}, util::char_is_whitespace,  {}  };



	}
}
