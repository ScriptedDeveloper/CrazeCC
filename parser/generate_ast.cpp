#include "generate_ast.hpp"
#include "ast.hpp"

std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> generate_ast::variable::var_map{};

std::unordered_map<std::string, std::shared_ptr<AST::AnyAST>> generate_ast::function::function_map{};

ExpressionRet generate_ast::variable::check(lexer::token &token) {

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
	return (assign(token, last_expression));
}

ExpressionRet generate_ast::variable::assign(lexer::token &token, auto &last_expression) {

	AST::variable var_obj = std::get<AST::variable>(*last_expression->first);

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
		*__is_variable = false;
		complete = true;	
		function::check_is_function_body();
		var_map[var_obj.get_name().data()] = last_expression->first;
		clear_expression(); // token is a semicolon, variable declaration has ended
		potential_last_error = SYNTAX_SUCCESS;
	}
	else if(token.is_value() || token.is_brackets()) {
		if(var_obj.defined_equal_symbol()) {
			auto it = function::function_map.find(token.data());
			if(it == function::function_map.end() && !token.is_brackets()) {
				var_obj.define_value(token.data());
				potential_last_error = ERROR_EXPECTED_SEMICOLON;
			} else {
				if(std::holds_alternative<std::string>(var_obj.get_value()))
					var_obj.define_value(AST::function_call());
				auto obj = std::get<AST::function_call>(var_obj.get_value());
				auto ptr = std::make_shared<AST::AnyAST>(AST::AnyAST(obj));
				generate_ast::function_call f_(lex_vec, &is_function_call, true);
				f_.check(token, ptr);
				complete = false;
				var_obj.define_value(std::get<AST::function_call>(*ptr));
			}
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


ExpressionRet generate_ast::function::check(lexer::token &token) {	
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
	if(token.is_curly_brackets() && my_func.get_curly_parenthesis_count() == 2)
			return {ERROR_UNEXPECTED_PARENTHESIS, line};
	if(((token.is_value() && my_func.defined_name())) && (my_func.params.empty() 
			|| my_func.params.rbegin()->first.empty() ))
		return {ERROR_EXPECTED_PARAM_VALUE, line};


	last_expression->second = true;

	return assign(token);
}
			

void generate_ast::function::check_is_function_body(bool is_block_expression) {
	if(parenthesis_st.empty() || ((!parenthesis_st.empty() 
		&& !std::holds_alternative<AST::function>(*parenthesis_st.top())) &&
		!std::holds_alternative<AST::if_statement>(*parenthesis_st.top())))
		/*
		 * If we are inside a function, add it to the function body, otherwise, it's just a global variable
		 */
		ast_vector.push_back(last_expression->first);
	else {
		if(std::holds_alternative<AST::function>(*parenthesis_st.top())) {
			auto top = std::get<AST::function>(*parenthesis_st.top());
			top.function_body.push_back(*last_expression->first);

			if(is_block_expression)
				parenthesis_st.push(last_expression->first);
			else
				*parenthesis_st.top() = top;
		} else {
			auto top = std::get<AST::if_statement>(*parenthesis_st.top());
			top.function_body.push_back(*last_expression->first);
			*parenthesis_st.top() = top;
		}
	}

}


ExpressionRet generate_ast::function::assign(lexer::token &token) {	
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
			potential_last_error = ERROR_EXPECTED_PARAM_VALUE;
		} else {  
			my_func.define_type(token.data());
			potential_last_error = ERROR_EXPECTED_FUNCTION_NAME;
		}
		
	}
	else if(token.is_value()) {
		if(my_func.defined_name()) {
			auto param_vec = &my_func.params;	
			/*
			 * Now we have to define the param name of last param
			 */
			param_vec->rbegin()->second = token.data();
			/*
			 * Once the function is complete, we can assign it. Right now our object is incomplete
			 */
			potential_last_error = ERROR_EXPECTED_PARENTHESIS;
		} else {
			my_func.define_name(token.data());
			potential_last_error = ERROR_EXPECTED_PARENTHESIS;
		}
	} else if(token.is_brackets()) {
		my_func.increment_parenthesis_count();
		if(my_func.get_parenthesis_count() == 2)
			my_func.define_params();

		potential_last_error = ERROR_EXPECTED_PARENTHESIS;
	} else if(token.is_curly_brackets()) {
	
		my_func.increment_curly_parenthesis_count();
		parenthesis_st.push(last_expression->first);
		ast_vector.push_back(last_expression->first);
		potential_last_error = SYNTAX_SUCCESS;

		*is_function_ptr = false;
		complete = true;
		last_expression->second = false;
		function_map[my_func.get_name().data()] = *ast_vector.rbegin();
		clear_expression();

	}
	if(last_expression->first != nullptr)
		*last_expression->first = my_func;
	return {SYNTAX_SUCCESS, -1};
	
}


