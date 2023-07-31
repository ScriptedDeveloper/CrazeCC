#pragma once	
#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <memory>

namespace AST {
	class variable;
	class if_statement;
	class function;
	class function_call;
	using AnyAST = std::variant<variable, if_statement, function, function_call>;
	class variable {
		public:
			variable() {};
			inline bool defined_type() {
				return !type.empty();
			}
			inline bool defined_equal_symbol() {
				return __defined_equal_symbol;
			}
			inline bool defined_value() {
				return !value.empty();
			}
			inline bool defined_name() {
				return !name.empty();
			}
			inline void define_name(std::string_view name_param) {
				name = name_param.data();
			}
			inline void define_type(std::string_view type_param) {
				type = type_param.data();
			}	
			inline void define_value(std::string_view type_value) {
				value = type_value.data(); }
			inline void define_equal_symbol() {
				__defined_equal_symbol = true;
			}
			inline std::string_view get_name() {
				return name;
			}
			inline std::string_view get_type() {
				return type;
			}
			inline std::string_view get_value() {
				return value;
			}
		std::string instruction{};
		std::string memory_location{};
		static constexpr std::string instruction_type{"mov"};
		private:
			std::string name{};
			bool __defined_equal_symbol{false};
			std::string type{};
			std::string value{}; // not going to overcomplicate things here with std::variant...
							
	};	
	class if_statement {
		public:
			if_statement() {};
			inline bool defined_keyword() {
				return __defined_keyword;
			}
			inline bool defined_expression() {
				return __defined_expression;
			}
			inline bool defined_code_block() {
				return __defined_code_block;
			}
			inline void define_code_block() {
				__defined_code_block = true;
			}
		private:
			/*
			 * This is not DONE 
			 * TODO: Parse the expression correctly (variable, function return val etc)
			 */
			bool __defined_keyword{false};
			bool __defined_expression{false};
			bool __defined_code_block{false};
	};
	class function {
		public:
			function() {};
			virtual ~function() {};
			inline bool defined_keyword() {
				return __defined_keyword;
			}
			inline bool defined_type() {
				return !__type.empty();
			}
			inline void define_type(std::string_view type_param) {
				__type = type_param;
			}
			inline void define_name(std::string_view name_param) {
				__name = name_param;
			}
			inline void define_keyword() {
				__defined_keyword = true;
			}
			inline std::string_view get_type() {
				return __type;
			}
			inline bool defined_params() {
				return __defined_params;
			}
			inline bool defined_name() {
				return !__name.empty();
			}
			inline std::string_view get_name() {
				return __name;
			}
			inline uint8_t get_parenthesis_count() {
				return __parenthesis_count;
			}
			inline uint8_t get_curly_parenthesis_count() {
				return __curly_parenthesis_count;
			}	
			inline void increment_parenthesis_count() {
				__parenthesis_count++;
			}
			inline void increment_curly_parenthesis_count() {
				__curly_parenthesis_count++;
			}
			inline bool defined_code_block() {
				return __defined_code_block;
			}
			inline void define_code_block() {
				__defined_code_block = true;
			}	
			inline void define_params() {
				__defined_params = true;
			}
			
		std::vector<std::pair<std::string, std::string>> params{}; //first pair member is type, second is name
		std::vector<AnyAST> function_body{};
		private:
			bool __defined_keyword{false};
			bool __defined_params{false};
			bool __defined_code_block{false};
			uint8_t __parenthesis_count{};
			uint8_t __curly_parenthesis_count{};
			std::string __name{}, __type{};
	};

	class function_call {
		public:
			function_call() {};
			virtual ~function_call() {};

			inline void define_params() {
				__defined_params = true;
			}
			inline bool defined_params() {
				return __defined_params;
			}
			inline bool defined_function_name() {
				return __defined_function_name;
			}
			inline bool defined_semicolon() {
				return __defined_semicolon;
			}
			inline void define_semicolon() {
				__defined_semicolon = true;
			}
			inline void push_params(variable var) {
				params.push_back(AnyAST(var));
			}
			inline void define_function_name(std::shared_ptr<AST::AnyAST> func) {
				__function_name = func;
				__defined_function_name = true;
			}
			inline std::shared_ptr<AST::AnyAST> get_function() {
				return __function_name;
			}
			
		private:
			/*
			 * Right now, only pass by reference is supported
			 */
			std::vector<AST::AnyAST> params{};
			bool __defined_function_name{false};
			bool __defined_params{false};
			bool __defined_semicolon{false};
			std::shared_ptr<AST::AnyAST> __function_name{};

	};


};
