find_package(Doxygen)

if (DOXYGEN_FOUND)
    # request to configure the file
    configure_file(Doxyfile build/Doxyfile @ONLY)
	  configure_file(Makefile.spx Makefile.spx @ONLY)
    message("Doxygen build started")

    # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
    add_custom_target( docs
        COMMAND make html -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile.spx
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Sphinx and Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)