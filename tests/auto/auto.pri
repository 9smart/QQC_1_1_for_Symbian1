CONFIG += qtestlib testcase

symbian {
    imports.sources = $$OTHER_FILES
    imports.path = .
    DEPLOYMENT += imports
}
