
add_library(Qt5::QTcpServerConnection MODULE IMPORTED)

_populate_Qml_plugin_properties(QTcpServerConnection RELEASE "qmltooling/qmldbg_tcp.dll")
_populate_Qml_plugin_properties(QTcpServerConnection DEBUG "qmltooling/qmldbg_tcpd.dll")

list(APPEND Qt5Qml_PLUGINS Qt5::QTcpServerConnection)
