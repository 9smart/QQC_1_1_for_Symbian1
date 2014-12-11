!mobility:!no_mobility {
    # bool load(feature, ignore_error)
    load(mobilityconfig, true):CONFIG += mobility
}

CONFIG += qt-components depend_includepath qt-components_build

unix:!symbian {
    CONFIG(release, debug|release) {
        OBJECTS_DIR = .release
        MOC_DIR = .release
        RCC_DIR = .release
    } else {
        OBJECTS_DIR = .debug
        MOC_DIR = .debug
        RCC_DIR = .debug
    }
}

# Transition flag for aligning screen api's width/height/displayWidth/displayHeight
# Once application have been updated, remove this define and flagings from screen api implementation and tests
DEFINES += USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
