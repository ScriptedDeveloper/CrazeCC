#include "exception.hpp"


std::string exception::GetLastError(int error) {
	/*
	 * * Kinda similiar to WINAPI, gets the error code in form of basic_string
	 */
	switch(error) {
		case syntax_validator::SYNTAX_SUCCESS: {
			return "SYNTAX_SUCCESS";
		};
		case syntax_validator::ERROR_EXPECTED_VALUE : {
			return "ERROR_EXPECTED_VALUE";
		};
		case syntax_validator::ERROR_UNKNOWN_VARIABLE : {
			return "ERROR_UNKNOWN_VARIABLE";
		};
		case syntax_validator::ERROR_EXPECTED_TYPE : {
			return "ERROR_EXPECTED_TYPE";
		};
		case syntax_validator::ERROR_EXPECTED_EXPRESSION : {
			return "ERROR_EXPECTED_EXPRESSION";
		};
		case syntax_validator::ERROR_EXPECTED_SEMICOLON : {
			return "ERROR_EXPECTED_SEMICOLON";
		};
		case syntax_validator::ERROR_EXPECTED_EQUAL_SYMBOL : {
			return "ERROR_EXPECTED_EQUAL_SYMBOL";
		};	
		case syntax_validator::ERROR_EXPECTED_FUNCTION_DECLARATION : {
			return "ERROR_EXPECTED_FUNCTION_DECLARATION";
		};	
		case syntax_validator::ERROR_EXPECTED_FUNCTION_TYPE : {
			return "ERROR_EXPECTED_FUNCTION_TYPE";
		};	
		case syntax_validator::ERROR_EXPECTED_PARAM_VALUE : {
			return "ERROR_EXPECTED_PARAM_VALUE";
		};	
		case syntax_validator::ERROR_EXPECTED_PARENTHESIS : {
			return "ERROR_EXPECTED_PARENTHESIS";
		};	
		case syntax_validator::ERROR_UNEXPECTED_PARENTHESIS : {
			return "ERROR_UNEXPECTED_PARENTHESIS";
		};
		case syntax_validator::SYNTAX_FUNCTION_FINISHED : {
			return "SYNTAX_FUNCTION_FINISHED";
		};
		case syntax_validator::ERROR_INVALID_KEYWORD : {
			return "ERROR_INVALID_KEYWORD";
		};
		case code_generator::GENERATE_UNKNOWN_VARIABLE : {
			return "GENERATE_UNKNOWN_VARIABLE";
		}	
		case code_generator::GENERATE_UNKNOWN_VARIABLE_VALUE : {
			return "GENERATE_UNKNOWN_VARIABLE_VALUE";
		}
		case code_generator::GENERATE_SUCCESS : {
			return "GENERATE_SUCCESS";
		};
		default : 
			return "ERROR_GET_ERROR_CODE";
	};
}
