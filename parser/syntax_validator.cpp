#include "syntax_validator.hpp"
#include "ast.hpp"
#include "generate_ast.hpp"
/*
 * Static variable declarations
 */

std::vector<std::shared_ptr<AST::AnyAST>> syntax_validator::ast_vector{};
std::shared_ptr<lexer::LexVector> syntax_validator::__lex_vec{};
int syntax_validator::line{1};
int syntax_validator::potential_last_error{SYNTAX_SUCCESS};
bool syntax_validator::complete{false};
std::unique_ptr<syntax_validator::expression_variant> 
syntax_validator::last_expression = std::make_unique<expression_variant>();
std::stack<std::shared_ptr<AST::AnyAST>> syntax_validator::parenthesis_st{};

std::pair<int ,std::variant<int, std::vector<std::shared_ptr<AST::AnyAST>>>> syntax_validator::check_syntax_tokens() {	
	/*
	 * the int in the pair tells us whether the expression is existing or not.
	 */

	for(auto token : *__lex_vec) {	
		line = token.get_line();
		auto ret = check_keyword_tokens(token);
		if(ret.first != SYNTAX_SUCCESS && ret.first != ERROR_INVALID_KEYWORD)
			return ret;
		else if(ret.first != ERROR_INVALID_KEYWORD)
			continue;
		else if(complete && potential_last_error != SYNTAX_SUCCESS)
			return {potential_last_error, line}; // something went wrong in the past line
		if(is_variable || (token.is_data_type() && !last_expression->second)) {
			generate_ast::variable v(__lex_vec, &is_variable);
			auto variable_ret = v.check(token);
			if(variable_ret.first != SYNTAX_SUCCESS)
				return variable_ret;

		} else if(is_if || (token.is_keyword() && token.data() == lexer::keywords.begin()->first)) {
			/*

				*/
		} else if(token.is_curly_brackets()) {
			auto ret = check_curly_brackets(token, last_expression->first);
			clear_expression(); // we no longer need the other expression
			if(ret.first != SYNTAX_SUCCESS)
				return ret;
		}
		else if(token.is_value() || is_function_call) {
			generate_ast::function_call f_call(__lex_vec, &is_function_call);
			auto call_ret = f_call.check(token, last_expression->first);
			if(call_ret.first != SYNTAX_SUCCESS)
				return call_ret;
		}
	}
	preprocessor p(__lex_vec);
	if(!complete)
		return {potential_last_error, line};
	return {SYNTAX_SUCCESS, std::move(ast_vector)};
}


ExpressionRet syntax_validator::check_keyword_tokens(lexer::token &token) { 
	if(token.is_space())
		return {SYNTAX_SUCCESS, -1};


	auto it = std::find_if(lexer::keywords.begin(), lexer::keywords.end(), [&](const auto &pair) {
		return pair.first == token.data();
	});
	int keyword = (it == lexer::keywords.end()) ? -1 : it->second;

	if(potential_last_error != SYNTAX_SUCCESS && keyword != -1)
		return {potential_last_error, line};

	bool has_computed{true};
	if(keyword == lexer::RETURN_KEYWORD || is_return) {
		generate_ast::return_gen ret_obj(__lex_vec, &is_return);
		auto ret = ret_obj.check(token, last_expression->first);
		if(ret.first != SYNTAX_SUCCESS)
			return ret;

	} else if(is_function || keyword == lexer::DEF_KEYWORD) {
		generate_ast::function f(__lex_vec, &is_function);
		auto func_ret = f.check(token); // need check last error
		if(func_ret.first != SYNTAX_SUCCESS)
			return func_ret;
	} else if(is_if || keyword == lexer::IF_KEYWORD) {
		generate_ast::if_statement if_obj(__lex_vec, &is_if);
		auto if_ret = if_obj.check(token);
			if(if_ret.first != SYNTAX_SUCCESS)
				return if_ret;
	} else
		has_computed = false;
	
	if(it == lexer::keywords.end() && !has_computed) {

		return {ERROR_INVALID_KEYWORD, line};
	}

	return {SYNTAX_SUCCESS, -1};
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
