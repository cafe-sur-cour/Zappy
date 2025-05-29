/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Exceptions_test
*/

#include <gtest/gtest.h>
#include <string>
#include "../../../src/Exceptions/Exceptions.hpp"

class ExceptionsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test the base CLIParsingException
TEST_F(ExceptionsTest, CLIParsingExceptionTest) {
    std::string errorMessage = "Test error message";
    Exceptions::CLIParsingException exception(errorMessage);

    std::string expectedMessage = std::string(colors::RED) +
                                "CLI Parsing Error: " + errorMessage +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const std::exception*>(&exception) != nullptr);
}

// Test the CLIPortException
TEST_F(ExceptionsTest, CLIPortExceptionTest) {
    std::string errorMessage = "Port must be a positive integer";
    Exceptions::CLIPortException exception(errorMessage);

    std::string expectedMessage = std::string(colors::RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::CYAN) +
                                "Port Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIHostException
TEST_F(ExceptionsTest, CLIHostExceptionTest) {
    std::string errorMessage = "Hostname cannot be empty";
    Exceptions::CLIHostException exception(errorMessage);

    std::string expectedMessage = std::string(colors::RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::CYAN) +
                                "Hostname Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIMissingArgumentException
TEST_F(ExceptionsTest, CLIMissingArgumentExceptionTest) {
    std::string errorMessage = "Missing required option: -p port";
    Exceptions::CLIMissingArgumentException exception(errorMessage);

    std::string expectedMessage = std::string(colors::RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::CYAN) +
                                "Missing Argument: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIInvalidArgumentException
TEST_F(ExceptionsTest, CLIInvalidArgumentExceptionTest) {
    std::string errorMessage = "Unknown option: -x";
    Exceptions::CLIInvalidArgumentException exception(errorMessage);

    std::string expectedMessage = std::string(colors::RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::CYAN) +
                                "Invalid Argument: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}
