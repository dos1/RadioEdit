if(UNIX AND NOT APPLE)
  install(FILES 16/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/16x16/apps)
  install(FILES 32/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/32x32/apps)
  install(FILES 48/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/48x48/apps)
  install(FILES 64/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/64x64/apps)
  install(FILES 128/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/128x128/apps)
  install(FILES 256/radioedit.png DESTINATION ${ICON_INSTALL_DIR}/hicolor/256x256/apps)
endif(UNIX AND NOT APPLE)
install(FILES radioedit.png DESTINATION ${DATADIR}/icons)
