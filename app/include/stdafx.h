#pragma once

//
// Qt
//

#include <QObject>
#include <QThread>
#include <QWidget>
#include <QDialog>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QMessageBox>
#include <QTextCodec>
#include <QAbstractListModel>
#include <QKeyEvent>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaType>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QAbstractItemDelegate>
#include <QWaitCondition>
#include <QQueue>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QDateTime>
#include <QLineEdit>
#include <QScrollBar>
#include <QScreen>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QReadWriteLock>
#include <QLabel>

//
// C/C++
//
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <atomic>
#include <memory>
#include <utility>
#include <cassert>
#include <functional>
#include <optional>
#include <sstream>
#include <csignal>
#include <limits>
#include <fstream>
#include <iostream>
#include <random>
#include <array>

#include <windows.h>

#include "assert_impl.h"

#define ASSERT(condition) if((condition) == false) \
	doAssert(__FILE__, __LINE__, __FUNCTION__, #condition);

#ifdef NDEBUG

#define DEBUG_ASSERT(condition)
#define VERIFY(Connection) Connection

#else

#define DEBUG_ASSERT(condition) ASSERT(condition)
#define VERIFY(Connection) ASSERT(Connection)

#endif
