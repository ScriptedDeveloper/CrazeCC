#pragma once
#include <iostream>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../code_gen/code_gen.hpp"
#include "../exception/exception.hpp"
#include "../preprocessor/preprocessor.hpp"

template<bool is_debug>
class compiler {
	public:
		constexpr compiler(int argc = 0, char **argv = nullptr, std::string_view code = "") {
			__argc = argc;
			__argv = argv;
			__code = code;
		};
		virtual std::string run() {
				std::string file_name{}, output{};
				if(__argc != 1) {
					if(__argc == 1) {
						std::cerr << "No source files specified." << std::endl << "./" << __argv[0] << " <source_file>" << std::endl;
						std::exit(1);
					} else if(__argc <= 3 && std::string_view(__argv[1]) != "-d") {
						std::cerr << "Expected output file! Specifiy with -o" << std::endl;
						std::exit(1);
					}	
					file_name = __argv[1];
					output = __argv[3];
				} else {
					file_name = "asd";
					output = "erd";
				}
				lexer l{};
				if constexpr(is_debug) {
					l = lexer("", __code);
				} else {
					l = lexer(file_name, "");
				}
				l.parse();
				preprocessor prep(l.lexer_vec);
				prep.init();
				parser p(l.lexer_vec);
				code_generator c(p.ast_vec, output);
				int ret = c.init();
				if(ret != code_generator::GENERATE_SUCCESS)
					exception(ret, -1);
				if(is_debug)
					return code_generator::asm_content;
				return "";
			}

	private:
		int __argc{};
		char **__argv{};
		std::string_view __code{};
};
