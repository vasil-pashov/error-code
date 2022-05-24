#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "error_code.h"
#include <cstring>

TEST_CASE("Default constructor is success") {
	EC::ErrorCode err;
	CHECK(err.hasError() == false);
}

TEST_CASE("Constructor with 0 is success") {
	EC::ErrorCode err(0, "No error");
	CHECK(err.hasError() == false);
}

TEST_CASE("Constructor with string is failure") {
	EC::ErrorCode err("Error!");
	CHECK(err.hasError() == true);
	CHECK(strcmp(err.getMessage(), "Error!") == 0);
}

TEST_CASE("Constructor with non-zero code is failure") {
	EC::ErrorCode err(5, "Error!");
	CHECK(err.hasError() == true);
	CHECK(strcmp(err.getMessage(), "Error!") == 0);
	CHECK(err.getStatus() == 5);
}

TEST_CASE("String formatting works") {
	EC::ErrorCode err("%d %.2f %s", 1, 2.25, "a string");
	CHECK(strcmp(err.getMessage(), "1 2.25 a string") == 0);
}
