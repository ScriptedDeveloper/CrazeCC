#pragma once	
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace AST {
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
				value = type_value.data();
			}
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
		static constexpr std::string instruction = "mov";
		std::string instruction_type{};
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
			inline uint8_t get_curly_parenthesis_count() {
				return __curly_parenthesis_count;
			}
			inline uint8_t get_parenthesis_count() {
				return __parenthesis_count;
			}
		std::vector<std::pair<std::string, std::string>> params{}; //first pair member is type, second is name
		uint8_t __parenthesis_count{};
		uint8_t __curly_parenthesis_count{};
		private:
			bool __defined_keyword{false};
			bool __defined_params{false};
			std::string __name{}, __type{};
	};

	using AnyAST = std::variant<variable, if_statement, function>;

};
