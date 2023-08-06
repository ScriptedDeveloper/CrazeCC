#include "generate_ast.hpp"
#include "ast.hpp"

//std::unordered_map<std::string_view, std::vector<std::vector<AST::AnyAST>::iterator>> function::param_map{};

			
std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> generate_ast::variable::var_map{};

std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> generate_ast::function::function_map{};

ExpressionRet generate_ast::variable::check(lexer::token &token, 
	 bool &is_variable, int &potential_last_error, bool &complete) {

	is_variable = true;
	complete = false;
	if(token.is_space())
		return {SYNTAX_SUCCESS, -1}; // ignoring spaces
	if(!std::holds_alternative<AST::variable>(*last_expression->first) && !token.is_data_type())
		return {ERROR_EXPECTED_TYPE, line}; // compiler error...
	if(!last_expression->second && !token.is_data_type())
		return {ERROR_EXPECTED_TYPE, line};
	if(!last_expression->second)
		last_expression->first = std::make_shared<AST::AnyAST>(AST::variable());
	auto var_obj = std::get<AST::variable>(*last_expression->first);
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
				
	last_expression->second = true; // it exists!
	return (assign(token, last_expression, is_variable, line, potential_last_error, complete));
}

ExpressionRet generate_ast::variable::assign(lexer::token &token, auto &last_expression, bool &is_variable, 
	int line, int &potential_last_error, bool &complete) {

	auto var_obj = std::get<AST::variable>(*last_expression->first);

	if(token.is_data_type()) {
		var_obj.define_type_str(token.data());
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
		function::check_is_function_body();
		var_map[var_obj.get_name().data()] = last_expression->first;
		clear_expression();
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
	if(last_expression->first != nullptr)
		*last_expression->first = var_obj;
	return {SYNTAX_SUCCESS, -1};
}

void assign_variable_param() {
	/*
	 * Assigns the variable to its function param, if it is one
	 */

}


ExpressionRet generate_ast::function::check(lexer::token &token, 
bool &is_function, bool &complete) {	
	if(token.is_space())
		return {SYNTAX_SUCCESS, -1}; // ignoring spaces
	/*
	 * Error checking
	 */
	if(!last_expression->second && !token.is_keyword())
		return {ERROR_EXPECTED_FUNCTION_DECLARATION, line};
	if(!std::holds_alternative<AST::function>(*last_expression->first))
		*last_expression->first = AST::function();
	auto my_func = std::get<AST::function>(*last_expression->first);
	if(!my_func.defined_type() && token.is_brackets())
		return {ERROR_EXPECTED_FUNCTION_TYPE, line};
	if(!my_func.defined_name() && token.is_curly_brackets()) 
		return {ERROR_EXPECTED_FUNCTION_NAME, line};
	if(my_func.get_parenthesis_count() < 2 && token.is_curly_brackets())
		return {ERROR_EXPECTED_PARENTHESIS, line};

	last_expression->second = true;

	return assign(token, is_function, complete);
}
			

void generate_ast::function::check_is_function_body() {
	if(parenthesis_st.empty() || (!parenthesis_st.empty() 
		&& !std::holds_alternative<AST::function>(*parenthesis_st.top())))
		/*
		 * If we are inside a function, add it to the function body, otherwise, it's just a global variable
		 */
		ast_vector.push_back(last_expression->first);
	else {
		auto top = std::get<AST::function>(*parenthesis_st.top());
		top.function_body.push_back(*last_expression->first);
		*parenthesis_st.top() = top;
	}

}


ExpressionRet generate_ast::function::assign(lexer::token &token, 
bool &is_function, bool &complete) {	
	auto my_func = std::get<AST::function>(*last_expression->first);

	if(token.is_keyword())
		my_func.define_keyword();
	else if(token.is_data_type()) {
		if(my_func.defined_type()) {
			auto param_vec = &my_func.params;
			/*
			 * Okay, now we have to define the param type.
			 */
			param_vec->push_back({token.data(), ""});
		} else 
			my_func.define_type(token.data());
		
	}
	else if(token.is_value()) {
		if(my_func.defined_name()) {
			auto param_vec = &my_func.params;	
			if(param_vec->empty() || param_vec->rbegin()->first.empty())
				return {ERROR_EXPECTED_PARAM_VALUE, line};
			/*
			 * Now we have to define the param name of last param
			 */
			param_vec->rbegin()->second = token.data();
			/*
			 * Once the function is complete, we can assign it. Right now our object is incomplete
			 */
		} else {
			my_func.define_name(token.data());
		}
	} else if(token.is_brackets()) {
		if(my_func.get_parenthesis_count() >= 2)
			return {ERROR_UNEXPECTED_PARENTHESIS, line};
		my_func.increment_parenthesis_count();
		if(my_func.get_parenthesis_count() == 2)
			my_func.define_params();
	} else if(token.is_curly_brackets()) {
		if(my_func.get_curly_parenthesis_count() == 2)
			return {ERROR_UNEXPECTED_PARENTHESIS, line};
		my_func.increment_curly_parenthesis_count();
		parenthesis_st.push(last_expression->first);
		ast_vector.push_back(last_expression->first);
/*
		for(auto &i : param_map) {
			auto it = std::find(i.second.begin(), i.second.end(), ast_vector.end());
			if(it != i.second.end()) {
				*it = ast_vector.begin();
			}
		}	
		*/
			
		is_function = false;
		complete = true;
		last_expression->second = false;
		function_map[my_func.get_name().data()] = *ast_vector.rbegin();

	}
	if(last_expression->first != nullptr)
		*last_expression->first = my_func;
	return {SYNTAX_SUCCESS, -1};
	
}


ExpressionRet generate_ast::function_call::check(lexer::token &token, bool &is_function_call, int &line, std::shared_ptr<AST::AnyAST> &last_expr) {
	if(is_invisible_char(token.data()))
		return {SYNTAX_SUCCESS, -1}; // ignore invisible chars

	if(!std::holds_alternative<AST::function_call>(*last_expr))
		*last_expr = AST::function_call();
	
	auto func_call = std::get<AST::function_call>(*last_expr);	

	if(!func_call.defined_function_name() && function::function_map.find(token.data()) == function::function_map.end())
		return {syntax_validator::ERROR_UNKNOWN_FUNCTION_CALL, line};


	if(func_call.defined_params() && token.is_data_type())
		return {syntax_validator::ERROR_UNKNOWN_VARIABLE, line};

	if(token.is_data_type() && func_call.defined_function_name() && variable::var_map.find(token.data()) == variable::var_map.end())
		return {syntax_validator::ERROR_UNKNOWN_VARIABLE, line};

	if(token.is_data_type() && func_call.defined_params())
		return {syntax_validator::ERROR_UNKNOWN_VARIABLE, line};

	if(token.is_semicolon() && !func_call.defined_params())
		return {syntax_validator::ERROR_UNEXPECTED_SEMICOLON, line};

	if(token.is_curly_brackets())
		return {syntax_validator::ERROR_UNEXPECTED_PARENTHESIS, line};

	if(token.is_keyword())
		return {syntax_validator::ERROR_UNEXPECTED_KEYWORD, line};
	
	last_expression->second = true; // it exists!

	auto ret = assign(func_call, is_function_call, token);
	if(last_expr != nullptr)
		*last_expr = func_call;
	return ret;
}
			

ExpressionRet generate_ast::function_call::assign(AST::function_call &last_expr, bool &is_function_call, lexer::token &t) {
	if(t.is_value()) {
		if(!last_expr.defined_function_name()) {
			last_expr.define_function_name(function::function_map.find(t.data())->second);
		} else {
			/*
			 * We're gonna assume its a parameter
			 */
			last_expr.push_params(generate_ast::variable::var_map[t.data()]);
		}
	} else if(t.is_brackets()) {
		last_expr.increment_parenthesis_count();
	}

	if(last_expr.get_parenthesis_count() == 2) {
		is_function_call = false;
		complete = true;
		*last_expression->first = AST::AnyAST(last_expr);
		function::check_is_function_body();
	}
	
	return {SYNTAX_SUCCESS, -1};
}
