cmake_minimum_required(VERSION 3.16)

project(GreatScanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Multimedia MultimediaWidgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Multimedia MultimediaWidgets Pdf PdfWidgets Concurrent Sql)
find_package(OpenCV REQUIRED) # 添加 OpenCV 库

# 包含 OpenCV 头文件目录
include_directories(
${OpenCV_INCLUDE_DIRS}
${OpenCV_DIR}/include
"D:/cppsoft/minidocx-0.6.0/3rdparty/pugixml-1.13"
"D:/cppsoft/minidocx-0.6.0/3rdparty/zip-0.2.1"
"D:/cppsoft/minidocx-0.6.0/src"
"D:/cppsoft/xlnt-master/include"
"D:/cppsoft/mupdf-1.26.3-source/include"
)


set(PROJECT_SOURCES
main.cpp
imgeditor.h imgeditor.cpp imgeditor.ui
assets.qrc
pointitem.h pointitem.cpp
frame.h frame.cpp
customgraphicsview.h customgraphicsview.cpp
infinitedial.h infinitedial.cpp
customtoolbutton.h customtoolbutton.cpp
movepointcommand.h movepointcommand.cpp
undo_stack_singleton.h
rotateimgcommand.h rotateimgcommand.cpp
undo_stack_singleton.cpp
config.ini
global.h
global.cpp



)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt_add_executable(GreatScanner
MANUAL_FINALIZATION
${PROJECT_SOURCES}




singleton.h

flowlayout.h flowlayout.cpp
cardview.h cardview.cpp
cardgroup.h cardgroup.cpp


textlabel.h textlabel.cpp

infocard.h infocard.cpp
roundprogressbar.h roundprogressbar.cpp

Def.h

TestWindow.h
simplebutton.h simplebutton.cpp
logicmgr.h logicmgr.cpp
tcpclient.h tcpclient.cpp


fileuploader.h fileuploader.cpp
fileuploadqueue.h fileuploadqueue.cpp


rightbtnmenu.h rightbtnmenu.cpp

notification.h notification.cpp



httpclient.h httpclient.cpp
loginorsignupwidget.h loginorsignupwidget.cpp loginorsignupwidget.ui
clickablelabel.h clickablelabel.cpp
sliderindicator.h sliderindicator.cpp
_singleton.h
diffusion_button.h diffusion_button.cpp
main_window.h main_window.cpp main_window.ui
toolspage.h toolspage.cpp
panel.cpp
panel.h
panel.h
srcselector.h srcselector.cpp srcselector.ui
ocrclient.h ocrclient.cpp
ipc/include/IShmChannelNotify.h ipc/include/ShmIPCClient.h ipc/include/ShmIPCServer.h
ipc/src/ShmIPCDefine.h ipc/src/ShmMsgDataBuffer.h ipc/src/StringTools.h
ipc/src/ShmIPCClient.cpp ipc/src/ShmIPCServer.cpp ipc/src/ShmMsgDataBuffer.cpp ipc/src/StringTools.cpp
ipc/src/sharemem/SharedMemory.cpp ipc/src/sharemem/SharedMemory.h
ipc/src/handler/ShmIPCReader.cpp ipc/src/handler/ShmIPCReader.h ipc/src/handler/ShmIPCWriter.cpp ipc/src/handler/ShmIPCWriter.h ipc/src/handler/ThreadCtrl.cpp ipc/src/handler/ThreadCtrl.h

systray.h systray.cpp
ocrtextdlg.h ocrtextdlg.cpp ocrtextdlg.ui
json.hpp
placeholdertextitem.h placeholdertextitem.cpp
ocrviewer.h ocrviewer.cpp
constants.hpp








LayoutReconstructor.h
cvutils.h cvutils.cpp
ExcelReconstructor.h
pdfwatermarkdlg.h pdfwatermarkdlg.cpp pdfwatermarkdlg.ui
markeditor.h markeditor.cpp markeditor.ui
colorblock.h colorblock.cpp
optionsdlg.h optionsdlg.cpp optionsdlg.ui
hotkeymapper.h


myrect.h
screen.h
qslabel.h
controlwidget.h
myrect.cpp
qslabel.cpp
screen.cpp
controlwidget.cpp
controlwidget.ui
configs/ShortCut.json
localkeyeventmgr.h
globalkeyeventmgr.h

globalkeyeventmgr.cpp
search/SearchDef.h

searchbox.h searchbox.cpp


search/searchengine.h
search/cppjieba/DictTrie.hpp search/cppjieba/FullSegment.hpp search/cppjieba/HMMModel.hpp search/cppjieba/HMMSegment.hpp search/cppjieba/Jieba.hpp search/cppjieba/KeywordExtractor.hpp search/cppjieba/MixSegment.hpp search/cppjieba/MPSegment.hpp search/cppjieba/PosTagger.hpp search/cppjieba/PreFilter.hpp search/cppjieba/QuerySegment.hpp search/cppjieba/SegmentBase.hpp search/cppjieba/SegmentTagged.hpp search/cppjieba/TextRankExtractor.hpp search/cppjieba/Trie.hpp search/cppjieba/Unicode.hpp
search/cppjieba/dict/hmm_model.utf8 search/cppjieba/dict/idf.utf8 search/cppjieba/dict/jieba.dict.utf8 search/cppjieba/dict/stop_words.utf8 search/cppjieba/dict/user.dict.utf8
autocomplete.h autocomplete.cpp autocomplete.ui
autocompleteitem.h autocompleteitem.cpp autocompleteitem.ui
local_history.db
search/JiebaSingleton.h

sql/sqlite/sqlitemgr.cpp sql/sqlite/sqlitemgr.h
sql/sqlite/basedao.h
sql/sqlite/historydao.h sql/sqlite/historydao.cpp
historyrecorditem.h historyrecorditem.cpp historyrecorditem.ui























)
else()
add_executable(GreatScanner
${PROJECT_SOURCES}
)
endif()


# 添加多媒体链接库
target_link_libraries(GreatScanner PRIVATE
Qt${QT_VERSION_MAJOR}::Widgets
Qt${QT_VERSION_MAJOR}::Network
Qt${QT_VERSION_MAJOR}::Multimedia
Qt${QT_VERSION_MAJOR}::MultimediaWidgets
Qt${QT_VERSION_MAJOR}::Pdf # 链接PDF核心库
Qt${QT_VERSION_MAJOR}::PdfWidgets # 链接PDF显示组件
Qt${QT_VERSION_MAJOR}::Concurrent
Qt${QT_VERSION_MAJOR}::Sql
${OpenCV_LIBS}
"D:/cppsoft/minidocx-0.6.0/build/Debug/minidocxd.lib"
"D:/cppsoft/xlnt-master/build/source/Debug/xlntd.lib"
"D:/cppsoft/mupdf-1.26.3-source/platform/win32/x64/Debug/libmupdf.lib"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GreatScanner)
endif()
set_target_properties(GreatScanner PROPERTIES
${BUNDLE_ID_OPTION}
MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
MACOSX_BUNDLE TRUE
WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GreatScanner
BUNDLE DESTINATION .
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
qt_finalize_executable(GreatScanner)
endif()
