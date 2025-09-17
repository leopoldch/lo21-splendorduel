QT = core gui

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets


SOURCES += \

    core/card.cpp \
    core/tokens.cpp \
    core/game.cpp \
    core/player.cpp \
    core/deck.cpp \
    core/board.cpp \
    core/privilege.cpp \
    core/bag.cpp \
    core/draw.cpp \
    Qt/main_window.qt.cpp \


HEADERS += \

    core/Exception.h \
    core/card.h \
    core/tokens.h \
    core/game.h \
    core/player.h \
    core/deck.h \
    core/board.h \
    core/privilege.h \
    core/bag.h \
    core/draw.h \
    main.h \

