#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <array>
#include <algorithm>

/* I tried using modules, sadly I encountered a couple clang bugs while linking with stdlib
 */

class lexer {
	public:
		lexer(std::string_view file = "", std::string_view content = "") : __path(file.data()) {
			if (content.empty()) {
				std::ifstream target_stream{__path};
				std::stringstream ss{};
				ss << target_stream.rdbuf();
				__contents = ss.str();
			} else {
				__contents = content.data();
			}
		};
		int parse() {
			std::string curr_token{};
			int curr_line{};
			for(char c : __contents) {
			switch(c) {
				case ' ' : {
					if(!curr_token.empty())
						lexer_vec->push_back(token(std::move(curr_token), curr_line));
					else
						lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				};
				case ';' : {
					if(!lexer_vec->empty())
						lexer_vec->push_back(token(std::move(curr_token), curr_line));
					lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				};
				case '\n' : {
					curr_line++;
					continue;
				};
				case '{' : {
					push_remaining(curr_token, curr_line);
					lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				};
				case '}' : {
					push_remaining(curr_token, curr_line);
					lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				}
				case '(' : {
					push_remaining(curr_token, curr_line);
					lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				};	
				case ')' : {
					push_remaining(curr_token, curr_line);
					lexer_vec->push_back(token(std::string(1, c), curr_line));
					break;
				};
				case '\t' : {
					continue;
				}
				default : {
					curr_token += c;
					break;
				};
			};
			}
			if(lexer_vec->empty() || lexer_vec->rbegin()->data() != curr_token)
				lexer_vec->push_back(token(curr_token, curr_line));
			return LEXER_SUCCESS;
		}
		class token {
			public:
				token(std::string curr_token, int curr_line_param) {
					curr_line = curr_line_param;
					__data = std::move(curr_token);
					if(__check_if_space())
						__is_space = true;
					else if(__data == ";")
						__is_semicolon = true;
					else if(__data.size() == 1 && std::find(operators.begin(), 
						operators.end(), *__data.begin()) != operators.end())
						__is_operator = true;
					else if(std::find_if(keywords.begin(), keywords.end(), [&](const auto &pair) {
						return pair.first == __data;
								}) != keywords.end())
						__is_keyword = true;
					else if(*__data.begin() == '#')
						__is_hashtag = true;
					else if(std::find(data_types.begin(), data_types.end(), __data) != data_types.end())
						__is_data_type = true;
					else {
						if(std::find(brackets.begin(), brackets.end(), __data[0]) == brackets.end())
							__is_value = true;
					}
				};

			inline std::string data() {
				return __data;
			}
			inline bool is_space() {
				return __is_space;
			}
			inline bool is_semicolon() {
				return __is_semicolon;
			}
			inline bool is_keyword() { return __is_keyword;
			}
			inline bool is_operator() {
				return __is_operator;
			}
			inline bool is_value() {
				return __is_value;
			}
			inline bool is_parenthesis() {
				return __is_parenthesis;
			}
			inline bool is_data_type() {
				return __is_data_type;
			}
			inline bool is_brackets() {
				return __data == "(" || __data == ")";
			}
			inline bool is_curly_brackets() {
				return __data == "{" || __data == "}";
			}
			inline bool is_hashtag() {
				return __is_hashtag;
			}
			inline int get_line() {
				return curr_line;
			}
			private:
				std::string __data{};
				bool __is_space{false};
				bool __is_hashtag{false};
				bool __is_semicolon{false};
				bool __is_keyword{false};
				bool __is_data_type{false};
				bool __is_operator{false};
				bool __is_value{false};
				bool __is_parenthesis{false};
				int curr_line{};
				bool __check_if_space() {
					for(char c : __data) {
						if(c != ' ') {
							return false;
						}
					}
					return true;
				}
				static constexpr std::array<char, 5> brackets = {'(', ')', '{', '}'};

		};
		static constexpr int IF_KEYWORD{1};
		static constexpr int ELIF_KEYWORD{2};
		static constexpr int ELSE_KEYWORD{3};
		static constexpr int DEF_KEYWORD{4};
		static constexpr int RETURN_KEYWORD{5};
		static constexpr std::array<std::pair<std::string, int>, 5> keywords = {
			std::make_pair("if", IF_KEYWORD), {"elif", ELIF_KEYWORD}, 
			{"else", ELSE_KEYWORD}, {"def", DEF_KEYWORD},
			{"return", RETURN_KEYWORD}
		};
		static constexpr std::array<std::string, 4> data_types = {
			"int", "bool", "char", "void"
		};	
		static constexpr std::array<char, 6> operators = {
			'+', '-', '/', '*', '%', '='
		};

		static constexpr std::array<std::pair<std::string, int>, data_types.size()> data_sizes = {
			std::make_pair("int", 8), {"bool", 1}, {"char", 1}, {"void", 1}
		};
		using LexVector = std::vector<token>;
		/* 
		 * l8ter add bitwise operators
		 */
		std::shared_ptr<LexVector> lexer_vec{std::make_shared<LexVector>()};
		static constexpr int LEXER_SUCCESS = 0;
	private:
		std::string __path{};
		std::string __contents{};

		inline void push_remaining(std::string &curr_token, int curr_line) {
			if(!curr_token.empty())
				lexer_vec->push_back(token(std::move(curr_token), curr_line));
		}
		
};
