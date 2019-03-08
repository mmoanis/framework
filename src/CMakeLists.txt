set(SRC_FILES
    main.cpp
    module.cpp
    simulation.cpp
    threadPool.cpp
)

add_executable(framework ${SRC_FILES})

# https://stackoverflow.com/questions/1620918/cmake-and-libpthread
TARGET_LINK_LIBRARIES(framework Threads::Threads)
