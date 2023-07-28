#pragma once
#include "ast.hpp"
#include "syntax_validator.hpp"

class parser {
	public:
		parser(std::shared_ptr<lexer::LexVector> vec) {
			syntax_validator s(vec);
			auto ret_syntax = s.check_syntax_tokens();
			if(ret_syntax.first != syntax_validator::SYNTAX_SUCCESS) {
				std::cerr << "Syntax error! " << s.GetLastError(ret_syntax.first) << " at line : " 
					<< std::get<int>(ret_syntax.second) << std::endl;
				std::exit(1);
			}
			ast_vec = std::move(std::get<std::vector<std::shared_ptr<AST::AnyAST>>>(ret_syntax.second));

		};
		virtual ~parser() {

		};
		std::vector<std::shared_ptr<AST::AnyAST>> ast_vec{};
		std::unordered_map<std::string_view, AST::variable> var_map{};
	protected:	
		std::shared_ptr<lexer::LexVector> __lex_vec{};

};

