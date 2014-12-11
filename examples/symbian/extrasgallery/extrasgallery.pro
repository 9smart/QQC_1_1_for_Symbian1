TARGET = qmlextrasgallery
TEMPLATE = app
QT += declarative
CONFIG += qt-components
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD
include (../shared/utils.pri)

SOURCES += extrasgallery.cpp

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x2003A914
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = ReadDeviceData SwEvent WriteDeviceData
    gallery_qmls.sources = *.qml
    gallery_images.sources = assets/*.png
    gallery_images.path = assets
    DEPLOYMENT += gallery_qmls gallery_images
    BLD_INF_RULES.prj_exports += "qmlextrasgallery.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(qmlextrasgallery.iby)"
    VERSION = 10.1.0

    stubsis = \
        "START EXTENSION app-services.buildstubsis" \
        "OPTION SISNAME qmlextrasgallery_stub" \
        "OPTION SRCDIR ."\
        "END"
    BLD_INF_RULES.prj_extensions = stubsis

    vendor_info = \
            " " \
            "; Localised Vendor name" \
            "%{\"Nokia\"}" \
            " " \
            "; Unique Vendor name" \
            ":\"Nokia\"" \
            " "

    header = "$${LITERAL_HASH}{\"qmlextrasgallery\"},(0x2003A914),1,1,0,TYPE=SA,RU"
    package.pkg_prerules += vendor_info header
    DEPLOYMENT += package
}

OTHER_FILES += \
    main.qml \
    DatePickerDialog.qml \
    InfoBannerPage.qml \
    RatingIndicator.qml \
    TimePickerDialog.qml \
    Tumbler.qml
