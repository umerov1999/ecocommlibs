
add_library(Qt5::QtQuick2Plugin MODULE IMPORTED)

_populate_Qml_plugin_properties(QtQuick2Plugin RELEASE "qmltooling/qmldbg_qtquick2.dll")
_populate_Qml_plugin_properties(QtQuick2Plugin DEBUG "qmltooling/qmldbg_qtquick2d.dll")

list(APPEND Qt5Qml_PLUGINS Qt5::QtQuick2Plugin)
