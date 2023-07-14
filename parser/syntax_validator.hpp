#pragma once
#include "../lexer/lexer.hpp"
#include "ast.hpp"
#include <unordered_map>
#include <memory>
#include <variant>

class exception_handling;


class syntax_validator {
	public:
		syntax_validator(lexer::LexVector &lex_vec_tmp) {
			__lex_vec = lex_vec_tmp;
		};
		std::pair<int ,std::variant<int, std::vector<AST::AnyAST>>> check_syntax_tokens() {	
			/*
			 * the bool in the pair tells us whether the expression is existing or not.
			 */
			bool is_variable{false}, complete{false};
			bool is_if{false};
			int potential_last_error{SYNTAX_SUCCESS}; // in case we go to the next expression, we can look back in case something's missing
			for(auto token : __lex_vec) {
				if(complete && potential_last_error != SYNTAX_SUCCESS)
					return {potential_last_error, line}; // something went wrong in the past line
				if(is_variable || token.is_data_type()) {
					auto variable_ret = check_variable(token, is_variable, potential_last_error, complete);
					if(variable_ret.first != SYNTAX_SUCCESS)
						return variable_ret;
					if(token.is_semicolon())
						ast_vector.push_back(last_expression->first);
				} else if(is_if || (token.is_keyword() && token.data() == "if")) {
					auto if_ret = check_if_statement(token, is_if, potential_last_error, complete);
					if(if_ret.first != SYNTAX_SUCCESS)
						return if_ret;
				} else if(token.is_semicolon())
					line++;
			}
			if(!complete)
				return {potential_last_error, line};
			return {SYNTAX_SUCCESS, std::move(ast_vector)};
		}
		/*
		 * Kinda similiar to WINAPI, gets the error code in form of basic_string
		 */
		static std::string GetLastError(int error) {
			switch(error) {
				case SYNTAX_SUCCESS: {
					return "SYNTAX_SUCCESS";
				};
				case ERROR_EXPECTED_VALUE : {
					return "ERROR_EXPECTED_VALUE";
				};
				case ERROR_UNKNOWN_VARIABLE : {
					return "ERROR_UNKNOWN_VARIABLE";
				};
				case ERROR_EXPECTED_TYPE : {
					return "ERROR_EXPECTED_TYPE";
				};
				case ERROR_EXPECTED_EXPRESSION : {
					return "ERROR_EXPECTED_EXPRESSION";
				};
				case ERROR_EXPECTED_SEMICOLON : {
					return "ERROR_EXPECTED_SEMICOLON";
				};
				case ERROR_EXPECTED_EQUAL_SYMBOL : {
					return "ERROR_EXPECTED_EQUAL_SYMBOL";
				};
				default : 
					return "";
			};
		}
		static constexpr int SYNTAX_SUCCESS = -1;
		static constexpr int ERROR_EXPECTED_VALUE = -2;
		static constexpr int ERROR_UNKNOWN_VARIABLE = -3; // in case i forgot to catch something in check_variable()
		static constexpr int ERROR_EXPECTED_TYPE = -4;
		static constexpr int ERROR_EXPECTED_EXPRESSION = -5;
		static constexpr int ERROR_EXPECTED_SEMICOLON = -6;
		static constexpr int ERROR_EXPECTED_EQUAL_SYMBOL = -7;

		using expression_variant = std::pair<AST::AnyAST, bool>;
		std::vector<AST::AnyAST> ast_vector{};
	private:
		lexer::LexVector __lex_vec{};
		int line{1};
	
		std::unique_ptr<expression_variant> last_expression = std::make_unique<expression_variant>();

		std::pair<int, int> assign_variable(lexer::token &token, auto &last_expression, bool &is_variable, 
				int line, int &potential_last_error, bool &complete) {
			last_expression->second = true; // it exists!
			auto var_obj = std::get<AST::variable>(last_expression->first);
			if(token.is_data_type()) {
				var_obj.define_type(token.data());
				potential_last_error = ERROR_UNKNOWN_VARIABLE;
			}
			else if(token.is_operator()) {
				var_obj.define_equal_symbol();
				potential_last_error = ERROR_EXPECTED_VALUE;
			}
			else if(token.is_semicolon()) {
				var_obj.define_equal_symbol();
				is_variable = false;
				complete = true;
				potential_last_error = SYNTAX_SUCCESS;
			}
			else if(token.is_value()) {
				if(var_obj.defined_equal_symbol()) {
					var_obj.define_value(token.data());
					potential_last_error = ERROR_EXPECTED_SEMICOLON;
				}
				else {
					var_obj.define_name(token.data()); // we gonna assume its the variable name
					potential_last_error = ERROR_EXPECTED_EQUAL_SYMBOL;
					var_obj.define_name(token.data());
				}
			}
			else
				return {ERROR_EXPECTED_VALUE, line};
			last_expression->first = var_obj;
			return {SYNTAX_SUCCESS, -1};
		}
		std::pair<int, int> check_variable(lexer::token &token, 
				 bool &is_variable, int &potential_last_error, bool &complete) {
			is_variable = true;
			complete = false;
			if(token.is_space())
				return {SYNTAX_SUCCESS, -1}; // ignoring spaces
			if(!std::holds_alternative<AST::variable>(last_expression->first) && !token.is_keyword())
				return {ERROR_EXPECTED_TYPE, line}; // compiler error...
			if(!last_expression->second && !token.is_data_type())
				return {ERROR_EXPECTED_TYPE, line};
			if(!last_expression)
				last_expression->first = AST::variable();
			auto var_obj = std::get<AST::variable>(last_expression->first);
			if(token.is_value() && var_obj.defined_value())
				return {ERROR_EXPECTED_SEMICOLON, line};
			if(token.is_semicolon() && !var_obj.defined_value())
				return {ERROR_EXPECTED_EXPRESSION, line};
			if(token.is_operator() && token.data() != "=")
				return {ERROR_EXPECTED_EQUAL_SYMBOL, line};
			/*
			 * Error checking done.. basic
			 * Now lets assign them values
			 */
			return (assign_variable(token, last_expression, is_variable, line, potential_last_error, complete));
		}

		std::pair<int, int> check_if_statement(lexer::token &token, 
				 bool &is_variable, int &potential_last_error, bool &complete) {

			
		}

		std::unordered_map<std::string_view, std::shared_ptr<std::variant<AST::variable>>> var_map{};


};


