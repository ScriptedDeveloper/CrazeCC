#include "generate_ast.hpp"

std::pair<int, int> generate_ast::variable::check_variable(lexer::token &token, 
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

std::pair<int, int> generate_ast::variable::assign_variable(lexer::token &token, auto &last_expression, bool &is_variable, 
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
		}
	}
	else
		return {ERROR_EXPECTED_VALUE, line};
	last_expression->first = var_obj;
	return {SYNTAX_SUCCESS, -1};
}

