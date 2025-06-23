/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Client_test
*/

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "../../../../gui/src/Client/Client.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"

class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        originalCout = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(originalCout);
    }

    char **createArgv(const std::vector<std::string> &args) {
        char **argv = new char*[args.size()];
        for (size_t i = 0; i < args.size(); i++) {
            argv[i] = new char[args[i].length() + 1];
            strcpy(argv[i], args[i].c_str());
        }
        return argv;
    }

    void cleanupArgv(char **argv, int argc) {
        for (int i = 0; i < argc; i++) {
            delete[] argv[i];
        }
        delete[] argv;
    }

    std::stringstream buffer;
    std::streambuf* originalCout;
};

// Test successful client initialization with valid arguments
TEST_F(ClientTest, SuccessfulInitialization) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Client initialized with configuration") != std::string::npos);
    EXPECT_TRUE(output.find("Port: 4242") != std::string::npos);
    EXPECT_TRUE(output.find("Hostname: 127.0.0.1") != std::string::npos);

    cleanupArgv(argv, argc);
}

// Test client initialization with localhost
TEST_F(ClientTest, LocalhostInitialization) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "localhost"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Client initialized with configuration") != std::string::npos);
    EXPECT_TRUE(output.find("Port: 4242") != std::string::npos);
    EXPECT_TRUE(output.find("Hostname: 127.0.0.1") != std::string::npos);

    cleanupArgv(argv, argc);
}

// Test client initialization with reversed argument order
TEST_F(ClientTest, ReversedArgumentOrder) {
    std::vector<std::string> args = {"./zappy_gui", "-h", "127.0.0.1", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Client initialized with configuration") != std::string::npos);
    EXPECT_TRUE(output.find("Port: 4242") != std::string::npos);
    EXPECT_TRUE(output.find("Hostname: 127.0.0.1") != std::string::npos);

    cleanupArgv(argv, argc);
}

// Test client initialization with invalid port (should throw exception)
TEST_F(ClientTest, InvalidPort) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "-1", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test client initialization with invalid hostname (should throw exception)
TEST_F(ClientTest, InvalidHostname) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "invalid.host"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test client initialization with missing port (should throw exception)
TEST_F(ClientTest, MissingPort) {
    std::vector<std::string> args = {"./zappy_gui", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test client initialization with missing hostname (should throw exception)
TEST_F(ClientTest, MissingHostname) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test client initialization with unknown flag (should throw exception)
TEST_F(ClientTest, UnknownFlag) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-x", "unknown", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test client initialization with incorrect number of arguments (should throw exception)
TEST_F(ClientTest, IncorrectNumberOfArguments) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h"};
    int argc = args.size();
    char **argv = createArgv(args);

    EXPECT_THROW(Client client(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test client initialization with maximum valid port
TEST_F(ClientTest, MaximumValidPort) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "65535", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Client initialized with configuration") != std::string::npos);
    EXPECT_TRUE(output.find("Port: 65535") != std::string::npos);
    EXPECT_TRUE(output.find("Hostname: 127.0.0.1") != std::string::npos);

    cleanupArgv(argv, argc);
}

// Tests for tryToCreateGuiWithSharedLibInFolder method
TEST_F(ClientTest, TryCreateGuiWithValidDirectory) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Create a temporary test directory
    std::filesystem::path testDir = std::filesystem::temp_directory_path() / "zappy_test_gui";
    std::filesystem::create_directories(testDir);

    // Create a dummy .so file
    std::ofstream dummyLib(testDir / "libTest.so");
    dummyLib << "dummy shared library content";
    dummyLib.close();

    // Should not throw, even if the shared library is invalid
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder(testDir.string()));

    // Clean up
    std::filesystem::remove_all(testDir);
    cleanupArgv(argv, argc);
}

TEST_F(ClientTest, TryCreateGuiWithNonExistentDirectory) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Should handle non-existent directory gracefully
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder("/non/existent/path"));

    cleanupArgv(argv, argc);
}

TEST_F(ClientTest, TryCreateGuiWithEmptyDirectory) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Create empty test directory
    std::filesystem::path testDir = std::filesystem::temp_directory_path() / "zappy_empty_test";
    std::filesystem::create_directories(testDir);

    // Should handle empty directory gracefully
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder(testDir.string()));

    // Clean up
    std::filesystem::remove_all(testDir);
    cleanupArgv(argv, argc);
}

TEST_F(ClientTest, TryCreateGuiWithDirectoryContainingNonSoFiles) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Create test directory with non-.so files
    std::filesystem::path testDir = std::filesystem::temp_directory_path() / "zappy_non_so_test";
    std::filesystem::create_directories(testDir);

    // Create non-.so files
    std::ofstream txtFile(testDir / "readme.txt");
    txtFile << "readme content";
    txtFile.close();

    std::ofstream configFile(testDir / "config.cfg");
    configFile << "config content";
    configFile.close();

    // Should handle directory with no .so files gracefully
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder(testDir.string()));

    // Clean up
    std::filesystem::remove_all(testDir);
    cleanupArgv(argv, argc);
}

TEST_F(ClientTest, TryCreateGuiWithMixedFiles) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Create test directory with mixed files
    std::filesystem::path testDir = std::filesystem::temp_directory_path() / "zappy_mixed_test";
    std::filesystem::create_directories(testDir);

    // Create .so files
    std::ofstream soFile1(testDir / "lib1.so");
    soFile1 << "dummy library 1";
    soFile1.close();

    std::ofstream soFile2(testDir / "lib2.so");
    soFile2 << "dummy library 2";
    soFile2.close();

    // Create non-.so files
    std::ofstream txtFile(testDir / "readme.txt");
    txtFile << "readme";
    txtFile.close();

    // Should process .so files and ignore others
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder(testDir.string()));

    // Clean up
    std::filesystem::remove_all(testDir);
    cleanupArgv(argv, argc);
}

TEST_F(ClientTest, TryCreateGuiWithDefaultPath) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    Client client(argc, argv);

    // Should handle default path without throwing
    EXPECT_NO_THROW(client.tryToCreateGuiWithSharedLibInFolder("./gui/lib/"));

    cleanupArgv(argv, argc);
}
