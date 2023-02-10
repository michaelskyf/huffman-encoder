build_tests = get_option('tests')

if not build_tests.disabled()

    gtest_main_dep = dependency('gtest', main : true, fallback: ['gtest', 'gtest_main_dep'], required: build_tests)
    gtest_dep = dependency('gtest', fallback: ['gtest', 'gtest_dep'], required: build_tests)

    if gtest_dep.found() and gtest_main_dep.found()
        inc += include_directories('../src')
        subdir('src')
    endif

endif