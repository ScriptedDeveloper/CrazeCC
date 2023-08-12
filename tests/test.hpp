#pragma once
#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../compiler/compiler.hpp"


class compiler_mock : public compiler<true> {
	public:
		MOCK_METHOD(std::string, run, (), (override));
};

TEST(T1, create_empty_main) {
	compiler_mock m;


}
