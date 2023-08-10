#pragma once
#include <iostream>
#include <string_view>
#include <filesystem>
#include "../lexer/lexer.hpp"
#include "ast.hpp"

class preprocessor {
	public:
		preprocessor(std::shared_ptr<std::vector<lexer::token>> &lex_vec_p) {
			lex_vec = lex_vec_p;
		}
		virtual ~preprocessor() {
			
		}
		int init();
		static constexpr int PREPROECSSING_SUCCESS = 0;
		static constexpr int INVALID_EXPRESSION = -1;
		static constexpr int UNKNOWN_FILE = -2;
	private:
		std::shared_ptr<std::vector<lexer::token>> lex_vec{};
		int replace_include(lexer::LexVector::iterator &token);
		bool is_include{false};
		AST_preprocessor::include inc_obj{};

		inline bool is_preprocessor_statement() {
			return is_include;
		}

		/*
		 * For erasing the preprocessor tokens
		 */
		int begin{};
		int end{};
};
