#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

/*
 * I tried using modules, sadly I encountered a couple clang bugs while linking with stdlib
 */

class lexer {
	public:
		lexer(std::string_view file) : __path(file.data()) {
			std::ifstream target_stream{__path};
			std::stringstream ss{};
			ss << target_stream.rdbuf();
			__contents = std::move(ss.str());
		};
		int parse() {
			std::string curr_token{};
			for(char c : __contents) {
			switch(c) {
				case ' ' : {
					lexer_vec.push_back(token(std::move(curr_token)));
				};
				case ';' : {
					lexer_vec.push_back(token(std::string(1, c)));
				};
				default : {
					curr_token += c;
					};
				};
			}
			return PARSING_SUCCESS;
		}
		class token {
			public:
				token(std::string curr_token) {
					__data = std::move(curr_token);
					if(__check_if_space())
						__is_space = true;
					else if(__data == ";")
						__is_semicolon = true;
					else if(__data.size() == 1 && std::find(operators.begin(), operators.end(), *__data.begin()) != operators.end())
						__is_operator = true;
					else if(std::find(keywords.begin(), keywords.end(), __data) != keywords.end())
						__is_keyword = true;
					else
						__is_value = true;
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
			inline bool is_keyword() {
				return __is_keyword;
			}
			inline bool is_operator() {
				return __is_operator;
			}
			inline bool is_value() {
				return __is_value;
			}
			private:
				std::string __data{};
				bool __is_space{false};
				bool __is_semicolon{false};
				bool __is_keyword{false};
				bool __is_operator{false};
				bool __is_value{false};
				bool __check_if_space() {
					for(char c : __data) {
						if(c != ' ') {
							return false;
						}
					}
					return true;
				}

		};
		static constexpr std::array<std::string, 3> keywords = {
			"int", "bool", "char"
		};	
		static constexpr std::array<char, 5> operators = {
			'+', '-', '/', '*', '%'
		};
		/* 
		 * l8ter add bitwise operators
		 */
		std::vector<token> lexer_vec{};
		static constexpr int PARSING_SUCCESS = 0;
	private:
		std::string __path{};
		std::string __contents{};
		
};

