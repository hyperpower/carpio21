function(foo)
    file(WRITE "./out.txt" CMake_Version : ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}\n)
    file(APPEND "./out.txt" System_Name : ${CMAKE_SYSTEM_NAME}\n)
    file(APPEND "./out.txt" System_Verion : ${CMAKE_SYSTEM_VERSION}\n)
    file(APPEND "./out.txt" System_Processor : ${CMAKE_HOST_SYSTEM_PROCESSOR}\n)
    file(APPEND "./out.txt" CXX_Compiler : ${CMAKE_CXX_COMPILER}\n)
    file(APPEND "./out.txt" CXX_Compiler_Version : ${CMAKE_CXX_COMPILER_VERSION}\n)
    file(APPEND "./out.txt" CXX_Flags : ${CMAKE_CXX_FLAGS}\n)
    file(APPEND "./out.txt" test : ${CMAKE_SYSTEM_PROCESSOR}\n)
    

    # cmake_host_system_information(RESULT DISTRO QUERY OS_NAME NUMBER_OF_PHYSICAL_CORES NUMBER_OF_LOGICAL_CORES TOTAL_PHYSICAL_MEMORY PROCESSOR_NAME PROCESSOR_DESCRIPTION)
    # foreach(VAR IN LISTS DISTRO)
    #     file(APPEND "./out.txt" ${VAR}=${${VAR}}\n)
    # endforeach()
# message(PROJECT_SOURCE_DIR)
endfunction()