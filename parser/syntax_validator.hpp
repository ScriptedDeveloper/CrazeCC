#pragma once
#include "../lexer/lexer.hpp"
#include "ast.hpp"
#include <unordered_map>
#include <memory> 
#include <variant>
class exception_handling;
class syntax_validator;


class syntax_validator {
	public:
		syntax_validator(std::shared_ptr<lexer::LexVector> lex_vec_tmp) {
			__lex_vec = lex_vec_tmp;
		};
		std::pair<int ,std::variant<int, std::vector<AST::AnyAST>>> check_syntax_tokens();
		/*
		 * Kinda similiar to WINAPI, gets the error code in form of basic_string
		 */
		static std::string GetLastError(int error);
		static constexpr int SYNTAX_SUCCESS = -1;
		static constexpr int ERROR_EXPECTED_VALUE = -2;
		static constexpr int ERROR_UNKNOWN_VARIABLE = -3; // in case i forgot to catch something in check_variable()
		static constexpr int ERROR_EXPECTED_TYPE = -4;
		static constexpr int ERROR_EXPECTED_EXPRESSION = -5;
		static constexpr int ERROR_EXPECTED_SEMICOLON = -6;
		static constexpr int ERROR_EXPECTED_EQUAL_SYMBOL = -7;
		static constexpr int ERROR_EXPECTED_FUNCTION_DECLARATION = -10;
		static constexpr int ERROR_EXPECTED_FUNCTION_TYPE = -11;
		static constexpr int ERROR_EXPECTED_FUNCTION_NAME = -12;
		static constexpr int ERROR_EXPECTED_PARAM_TYPE = -13;
		static constexpr int ERROR_EXPECTED_PARAM_VALUE = -14;
		static constexpr int ERROR_EXPECTED_PARENTHESIS = -15;
		static constexpr int ERROR_UNEXPECTED_PARENTHESIS = -16;
		static constexpr int SYNTAX_FUNCTION_FINISHED = -17;
		static constexpr int ERROR_INVALID_KEYWORD = -18;


	protected:	
		/*
		 * We declare the variables static so the inherited classes (variable, if_statement
		 * etc..) from generate_ast.cpp can access them without copies
		 */
		using expression_variant = std::pair<AST::AnyAST, bool>;
		static std::vector<AST::AnyAST> ast_vector;
		static std::shared_ptr<lexer::LexVector> __lex_vec;
		static int line;
		static std::unique_ptr<expression_variant> last_expression;
		static std::unordered_map<std::string_view, std::shared_ptr<AST::AnyAST>> var_map;

	private:
		std::pair<int, int> check_if_statement(lexer::token &token, 
				 bool &is_if, int &potential_last_error, bool &complete) {

			return {SYNTAX_SUCCESS, -1};
		}	
		std::pair<int, int> check_function(lexer::token &token, 
				 bool &is_function, int &potential_last_error, bool &complete);

		inline void clear_expression();


};
#include "generate_ast.hpp"
