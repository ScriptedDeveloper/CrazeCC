#pragma once
#include "ast.hpp"
#include <cstdio>
class syntax_validator;
#include "syntax_validator.hpp"

using ExpressionRet = std::pair<int, int>;

namespace generate_ast {
		
	class function_call : public syntax_validator {
		public:
			function_call(std::shared_ptr<lexer::LexVector> &lex, bool &is_function_call, bool is_child = false) : syntax_validator(lex) {
				is_function_call = true;
				__is_child = is_child;

			};
			virtual ~function_call() {};
			ExpressionRet check(lexer::token &token, bool &is_function_call, int &line, std::shared_ptr<AST::AnyAST> &last_expr);

		private:
			ExpressionRet assign(AST::function_call &last_expr, bool &is_function_call, lexer::token &t);
			inline bool is_invisible_char(std::string_view token) {
				return token[0] == '\t' || token[0] == '\n';
			}
			bool __is_child{};
		
	};
	class variable : public syntax_validator {
		public:
			variable(std::shared_ptr<lexer::LexVector> &lex, bool &is_variable) : syntax_validator(lex) {
				is_variable = true;
				lex_vec = lex;
				f = std::make_shared<AST::AnyAST>();
			}
			virtual ~variable(){};	
			ExpressionRet check(lexer::token &token, 
				bool &is_variable, int &potential_last_error, bool &complete);
			
			static std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> var_map;

		private:
			ExpressionRet assign(lexer::token &token, auto &last_expression, bool &is_variable, 
				int line, int &potential_last_error, bool &complete);

			std::shared_ptr<lexer::LexVector> lex_vec{};
			bool is_function_call{};
			std::shared_ptr<AST::AnyAST> f; // in case the variable directly calls a function
	
	};
	class function : public syntax_validator {
		public:
			function(std::shared_ptr<lexer::LexVector> &lex, bool &is_function) : syntax_validator(lex) {
				is_function = true;
				complete = false;

			};
			ExpressionRet check(lexer::token &token, 
			 bool &is_function, bool &complete);

	//		static std::unordered_map<std::string_view, std::vector<std::vector<AST::AnyAST>::iterator>> param_map;
			/*
			 * Stores the function pointers by name.
			 */
			static std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> function_map;
			static void check_is_function_body();

		private:
			std::vector<AST::AnyAST> function_body{};	
			ExpressionRet assign(lexer::token &token, 
			 bool &is_function, bool &complete);

	};
	class if_statement : public syntax_validator {
		public :
			if_statement(std::shared_ptr<lexer::LexVector> &lex) : syntax_validator(lex) {

			};
		bool body_completed{false};
	};


	class return_gen : public syntax_validator {
		public:
			return_gen(std::shared_ptr<lexer::LexVector> &lex, bool &is_return) : syntax_validator(lex) {
				is_return = true;
			};
			ExpressionRet check(lexer::token &token, bool &is_return, int &line, std::shared_ptr<AST::AnyAST> &last_expr);
		private:
			ExpressionRet assign(std::shared_ptr<AST::AnyAST> &ret_ptr, bool &is_return, lexer::token &t);

	};
};


