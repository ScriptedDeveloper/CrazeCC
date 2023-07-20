#pragma once
#include "ast.hpp"
class syntax_validator;
#include "syntax_validator.hpp"


namespace generate_ast {
	class variable : public syntax_validator {
		public:
			variable(std::shared_ptr<lexer::LexVector> &lex) : syntax_validator(lex) {
				
			}
			virtual ~variable(){};	
			std::pair<int, int> check_variable(lexer::token &token, 
				bool &is_variable, int &potential_last_error, bool &complete);
		private:
			std::pair<int, int> assign_variable(lexer::token &token, auto &last_expression, bool &is_variable, 
				int line, int &potential_last_error, bool &complete);
	
	};
	class function : public syntax_validator {
		public:
			function(std::shared_ptr<lexer::LexVector> &lex) : syntax_validator(lex) {

			};
			std::pair<int, int> check_function(lexer::token &token, 
			 bool &is_function, bool &complete);
		private:
			std::vector<AST::AnyAST> function_body{};	
			std::pair<int, int> assign_function(lexer::token &token, 
			 bool &is_function, bool &complete);

	};
};


