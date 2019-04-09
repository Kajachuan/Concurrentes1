cmake_minimum_required(VERSION 3.5)
project(primer-proyecto)

set(CMAKE_CXX_STANDARD 11)

# Exceptions
set(MKNOD_EXCEPTION Exceptions/MKNODException.cpp Exceptions/MKNODException.h)
set(FILE_EXIST_EXCEPTION Exceptions/FileExistsException.cpp Exceptions/FileExistsException.h)

# IPC
set(FIFO IPC/Fifo/Fifo.cpp IPC/Fifo/Fifo.h IPC/Fifo/FifoReader.cpp IPC/Fifo/FifoReader.h
        IPC/Fifo/FifoWriter.cpp IPC/Fifo/FifoWriter.h ${MKNOD_EXCEPTION} ${FILE_EXIST_EXCEPTION})

# Portal
set(PORTAL_SOURCES Portal/PortalApp.cpp Portal/PortalController.cpp Portal/PortalController.h)
add_executable(portal ${PORTAL_SOURCES} ${FIFO})

# ClientController
set(CLIENT_SOURCES Client/ClientApp.cpp Client/ClientController.cpp Client/ClientController.h)
add_executable(client ${CLIENT_SOURCES} ${FIFO})

# Weather microservice
set(WEATHER_MS_SOURCES Weather/City.cpp Weather/City.h Weather/WeatherMicroserviceApp.cpp
        Weather/WeatherMicroservice.cpp Weather/WeatherMicroservice.h)
add_executable(weather_ms ${WEATHER_MS_SOURCES} ${FIFO})

# Test
set(TEST_CATCH test/catch.hpp)
set(FIFO_TEST test/FifoTest/FifoTest.cpp ${TEST_CATCH} ${FIFO})
add_executable(tests ${FIFO_TEST})