ExpressionRet generate_ast::function_call::check(lexer::token &token, std::shared_ptr<AST::AnyAST> &last_expr) {
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

	auto ret = assign(func_call, token);
	if(last_expression->first)
		*last_expr = func_call;
	return ret;
}
			

ExpressionRet generate_ast::function_call::assign(AST::function_call &last_expr, lexer::token &t) {
	if(t.is_value()) {
		if(!last_expr.defined_function_name()) {
			last_expr.define_function_name(function::function_map.find(t.data())->second);
			potential_last_error = ERROR_EXPECTED_PARENTHESIS;
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
		last_expr.define_params();
		potential_last_error = ERROR_EXPECTED_SEMICOLON;
	
	}
	if(t.is_semicolon()) {
		*__is_function_call = false;
		complete = true;	
		function::check_is_function_body();
		clear_expression();
		last_expr.define_semicolon();
		potential_last_error = SYNTAX_SUCCESS;	
	}
	if(!__is_child) {
		*last_expression->first = AST::AnyAST(last_expr);
	}

	return {SYNTAX_SUCCESS, -1};
}
			

ExpressionRet generate_ast::return_gen::check(lexer::token &token, std::shared_ptr<AST::AnyAST> &last_expr) {
	if(token.is_keyword() && std::holds_alternative<AST::return_ast>(*last_expr)) {
		return {ERROR_UNEXPECTED_KEYWORD, line};
	}

	if(!std::holds_alternative<AST::return_ast>(*last_expr))
		*last_expr = AST::return_ast();
	
	auto ret_obj = std::get<AST::return_ast>(*last_expr);

	if((token.is_semicolon() && !ret_obj.defined_value()) || (token.is_semicolon() && !ret_obj.defined_keyword()))
		/*
		 * We have a syntax error, unknown semicolon
		 */
		return {ERROR_UNEXPECTED_SEMICOLON, line};

	if(token.is_value() && !ret_obj.defined_keyword())
		return {ERROR_UNEXPECTED_KEYWORD, line};
	
	return assign(last_expr, token);

}
			
ExpressionRet generate_ast::return_gen::assign(std::shared_ptr<AST::AnyAST> &ret_ptr, lexer::token &t) {
	auto ret_obj = std::get<AST::return_ast>(*ret_ptr);
	if(t.is_keyword()) {
		ret_obj.define_keyword();
		potential_last_error = ERROR_EXPECTED_VALUE;
	} else if(t.is_value()) {
		/*
		 * Right now only supporting integer variables as return type
		 * will change in future
		 */
		ret_obj.define_value(t.data());
	}
	if(t.is_semicolon()) {
		*__is_return = false;
		function::check_is_function_body();
		potential_last_error = SYNTAX_SUCCESS;
	}

	*ret_ptr = ret_obj;
	return {SYNTAX_SUCCESS, -1};
}


ExpressionRet generate_ast::if_statement::check(lexer::token &t) {
	if(!std::holds_alternative<AST::if_statement>(*last_expression->first))
		*last_expression->first = AST::if_statement();

	if(t.is_semicolon())
		return {ERROR_UNEXPECTED_SEMICOLON, line};
	
	if(t.is_data_type())
		return {ERROR_UNEXPECTED_DATA_TYPE, line};

	AST::if_statement obj = std::get<AST::if_statement>(*last_expression->first);

	if(obj.defined_keyword() && t.is_keyword())
		return {ERROR_UNEXPECTED_KEYWORD, line};
	
	if(obj.defined_code_block() && !t.is_curly_brackets())
		return {ERROR_EXPECTED_PARENTHESIS, line};

	if(obj.defined_expression() && !t.is_curly_brackets())
		return {ERROR_EXPECTED_PARENTHESIS, line};

	return assign(t);
}

ExpressionRet generate_ast::if_statement::assign(lexer::token &token) {
	AST::if_statement obj = std::get<AST::if_statement>(*last_expression->first);

	if(token.is_keyword())
		obj.define_keyword();
	else if(token.is_brackets()) {
		obj.increment_parenthesis_count();
	} else if(token.is_curly_brackets()) {
		obj.increment_curly_parenthesis_count();
		if(obj.get_curly_parenthesis_count() == 1) {
			*__is_if_statement = false;
			function::check_is_function_body(true);
			clear_expression();
		} else 
			parenthesis_st.push(last_expression->first);

		return {SYNTAX_SUCCESS, -1};
	} else if(token.is_operator()) {
		if(token.data() == "==") {
			obj.__is_compare = true;
		}
	} else {
		if(obj.compare_pair.first.empty())
			obj.compare_pair.first = token.data();
		else
			obj.compare_pair.second = token.data();
	}
	*last_expression->first = obj;
	return {SYNTAX_SUCCESS, line};
}
