PROJ_ARCH = "Unknown";

equals(QT_ARCH, x86_64) {
    PROJ_ARCH = "x64"
} else : equals(QT_ARCH, i386) {
    PROJ_ARCH = "x86"
}

CONFIG(debug, debug|release) {
    PROJ_SPEC = $$basename(QMAKESPEC)-$${PROJ_ARCH}-d
} else {
    PROJ_SPEC = $$basename(QMAKESPEC)-$${PROJ_ARCH}-r
}

DependencyPath = "release"
CONFIG(debug, debug|release) {
    DependencyPath = "debug"
} else {
    DependencyPath = "release"
}
