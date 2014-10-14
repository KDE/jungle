find_program(GUESSIT_BINARY guessit)

if(GUESSIT_BINARY)
    exec_program(${GUESSIT_BINARY} ARGS --version OUTPUT_VARIABLE GUESSIT_VERSION_OUTPUT)

    string(REGEX MATCH "GuessIt ([0-9.]+)" GUESSIT_VERSION_MATCHED ${GUESSIT_VERSION_OUTPUT})
    if (GUESSIT_VERSION_MATCHED)
        set(GuessIt_VERSION ${CMAKE_MATCH_1})

        if("${GuessIt_VERSION}" VERSION_LESS "${GuessIt_FIND_VERSION}")
            message("GuessIt Incorrect version found ${GuessIt_VERSION}")
            set(GuessIt_VERSION_COMPATIBLE FALSE)
        else()
            set(GUESSIT_FOUND true)
            set(GuessIt_VERSION_COMPATIBLE TRUE)
        endif()
    endif()
endif()
