#include "syntax_validator.hpp"
#include "generate_ast.hpp"

/*
 * Static variable declarations
 */

std::vector<AST::AnyAST> syntax_validator::ast_vector{};
std::shared_ptr<lexer::LexVector> syntax_validator::__lex_vec{};
int syntax_validator::line{1};
std::unique_ptr<syntax_validator::expression_variant> 
syntax_validator::last_expression = std::make_unique<expression_variant>();
std::unordered_map<std::string_view, std::shared_ptr<AST::AnyAST>> syntax_validator::var_map;

std::pair<int ,std::variant<int, std::vector<AST::AnyAST>>> syntax_validator::check_syntax_tokens() {	
	/*
	 * the bool in the pair tells us whether the expression is existing or not.
	 */
	bool is_variable{false}, complete{false};
	bool is_function{};
	bool is_if{false};
	int potential_last_error{SYNTAX_SUCCESS}; // in case we go to the next expression, we can look back in case something's missing
	for(auto token : *__lex_vec) {
		if(complete && potential_last_error != SYNTAX_SUCCESS)
			return {potential_last_error, line}; // something went wrong in the past line
		if(is_variable || (token.is_data_type() && !last_expression->second)) {
			generate_ast::variable v(__lex_vec);
			auto variable_ret = v.check_variable(token, is_variable, potential_last_error, complete);
			if(variable_ret.first != SYNTAX_SUCCESS)
				return variable_ret;
			if(!is_variable) { // token is a semicolon, variable declaration has ended
				ast_vector.push_back(last_expression->first);
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
			generate_ast::function f(__lex_vec);
			auto func_ret = f.check_function(token, is_function, complete); // need check last error
			if(func_ret.first != SYNTAX_SUCCESS)
				return func_ret;
			if(!is_function)
				ast_vector.push_back(last_expression->first);

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
inline void syntax_validator::clear_expression() {
	last_expression = std::make_unique<expression_variant>();
}
