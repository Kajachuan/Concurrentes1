#define CATCH_CONFIG_MAIN

#include <fcntl.h>
#include <sys/wait.h>
#include "../catch.hpp"
#include "../../IPC/Fifo/FifoWriter.h"
#include "../../IPC/Fifo/FifoReader.h"
#include "../../Exceptions/FileExistsException.h"

using namespace std;
const int BUFFER_SIZE = 20;
const char *TEST_MESSAGE = "test message";
const char *TEST_FIFO_PATH = "/tmp/test_fifo_path";
const char *TEST_NOT_FIFO_PATH = "/bin/bash";

TEST_CASE("Fifo read creation ok") {
    REQUIRE_NOTHROW(FifoReader(TEST_FIFO_PATH));
}

TEST_CASE("Fifo write creation ok") {
    REQUIRE_NOTHROW(FifoWriter(TEST_FIFO_PATH));
    exit(0);
}

TEST_CASE("Fifo creation does not fail on file created by other side of fifo") {
    __pid_t fifoId = fork();
    if (fifoId == 0) {
        REQUIRE_NOTHROW(FifoWriter(TEST_FIFO_PATH));
        exit(0);
    } else {
        sleep(1);
        REQUIRE_NOTHROW(FifoReader(TEST_FIFO_PATH));
    }
    int status;
    wait(&status);
    REQUIRE(WEXITSTATUS(status) == 0);
}

TEST_CASE("Fifo creation fail on created files that are not fifos") {
    REQUIRE_THROWS_AS(FifoWriter(TEST_NOT_FIFO_PATH), FileExistsException);
}

TEST_CASE("Fifo open communication ok") {
    __pid_t fifoId = fork();
    if (fifoId == 0) {
        FifoWriter fifoEscritura(TEST_FIFO_PATH);
        exit(0);
    } else {
        sleep(1);
        FifoReader fifoLectura(TEST_FIFO_PATH);
        fifoLectura.open_fifo();
    }
    int status;
    wait(&status);
    REQUIRE(WEXITSTATUS(status) == 0);
}

TEST_CASE("Fifo complete communication ok") {
    __pid_t fifoId = fork();
    if (fifoId == 0) {
        FifoWriter fifoEscritura(TEST_FIFO_PATH);
        fifoEscritura.open_fifo();
        std::string mensaje = TEST_MESSAGE;
        REQUIRE(fifoEscritura.write_fifo(static_cast<const void *>(mensaje.c_str()), mensaje.length()) > 0);
        exit(0);
    } else {
        sleep(1);
        char buffer[BUFFER_SIZE];
        FifoReader fifoLectura(TEST_FIFO_PATH);
        fifoLectura.open_fifo();
        ssize_t readedBytes = fifoLectura.read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
        REQUIRE(readedBytes > 0);
        std::string mensaje = buffer;
        mensaje.resize(static_cast<unsigned long>(readedBytes));
        REQUIRE(mensaje == TEST_MESSAGE);
    }
    int status;
    wait(&status);
    REQUIRE(WEXITSTATUS(status) == 0);
}
