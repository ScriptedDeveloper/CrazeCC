#pragma once
#include "../lexer/lexer.hpp"
#include <unordered_map>
#include <memory>
#include <variant>

class exception_handling;


class syntax_validator {
	public:
		syntax_validator(lexer::LexVector &lex_vec_tmp) {
			__lex_vec = lex_vec_tmp;
		};
		std::pair<int ,int> check_syntax_tokens() {	
			/*
			 * the bool in the pair tells us whether the expression is existing or not.
			 */
			auto last_expression = std::make_shared<expression_variant>();
			int line{1};
			bool is_variable{false}, complete{false};
			int potential_last_error{SYNTAX_SUCCESS}; // in case we go to the next expression, we can look back in case something's missing
			for(auto token : __lex_vec) {
				if(complete && potential_last_error != SYNTAX_SUCCESS)
					return {potential_last_error, line}; // something went wrong in the past line
				if(is_variable || token.is_keyword()) {
					auto variable_ret = check_variable(last_expression, token, line, is_variable, potential_last_error, complete);
					if(variable_ret.first != SYNTAX_SUCCESS)
						return variable_ret;
				} else if(token.is_semicolon())
					line++;
			}
			if(!complete)
				return {potential_last_error, line};
			return {SYNTAX_SUCCESS, -1};
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
	private:
		lexer::LexVector __lex_vec{};
		class variable {
			public:
				variable() {

				};
				inline bool defined_keyword() {
					return __defined_keyword;
				}
				inline bool defined_equal_symbol() {
					return __defined_equal_symbol;
				}
				inline bool defined_value() {
					return __defined_value;
				}
				inline bool defined_name() {
					return __defined_name;
				}
				bool __defined_keyword{false};
				bool __defined_equal_symbol{false};
				bool __defined_value{false};
				bool __defined_name{false};
		};
		using expression_variant = std::pair<std::variant<variable>, bool>;
		std::pair<int, int> assign_variable(lexer::token &token, auto &var_obj, auto &last_expression, bool &is_variable, 
				int line, int &potential_last_error, bool &complete) {
			last_expression->second = true; // it exists!
			if(token.is_keyword()) {
				var_obj.__defined_keyword = true;
				potential_last_error = ERROR_UNKNOWN_VARIABLE;
			}
			else if(token.is_operator()) {
				var_obj.__defined_equal_symbol = true;
				potential_last_error = ERROR_EXPECTED_VALUE;
			}
			else if(token.is_semicolon()) {
				var_obj.__defined_equal_symbol = true;
				is_variable = false;
				complete = true;
				potential_last_error = SYNTAX_SUCCESS;
			}
			else if(token.is_value()) {
				if(var_obj.defined_equal_symbol()) {
					var_obj.__defined_value = true;
					potential_last_error = ERROR_EXPECTED_SEMICOLON;
				}
				else {
					var_obj.__defined_name = true; // we gonna assume its the variable name
					potential_last_error = ERROR_EXPECTED_EQUAL_SYMBOL;
				}
			}
			else
				return {ERROR_EXPECTED_VALUE, line};
			last_expression->first = var_obj;
			return {SYNTAX_SUCCESS, -1};
		}
		std::pair<int, int> check_variable(std::shared_ptr<expression_variant> last_expression, lexer::token &token, 
				int line, bool &is_variable, int &potential_last_error, bool &complete) {
			is_variable = true;
			complete = false;
			if(token.is_space())
				return {SYNTAX_SUCCESS, -1}; // ignoring spaces
			if(!std::holds_alternative<variable>(last_expression->first) && !token.is_keyword())
				return {ERROR_EXPECTED_TYPE, line}; // compiler error...
			if(!last_expression->second && !token.is_keyword())
				return {ERROR_EXPECTED_TYPE, line};
			if(!last_expression)
				last_expression->first = variable();
			auto var_obj = std::get<variable>(last_expression->first);
			if(token.is_value() && var_obj.defined_value())
				return {ERROR_EXPECTED_SEMICOLON, line};
			if(token.is_semicolon() && var_obj.defined_keyword())
				return {ERROR_EXPECTED_EXPRESSION, line};
			if(token.is_operator() && token.data() != "=")
				return {ERROR_EXPECTED_EQUAL_SYMBOL, line};
			/*
			 * Error checking done.. basic
			 * Now lets assign them values
			 */
			return (assign_variable(token, var_obj, last_expression, is_variable, line, potential_last_error, complete));
		}

		std::unordered_map<std::string_view, std::shared_ptr<std::variant<variable>>> var_map{};


};


