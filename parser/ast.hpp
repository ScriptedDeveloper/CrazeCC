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
	class return_ast;
	using AnyAST = std::variant<variable, if_statement, function, function_call, return_ast>;

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
			inline void push_params(std::shared_ptr<AnyAST> var) {
				params.push_back(var);
			}
			inline void define_function_name(std::shared_ptr<AST::AnyAST> func) {
				__function_name = func;
				__defined_function_name = true;
			}
			inline std::shared_ptr<AST::AnyAST> get_function() {
				return __function_name;
			}
			inline void increment_parenthesis_count() {
				parenthesis_count++;
			}
			inline uint8_t get_parenthesis_count() {
				return parenthesis_count;
			}
			std::vector<std::shared_ptr<AnyAST>> params{};
			
		private:
			/*
			 * Right now, only pass by reference is supported
			 */
			bool __defined_function_name{false};
			bool __defined_params{false};
			bool __defined_semicolon{false};
			uint8_t parenthesis_count{};
			std::shared_ptr<AST::AnyAST> __function_name{};

	};	

	class variable {
		public:
			variable() {};
			inline bool defined_type() {
				return type != 0;
			}
			inline bool defined_equal_symbol() {
				return __defined_equal_symbol;
			}
			inline bool defined_value() {
				return !value.empty() || call_function;
			}
			inline bool defined_name() {
				return !name.empty();
			}
			inline void define_name(std::string_view name_param) {
				name = name_param.data();
			}
			inline void define_type(const uint8_t type_param) {
				type = type_param;
				switch(type) {
					case TYPE_INT : {
						type_str = "int";
						break;
					};
					case TYPE_CHAR : {
						type_str = "char";
						break;
					};	
					case TYPE_VOID : {
						type_str = "void";
						break;
					};	
					case TYPE_BOOL : {
						type_str = "bool";
						break;
					};
				};
			}	
			inline void define_type_str(std::string_view type_param) {
				if(type_param == "int")
					define_type(TYPE_INT);
				else if(type_param == "bool")
					define_type(TYPE_BOOL);
				else if(type_param == "char")
					define_type(TYPE_CHAR);
				else
					define_type(TYPE_VOID);
			}
			inline void define_value(std::variant<std::string_view, function_call> type_value) {
				if(std::holds_alternative<function_call>(type_value)) {
					call_function = true;
					call = std::get<function_call>(type_value);
				} else {
					value = std::get<std::string_view>(type_value).data(); 
				}
			}
			inline void define_equal_symbol() {
				__defined_equal_symbol = true;
			}
			inline std::string_view get_name() {
				return name;
			}
			inline uint8_t get_type() {
				return type;
			}
			inline std::string_view get_type_str() {
				return type_str;
			}
			inline std::variant<std::string, AST::function_call> get_value() {
				if(call_function)
					return call;
				return value;
			}
		std::string instruction{};
		static constexpr std::string instruction_type{"mov"};

		static constexpr uint8_t TYPE_INT = 1;
		static constexpr uint8_t TYPE_CHAR = 2;
		static constexpr uint8_t TYPE_VOID = 3;
		static constexpr uint8_t TYPE_BOOL = 4;
		private:
			std::string name{};
			bool __defined_equal_symbol{false};
			uint8_t type{};
			std::string type_str{};
			std::string value{}; // not going to overcomplicate things here with std::variant...
			
			bool call_function{false};
			function_call call;
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
			inline void define_keyword() {
				__defined_keyword = true;
			}
			inline void increment_parenthesis_count() {
				__parenthesis_count++;
			}
			inline uint8_t get_parenthesis_count() {
				return __parenthesis_count;
			}	
			inline void increment__parenthesis_count() {
				__parenthesis_count++;
			}
			inline uint8_t get_curly_parenthesis_count() {
				return __curly_parenthesis_count;
			}	
			inline uint8_t increment_curly_parenthesis_count() {
				return __curly_parenthesis_count++;
			}
			std::pair<std::string, std::string> compare_pair{}; // will change this later.. will try to get some basic logic working first

			bool __is_compare{false};
			std::vector<AST::AnyAST> function_body{}; 
		private:
			/*
			 * This is not DONE 
			 * TODO: Parse the expression correctly (variable, function return val etc)
			 */
			bool __defined_keyword{false};
			bool __defined_expression{false};
			bool __defined_code_block{false};
			uint8_t __parenthesis_count{};
			uint8_t __curly_parenthesis_count{};
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


	class return_ast {
		public:
			return_ast() {};
			virtual ~return_ast() {};
			
			inline void define_value(std::string_view data) {
				value = data;
			}
			inline void define_keyword() {
				set_keyword = true;
			}
			inline std::string get_value() {
				return value;
			}
			inline bool defined_keyword() {
				return set_keyword;
			}
			inline bool defined_value() {
				return !value.empty();
			}

		private:
			std::string value{};
			bool set_keyword{false};
	};

	class preprocessor_definitions {
		public:
			preprocessor_definitions() {};
			virtual ~preprocessor_definitions() {};

			inline void set_exclamation_mark() {
				__set_exclamation_mark = true;
			}
			inline void set_expressions() {
				__set_expressions = true;
			}
			inline void set_keyword() {
				__set_keyword = true;
			}
		private:
			bool __set_exclamation_mark{false};
			bool __set_keyword{false};
			bool __set_expressions{false};
	};

};

