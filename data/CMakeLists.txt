SET(DATADIR "${SHARE_INSTALL_PREFIX}/radioedit/data")

if(APPLE)
  SET(DATADIR "${BIN_INSTALL_DIR}/radioedit.app/Contents/Resources/data") #OMG NASTY
endif(APPLE)

add_subdirectory(icons)
if(UNIX AND NOT APPLE)
  install(FILES radioedit.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
endif(UNIX AND NOT APPLE)
install(DIRECTORY chords DESTINATION ${DATADIR})
install(DIRECTORY sprites DESTINATION ${DATADIR})
install(DIRECTORY fonts DESTINATION ${DATADIR})
install(FILES bg.png DESTINATION ${DATADIR})
install(FILES cable.png DESTINATION ${DATADIR})
install(FILES cloud.png DESTINATION ${DATADIR})
install(FILES forest.png DESTINATION ${DATADIR})
install(FILES grass.png DESTINATION ${DATADIR})
install(FILES light.png DESTINATION ${DATADIR})
install(FILES lines.png DESTINATION ${DATADIR})
install(FILES mark-big.png DESTINATION ${DATADIR})
install(FILES mark-small.png DESTINATION ${DATADIR})
install(FILES speaker.png DESTINATION ${DATADIR})
install(FILES stage.png DESTINATION ${DATADIR})
install(FILES click.flac DESTINATION ${DATADIR})
install(FILES dosowisko.flac DESTINATION ${DATADIR})
install(FILES end.flac DESTINATION ${DATADIR})
install(FILES kbd.flac DESTINATION ${DATADIR})
install(FILES key.flac DESTINATION ${DATADIR})
install(FILES menu.flac DESTINATION ${DATADIR})
install(FILES quit.flac DESTINATION ${DATADIR})
install(FILES solo.flac DESTINATION ${DATADIR})
