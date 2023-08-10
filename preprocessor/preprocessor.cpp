#include "preprocessor.hpp"


int preprocessor::init() {
	for(auto i = lex_vec->begin(); i != lex_vec->end(); i++) {
		if(!i->is_hashtag() && !is_preprocessor_statement()) {
			/*
			 * This is not a preprocessor statement,
			 * so we simply skip this
			 */
			continue;
		}
		auto data = i->data();
		if(data == "#include" || is_include) {
			replace_include(i);
		}
	}

	return PREPROECSSING_SUCCESS;
}

int preprocessor::replace_include(lexer::LexVector::iterator &token) {
	is_include = true;
	auto data = token->data();

	if(data == "#include") {
		inc_obj.define_keyword();
		begin = token - lex_vec->begin();
	} else if(token->is_value()) {
		if(!std::filesystem::exists(data))
			return UNKNOWN_FILE;
		
		lexer lex(data);
		lex.parse();
		end = token - lex_vec->begin();
		lex_vec->insert(token + 1, lex.lexer_vec->begin(), lex.lexer_vec->end()) - 1;
		for(int i = begin; i <= end; i++) {
			lex_vec->erase(begin + lex_vec->begin());
		}
		is_include = false;
	} else {
		return INVALID_EXPRESSION;
	}
		
	return PREPROECSSING_SUCCESS;
}
