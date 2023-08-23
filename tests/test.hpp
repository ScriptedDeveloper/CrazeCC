#pragma once
#include "gtest/gtest.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../compiler/compiler.hpp"

/*
 * Not needed rn
 */
class compiler_mock : compiler<true> {
	public:
		compiler_mock(int argc = 0, char **argv = nullptr, 
				std::string_view code = "") : compiler<true>(argc, argv, code) {};

		MOCK_METHOD(std::string, run, (), (override));
};

inline std::string get_file_content(std::string_view name) {
	std::ifstream ifs(name.data());
	std::stringstream ss{};
	ss << ifs.rdbuf();
	return ss.str();
}

TEST(T1, create_empty_main) {
		std::set_terminate([]() {
			throw std::exception();
		});
	  compiler<true> m(0, nullptr, get_file_content("cases/main.cl"));
	  ASSERT_EXIT(m.run(), ::testing::KilledBySignal(0), ".*");
	  ASSERT_EXIT(m.run(), ::testing::KilledBySignal(SIGSEGV), ".*");
}
