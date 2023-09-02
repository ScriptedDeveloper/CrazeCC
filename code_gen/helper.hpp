#pragma once
#include <iostream>
#include <array>

namespace Helper {
	/*
	 * This namespace provides a simple API to generate assembly
	 */
	
	inline std::string move_to_register(std::array<std::pair<const std::string, bool>, 8>::iterator reg, std::string_view data, bool occupy = false) {
		if(occupy)
			reg->second = true;

		return "mov " + reg->first + + ", " + data.data() + "\n";
	}

	inline std::string stack_setup(std::string_view func_name) {
		return std::string(func_name.data() + std::string(":\n") + "push rbp\nmov rbp, rsp\n");
	}

	inline std::string subtract_bytes(std::string rsp_bytes) {
		return "sub rsp, " + rsp_bytes + "\n";
	}

	inline std::string compare(std::string_view val1, std::string_view val2) {
		return std::string("cmp ") + val1.data() + ", " + val2.data() + "\n";
	}
		
	inline std::string restore_stack_ptr(const std::string_view &bytes) {
		return std::string("add rsp, ") + bytes.data() + "\n";
	}
};
