find_program(BASH_EXE bash)
if (BASH_EXE)
    add_test(regtest ${BASH_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/test.sh)
endif(BASH_EXE)