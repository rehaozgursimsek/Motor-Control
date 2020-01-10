TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        clarke.c \
        error_table.c \
        limitation.c \
        main.c \
        park.c \
        pi_controller.c \
        svm.c \
        typedef.c

HEADERS += \
    clarke.h \
    error_table.h \
    limitation.h \
    park.h \
    pi_controller.h \
    svm.h \
    typedef.h
