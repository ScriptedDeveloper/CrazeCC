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
		std::pair<bool ,int> check_syntax_tokens() {	
			/*
			 * the bool in the pair tells us whether the expression is existing or not.
			 */
			auto last_expression = std::make_shared<expression_variant>();
			int line{};
			bool is_variable{false};
			for(auto token : __lex_vec) {
				if(is_variable || token.is_keyword()) {
					auto variable_ret = check_variable(last_expression, token, line, is_variable);
					if(variable_ret.first != SYNTAX_SUCCESS)
						return variable_ret;
				} else if(token.is_semicolon())
					line++;
			}
			return {SYNTAX_SUCCESS, -1};
		}	
		static constexpr bool SYNTAX_SUCCESS = true;
		static constexpr bool ERROR_EXPECTED_VALUE = false;
		static constexpr bool ERROR_UNKNOWN_VARIABLE = false; // in case i forgot to catch something in check_variable()
		static constexpr bool ERROR_EXPECTED_TYPE = false;
		static constexpr bool ERROR_EXPECTED_EXPRESSION = false;
		static constexpr bool ERROR_EXPECTED_SEMICOLON = false;
		static constexpr bool ERROR_EXPECTED_EQUAL_SYMBOL = false;
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
				inline bool is_complete() {
					return __complete;
				}
				bool __defined_keyword{false};
				bool __defined_equal_symbol{false};
				bool __defined_value{false};
				bool __complete{false};
		};
		using expression_variant = std::pair<std::variant<variable>, bool>;
		std::pair<bool, int> check_variable(std::shared_ptr<expression_variant> last_expression, lexer::token &token, 
				int line, bool &is_variable) {
			is_variable = true;
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
			last_expression->second = true; // it exists!
			if(token.is_keyword())
				var_obj.__defined_keyword = true;
			else if(token.is_operator())
				var_obj.__defined_equal_symbol = true;
			else if(token.is_semicolon()) {
				var_obj.__complete = true;
				var_obj.__defined_equal_symbol = true;
				is_variable = false;
			}
			else if(token.is_value())
				var_obj.__defined_value = true;
			else
				return {ERROR_EXPECTED_VALUE, line};
			return {SYNTAX_SUCCESS, -1};
		}

		std::unordered_map<std::string_view, std::shared_ptr<std::variant<variable>>> var_map{};


};


