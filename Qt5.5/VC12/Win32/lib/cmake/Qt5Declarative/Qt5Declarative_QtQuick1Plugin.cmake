
add_library(Qt5::QtQuick1Plugin MODULE IMPORTED)

_populate_Declarative_plugin_properties(QtQuick1Plugin RELEASE "qml1tooling/qmldbg_inspector.dll")
_populate_Declarative_plugin_properties(QtQuick1Plugin DEBUG "qml1tooling/qmldbg_inspectord.dll")

list(APPEND Qt5Declarative_PLUGINS Qt5::QtQuick1Plugin)
