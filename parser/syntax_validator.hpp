#pragma once
#include "../lexer/lexer.hpp"
#include "ast.hpp"
#include "../preprocessor/preprocessor.hpp"
#include <unordered_map>
#include <stack>
#include <memory> 
#include <map>
#include <variant>
class exception_handling;
class syntax_validator;

using ExpressionRet = std::pair<int, int>;

class syntax_validator {
	public:
		syntax_validator(std::shared_ptr<lexer::LexVector> lex_vec_tmp) {
			__lex_vec = lex_vec_tmp;
			if(last_expression->first == nullptr)
				last_expression->first = std::make_shared<AST::AnyAST>();
		};
		std::pair<int ,std::variant<int, std::vector<std::shared_ptr<AST::AnyAST>>>> check_syntax_tokens();
		/*
		 * Kinda similiar to WINAPI, gets the error code in form of basic_string
		 */
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
		static constexpr int ERROR_UNKNOWN_FUNCTION_CALL = -19;
		static constexpr int ERROR_UNEXPECTED_SEMICOLON = -20;
		static constexpr int ERROR_UNEXPECTED_KEYWORD = -21;	
		static constexpr int ERROR_UNEXPECTED_DATA_TYPE = -22;

		static int potential_last_error; // in case we go to the next 
							  // expression, we can look back in case something's missing

		
	protected:	
		/*
		 * We declare the variables static so the inherited classes (variable, if_statement
		 * etc..) from generate_ast.cpp can access them without copies
		 */
		using expression_variant = std::pair<std::shared_ptr<AST::AnyAST>, bool>;
		static std::vector<std::shared_ptr<AST::AnyAST>> ast_vector;
		static std::shared_ptr<lexer::LexVector> __lex_vec;
		static int line;
		static bool complete;
		static std::unique_ptr<expression_variant> last_expression;
		/*
		 * Stacks keeps track of the curly parenthesis, basically expression blocks
		 */
		static std::stack<std::shared_ptr<AST::AnyAST>> parenthesis_st;

		inline void clear_expression() {
			last_expression = std::make_unique<expression_variant>();
		}

	private:
		/*
		std::pair<int, int> check_if_statement(lexer::token &token, 
				 bool &is_if, int &potential_last_error, bool &complete) {

			return {SYNTAX_SUCCESS, -1};
		}	
		*/
		std::pair<int, int> check_curly_brackets(lexer::token &t, std::shared_ptr<AST::AnyAST> &expression);
		ExpressionRet check_keyword_tokens(lexer::token &token);

		template <typename T>
		void push_objects(auto top, auto if_expr);

		/*
		 * for check_syntax_tokens()
		 */
		bool is_variable{false};
		bool is_function{};
		bool is_if{false};
		bool is_function_call{false};
		bool is_return{false};
		bool is_keyword{false};

};
#include "generate_ast.hpp"
