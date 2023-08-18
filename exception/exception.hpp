#pragma once
#include <iostream>


class exception {
	public: 
		exception(int error, int line) {
			std::cerr << "Syntax error! " << GetLastError(error) << " at line : " 
					<< line << std::endl;
			std::exit(1);

		};
	private:
		std::string GetLastError(int error);
};

#include "../parser/syntax_validator.hpp"
#include "../code_gen/code_gen.hpp"
