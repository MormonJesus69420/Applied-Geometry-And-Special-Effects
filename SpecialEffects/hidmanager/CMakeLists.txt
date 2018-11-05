project(hidmanager)

################################
# Find the required Qt libraries
find_package(Qt5Core 5.1)
find_package(Qt5Quick 5.1)
find_package(Qt5Gui 5.1)


#########
# Headers
set( HDRS
  hidaction.h
  hidbinding.h
  hidinput.h
  hidinputevent.h
  hidkbmouseinput.h
  hidmanager.h
  hidmanagermodel.h
  hidmanagertreemodel.h
  standardhidmanager.h
  )

#########
# Sources
set( SRCS
  hidaction.cpp
  hidbinding.cpp
  hidinput.cpp
  hidinputevent.cpp
  hidkbmouseinput.cpp
  hidmanager.cpp
  hidmanagermodel.cpp
  hidmanagertreemodel.cpp
  standardhidmanager.cpp
  )

#########
# Qt: Moc
QT5_WRAP_CPP( HDRS_MOC
  hidaction.h
  hidmanager.h
  hidmanagermodel.h
  hidmanagertreemodel.h
  standardhidmanager.h
)

QT5_ADD_RESOURCES( RCCS_MOC ${RCCS} )

##########################
# Add files to the library
add_library( ${PROJECT_NAME} STATIC ${HDRS} ${SRCS} ${HDRS_MOC} ${RCCS_MOC} )
