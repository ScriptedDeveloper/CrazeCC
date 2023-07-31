#include "syntax_validator.hpp"
#include "ast.hpp"
#include "generate_ast.hpp"

/*
 * Static variable declarations
 */

std::vector<std::shared_ptr<AST::AnyAST>> syntax_validator::ast_vector{};
std::shared_ptr<lexer::LexVector> syntax_validator::__lex_vec{};
int syntax_validator::line{1};
bool syntax_validator::complete{false};
std::unique_ptr<syntax_validator::expression_variant> 
syntax_validator::last_expression = std::make_unique<expression_variant>();
std::stack<std::shared_ptr<AST::AnyAST>> syntax_validator::parenthesis_st{};

std::pair<int ,std::variant<int, std::vector<std::shared_ptr<AST::AnyAST>>>> syntax_validator::check_syntax_tokens() {	
	/*
	 * the int in the pair tells us whether the expression is existing or not.
	 */
	bool is_variable{false};
	bool is_function{};
	bool is_if{false};
	bool is_function_call{false};
	int potential_last_error{SYNTAX_SUCCESS}; // in case we go to the next expression, we can look back in case something's missing
	for(auto token : *__lex_vec) {
		if(complete && potential_last_error != SYNTAX_SUCCESS)
			return {potential_last_error, line}; // something went wrong in the past line
		if(is_variable || (token.is_data_type() && !last_expression->second)) {
			generate_ast::variable v(__lex_vec, is_variable);
			auto variable_ret = v.check(token, is_variable, potential_last_error, complete);
			if(variable_ret.first != SYNTAX_SUCCESS)
				return variable_ret;
			if(!is_variable) { // token is a semicolon, variable declaration has ended
				clear_expression();
			}
		} else if(is_if || (token.is_keyword() && token.data() == *lexer::keywords.begin())) {
			/*
			auto if_ret = check_if_statement(token, is_if, potential_last_error, complete);
			if(if_ret.first != SYNTAX_SUCCESS)
				return if_ret;
				*/
		} else if(token.is_semicolon())
			line++;
		else if(is_function || (token.is_keyword() && token.data() == *lexer::keywords.rbegin())) {
			generate_ast::function f(__lex_vec, is_function);
			auto func_ret = f.check(token, is_function, complete); // need check last error
			if(func_ret.first != SYNTAX_SUCCESS)
				return func_ret;
		} else if(token.is_curly_brackets()) {
			auto ret = check_curly_brackets(token, last_expression->first);
			clear_expression(); // we no longer need the other expression
			if(ret.first != SYNTAX_SUCCESS)
				return ret;
		}
		else if(token.is_value() || is_function_call) {
			generate_ast::function_call f_call(__lex_vec, is_function_call);
			auto call_ret = f_call.check(token, is_function_call, line, last_expression->first);
			if(call_ret.first != SYNTAX_SUCCESS)
				return call_ret;
		} else {
			if(!token.is_space())
				return {ERROR_INVALID_KEYWORD, line};
		}
	}
	if(!complete)
		return {potential_last_error, line};
	return {SYNTAX_SUCCESS, std::move(ast_vector)};
}
/*
 * Kinda similiar to WINAPI, gets the error code in form of basic_string
 */
std::string syntax_validator::GetLastError(int error) {
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
		case ERROR_EXPECTED_FUNCTION_DECLARATION : {
			return "ERROR_EXPECTED_FUNCTION_DECLARATION";
		};	
		case ERROR_EXPECTED_FUNCTION_TYPE : {
			return "ERROR_EXPECTED_FUNCTION_TYPE";
		};	
		case ERROR_EXPECTED_PARAM_VALUE : {
			return "ERROR_EXPECTED_PARAM_VALUE";
		};	
		case ERROR_EXPECTED_PARENTHESIS : {
			return "ERROR_EXPECTED_PARENTHESIS";
		};	
		case ERROR_UNEXPECTED_PARENTHESIS : {
			return "ERROR_UNEXPECTED_PARENTHESIS";
		};
		case SYNTAX_FUNCTION_FINISHED : {
			return "SYNTAX_FUNCTION_FINISHED";
		};
		case ERROR_INVALID_KEYWORD : {
			return "ERROR_INVALID_KEYWORD";
		};
		default : 
			return "ERROR_GET_ERROR_CODE";
	};
}	

std::pair<int, int> syntax_validator::check_curly_brackets(lexer::token &t, std::shared_ptr<AST::AnyAST> &expression) {
	/*
	 * sake of simplicity
	 */
	using namespace AST;

	auto token_data = t.data()[0];
	auto curr_expr = parenthesis_st.top();
	if(token_data == '{')
		parenthesis_st.push(expression);
	else {
		if(parenthesis_st.empty() || (curr_expr != nullptr && !std::holds_alternative<if_statement>(*curr_expr) 
		&& !std::holds_alternative<function>(*curr_expr)))
			/*
			 * Either the stack is empty or the expression is not one that can hold a body..
			 * we have a syntax error
			 */
			return {ERROR_UNEXPECTED_PARENTHESIS, line};
		function func_expr{};
		parenthesis_st.pop();
		if(std::holds_alternative<AST::if_statement>(*curr_expr)) {
			auto if_expr = std::get<if_statement>(*curr_expr);
			if(if_expr.defined_code_block())
				/*
				 * Same error as above
				 */
				return {ERROR_UNEXPECTED_PARENTHESIS, line};
			if_expr.define_code_block();
		} else {
			auto func_expr = std::get<function>(*curr_expr);
			if(func_expr.defined_code_block()) {
				return {ERROR_UNEXPECTED_PARENTHESIS, line};
			}
			func_expr.define_code_block();
		}
	}
	return {SYNTAX_SUCCESS, -1};
}
