if(Libdvbv5_FOUND)
  include_directories(${Libdvbv5_INCLUDE_DIRS})
endif(Libdvbv5_FOUND)

qt5_add_resources(RESOURCE_ADDED kaffeine.qrc dvbtest/omap/mapwidget/mapresources.qrc
)

set(MOC_HEADERS
   testdialog.h)

set (UIS
  testdialog.ui
)

SET(CMAKE_AUTOUIC ON)

QT5_WRAP_UI( UI_HEADERS ${UIS} )


set(kaffeine_SRCS
    kaffeine.qrc
    backend-vlc/vlcmediawidget.cpp
    playlist/playlistmodel.cpp
    playlist/playlisttab.cpp
    abstractmediawidget.cpp
    configuration.cpp
    configurationdialog.cpp
    datetimeedit.cpp
    dbusobjects.cpp
    ensurenopendingoperation.cpp
    main.cpp
    mainwindow.cpp
    mediawidget.cpp
    osdwidget.cpp
    sqlhelper.cpp
    sqlinterface.cpp)

if(HAVE_DVB)
  set(kaffeinedvb_SRCS
      iso-codes.cpp
      dvb/dvbcam_linux.cpp
      dvb/dvbchannel.cpp
      dvb/dvbchanneldialog.cpp
      dvb/dvbconfigdialog.cpp
      dvb/dvbdevice.cpp
      dvb/dvbdevice_linux.cpp
      dvb/dvbepg.cpp
      dvb/dvbepgdialog.cpp
      dvb/dvbliveview.cpp
      dvb/dvbmanager.cpp
      dvb/dvbrecording.cpp
      dvb/dvbrecordingdialog.cpp
      dvb/dvbscan.cpp
      dvb/dvbscandialog.cpp
      dvb/dvbsi.cpp
      dvb/dvbtab.cpp
      dvb/dvbtransponder.cpp
      dvb/xmltv.cpp
      dvbtest/testdialog.cpp
      dvbtest/testdialog.ui
      dvbtest/omap/core/
      dvbtest/omap/core/opmaps.cpp
      dvbtest/omap/core/providerstrings.cpp
      dvbtest/omap/core/rawtile.cpp
      dvbtest/omap/core/urlfactory.cpp
      dvbtest/omap/core/alllayersoftype.cpp
      dvbtest/omap/core/languagetype.cpp
      dvbtest/omap/core/diagnostics.cpp
      dvbtest/omap/core/placemark.cpp
      dvbtest/omap/core/pureimagecache.cpp
      dvbtest/omap/core/size.cpp
      dvbtest/omap/core/cache.cpp
      dvbtest/omap/core/memorycache.cpp
      dvbtest/omap/core/point.cpp
      dvbtest/omap/core/pureimage.cpp
      dvbtest/omap/core/tilecachequeue.cpp
      dvbtest/omap/core/cacheitemqueue.cpp
      dvbtest/omap/core/kibertilecache.cpp
      dvbtest/omap/internals/MouseWheelZoomType.cpp
      dvbtest/omap/internals/rectlatlng.cpp
      dvbtest/omap/internals/tilematrix.cpp
      dvbtest/omap/internals/core.cpp
      dvbtest/omap/internals/loadtask.cpp
      dvbtest/omap/internals/rectangle.cpp
      dvbtest/omap/internals/tile.cpp
      dvbtest/omap/internals/pointlatlng.cpp
      dvbtest/omap/internals/pureprojection.cpp
      dvbtest/omap/internals/sizelatlng.cpp
      dvbtest/omap/internals/projections/lks94projection.cpp
      dvbtest/omap/internals/projections/mercatorprojection.cpp
      dvbtest/omap/internals/projections/mercatorprojectionyandex.cpp
      dvbtest/omap/internals/projections/platecarreeprojection.cpp
      dvbtest/omap/internals/projections/platecarreeprojectionpergo.cpp
      dvbtest/omap/map/MAV2DIcon.cc
      dvbtest/omap/map/QGCMapToolBar.cc
      dvbtest/omap/map/QGCMapToolBar.ui
      dvbtest/omap/map/QGCMapWidget.cc
      dvbtest/omap/map/Waypoint2DIcon.cc
      dvbtest/omap/map/QGCMapTool.cc
      dvbtest/omap/map/QGCMapTool.ui
      dvbtest/omap/mapwidget/
      dvbtest/omap/mapwidget/configuration.cpp
      dvbtest/omap/mapwidget/homeitem.cpp
      dvbtest/omap/mapwidget/mapripform.cpp
      dvbtest/omap/mapwidget/gpsitem.cpp
      dvbtest/omap/mapwidget/traillineitem.cpp
      dvbtest/omap/mapwidget/waypointlineitem.cpp
      dvbtest/omap/mapwidget/mapgraphicitem.cpp
      dvbtest/omap/mapwidget/mapripform.ui
      dvbtest/omap/mapwidget/opmapwidget.cpp
      dvbtest/omap/mapwidget/Waypoint.cc
      dvbtest/omap/mapwidget/mapripper.cpp
      dvbtest/omap/mapwidget/uavitem.cpp
      dvbtest/omap/mapwidget/trailitem.cpp
      dvbtest/omap/mapwidget/waypointitem.cpp
      )
endif(HAVE_DVB)

configure_file(config-kaffeine.h.cmake ${CMAKE_BINARY_DIR}/config-kaffeine.h)

add_executable(kaffeine ${kaffeinedvb_SRCS} ${kaffeine_SRCS} ${UI_HEADERS} )
target_link_libraries(kaffeine Qt5::Sql Qt5::Network Qt5OpenGL Qt5Svg Qt5::X11Extras KF5::XmlGui KF5::I18n KF5::Solid
		      KF5::KIOCore KF5::KIOFileWidgets KF5::WindowSystem
		      KF5::DBusAddons ${X11_Xscreensaver_LIB} ${VLC_LIBRARY})

if(HAVE_DVB)
    target_link_libraries(kaffeine ${Libdvbv5_LIBRARIES})
endif(HAVE_DVB)

install(TARGETS kaffeine ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES scanfile.dvb DESTINATION ${DATA_INSTALL_DIR}/kaffeine)
install(PROGRAMS org.kde.kaffeine.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES org.kde.kaffeine.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
