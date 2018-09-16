TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    GTestProfile.cpp \
    ../googletest/googletest/src/gtest_main.cc \
    ../googletest/googletest/src/gtest-all.cc

INCLUDEPATH += \
	../googletest/googletest \
	../googletest/googletest/include

HEADERS += \
    ../Profile/include/ambiesoft.profile.h

linux-g++ {
    QMAKE_CXXFLAGS += -std=gnu++0x -pthread
    QMAKE_CFLAGS += -std=gnu++0x -pthread
    LIBS += -pthread
}
