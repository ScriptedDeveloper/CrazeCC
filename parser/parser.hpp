#include "ast.hpp"
#include "syntax_validator.hpp"

class parser {
	public:
		parser(lexer::LexVector vec) {
			syntax_validator s(vec);
			auto ret_syntax = s.check_syntax_tokens();
			if(ret_syntax.first != syntax_validator::SYNTAX_SUCCESS) {
				std::cerr << "Syntax error! " << s.GetLastError(ret_syntax.first) << " at line : " 
					<< std::get<int>(ret_syntax.second) << std::endl;
				std::exit(1);
			}
			ast_vec = std::move(std::get<std::vector<AST::AnyAST>>(ret_syntax.second));

		};
		virtual ~parser() {

		};
		std::vector<AST::AnyAST> ast_vec{};
	protected:	
		lexer::LexVector __lex_vec{};

};

