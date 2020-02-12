TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        adc.c \
        foc.c \
        main.c \
        pwm.c

HEADERS += \
    adc.h \
    const_def.h \
    err_table.h \
    foc.h \
    main.h \
    motor_ch.h \
    pwm.h
