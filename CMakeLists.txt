cmake_minimum_required(VERSION 3.5)
project(primer-proyecto)

set(CMAKE_CXX_STANDARD 11)

# Log
set(LOGGER Logger/LoggerClient.cpp Logger/LoggerClient.h Logger/LoggerServer.cpp Logger/LoggerServer.h)

# Exceptions
set(MKNOD_EXCEPTION Exceptions/MKNODException.cpp Exceptions/MKNODException.h)
set(FILE_EXIST_EXCEPTION Exceptions/FileExistsException.cpp Exceptions/FileExistsException.h)

# IPC
set(FIFO IPC/Fifo/Fifo.cpp IPC/Fifo/Fifo.h IPC/Fifo/FifoReader.cpp IPC/Fifo/FifoReader.h
        IPC/Fifo/FifoWriter.cpp IPC/Fifo/FifoWriter.h ${MKNOD_EXCEPTION} ${FILE_EXIST_EXCEPTION})
set(SIGNAL IPC/Signal/EventHandler.h IPC/Signal/SIGINTShutdownHandler.h IPC/Signal/SignalHandler.cpp
        IPC/Signal/SignalHandler.h)

# Commons
set(COMMONS Commons/Messages.h Commons/DataRecordManager.h)

# Portal
set(PORTAL_SOURCES Portal/PortalApp.cpp Portal/PortalController.cpp Portal/PortalController.h
        Portal/MSQueryController.cpp Portal/MSQueryController.h ${SIGNAL})
add_executable(portal ${PORTAL_SOURCES} ${FIFO} ${LOGGER} ${COMMONS})

# Microservices
set(MICROSERVICE_SOURCES Microservices/MicroserviceController.cpp Microservices/MicroserviceController.h
        ${FIFO} ${LOGGER})
set(CRUD_MS_SOURCES Microservices/MicroserviceApp.cpp Microservices/CRUDMicroserviceController.h
        ${MICROSERVICE_SOURCES} ${COMMONS})
add_executable(microservice ${CRUD_MS_SOURCES})

# ClientController
set(CLIENT_SOURCES Client/ClientApp.cpp Client/ClientController.cpp Client/ClientController.h)
add_executable(client ${CLIENT_SOURCES} ${FIFO} ${LOGGER} ${COMMONS})

# Test
set(TEST_CATCH Test/catch.hpp)
set(FIFO_TEST Test/FifoTest/FifoTest.cpp ${TEST_CATCH} ${FIFO})
add_executable(tests ${FIFO_TEST})

# Logger Server
add_executable(logger_server ${LOGGER} Logger/LoggerServerApp.cpp  ${FIFO}  ${SIGNAL})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/logs)