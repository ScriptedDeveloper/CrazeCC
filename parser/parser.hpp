#include "syntax_validator.hpp"
/*
 * sorry, no support for reflection
 */
#define watch(x) #x

class parser {
	public:
		parser(lexer::LexVector vec) {
			syntax_validator s(vec);
			auto ret_syntax = s.check_syntax_tokens();
			if(ret_syntax.first != syntax_validator::SYNTAX_SUCCESS) {
				std::cerr << "Syntax error! " << watch(ret_syntax.first) << " at line : " << ret_syntax.second;
			}

		};
		virtual ~parser() {

		};
	protected:	
		lexer::LexVector __lex_vec{};

};

