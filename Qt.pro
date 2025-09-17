QT = core gui

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets


SOURCES += \

    classes/card.cpp \
    classes/tokens.cpp \
    classes/game.cpp \
    classes/player.cpp \
    classes/deck.cpp \
    classes/board.cpp \
    classes/privilege.cpp \
    classes/bag.cpp \
    classes/draw.cpp \
    Qt_cmake/main_old.cpp \
    Qt_cmake/mainwindow.cpp \


HEADERS += \

    classes/Exception.h \
    classes/card.h \
    classes/tokens.h \
    classes/game.h \
    classes/player.h \
    classes/deck.h \
    classes/board.h \
    classes/privilege.h \
    classes/bag.h \
    classes/draw.h \
    main.h \

