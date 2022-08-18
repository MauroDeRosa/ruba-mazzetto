# LOGGING
option(log_debug "enable debug messages" ON)
option(log_date "log date and time (disable to log time only)" OFF)
option(log_exit_on_error "log will issue exit(EXIT_FALIURE) after any error" ON)
option(release_build "disable development compilation flags" OFF)

# ---- DO NOT EDIT IF YOU DON'T KNOW WHAT YOU'RE DOING ---- #

if(log_debug)
    add_compile_definitions(LOG_DEBUG)
endif()

if(log_date)
    add_compile_definitions(LOG_DATE)
endif()

if(log_exit_on_error)
    add_compile_definitions(LOG_EXIT_ON_ERROR)
endif()

if(release_build)
    set(DEVELOPMENT_FLAGS "")
endif()
