
add_library(Qt5::QJp2Plugin MODULE IMPORTED)

_populate_Gui_plugin_properties(QJp2Plugin RELEASE "imageformats/qjp2.dll")
_populate_Gui_plugin_properties(QJp2Plugin DEBUG "imageformats/qjp2d.dll")

list(APPEND Qt5Gui_PLUGINS Qt5::QJp2Plugin)
