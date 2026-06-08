/****************************************************************************
** Meta object code from reading C++ file 'AppViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../src/presentation/viewmodels/AppViewModel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_Presentation__AppViewModel_t {
    uint offsetsAndSizes[148];
    char stringdata0[27];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[14];
    char stringdata5[8];
    char stringdata6[10];
    char stringdata7[13];
    char stringdata8[9];
    char stringdata9[15];
    char stringdata10[25];
    char stringdata11[18];
    char stringdata12[17];
    char stringdata13[6];
    char stringdata14[6];
    char stringdata15[11];
    char stringdata16[14];
    char stringdata17[7];
    char stringdata18[7];
    char stringdata19[10];
    char stringdata20[9];
    char stringdata21[3];
    char stringdata22[12];
    char stringdata23[11];
    char stringdata24[10];
    char stringdata25[10];
    char stringdata26[12];
    char stringdata27[12];
    char stringdata28[19];
    char stringdata29[8];
    char stringdata30[17];
    char stringdata31[10];
    char stringdata32[13];
    char stringdata33[5];
    char stringdata34[13];
    char stringdata35[13];
    char stringdata36[13];
    char stringdata37[20];
    char stringdata38[16];
    char stringdata39[8];
    char stringdata40[8];
    char stringdata41[14];
    char stringdata42[11];
    char stringdata43[11];
    char stringdata44[18];
    char stringdata45[9];
    char stringdata46[14];
    char stringdata47[13];
    char stringdata48[7];
    char stringdata49[13];
    char stringdata50[7];
    char stringdata51[9];
    char stringdata52[8];
    char stringdata53[6];
    char stringdata54[12];
    char stringdata55[12];
    char stringdata56[17];
    char stringdata57[6];
    char stringdata58[20];
    char stringdata59[14];
    char stringdata60[11];
    char stringdata61[11];
    char stringdata62[20];
    char stringdata63[17];
    char stringdata64[20];
    char stringdata65[20];
    char stringdata66[12];
    char stringdata67[19];
    char stringdata68[8];
    char stringdata69[9];
    char stringdata70[11];
    char stringdata71[14];
    char stringdata72[11];
    char stringdata73[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Presentation__AppViewModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_Presentation__AppViewModel_t qt_meta_stringdata_Presentation__AppViewModel = {
    {
        QT_MOC_LITERAL(0, 26),  // "Presentation::AppViewModel"
        QT_MOC_LITERAL(27, 20),  // "currentScreenChanged"
        QT_MOC_LITERAL(48, 0),  // ""
        QT_MOC_LITERAL(49, 14),  // "sessionChanged"
        QT_MOC_LITERAL(64, 13),  // "errorOccurred"
        QT_MOC_LITERAL(78, 7),  // "message"
        QT_MOC_LITERAL(86, 9),  // "goToLogin"
        QT_MOC_LITERAL(96, 12),  // "goToRegister"
        QT_MOC_LITERAL(109, 8),  // "goToMenu"
        QT_MOC_LITERAL(118, 14),  // "goToInventario"
        QT_MOC_LITERAL(133, 24),  // "goToRegistroReproductivo"
        QT_MOC_LITERAL(158, 17),  // "goToRegistroLeche"
        QT_MOC_LITERAL(176, 16),  // "goToRegistroPeso"
        QT_MOC_LITERAL(193, 5),  // "login"
        QT_MOC_LITERAL(199, 5),  // "email"
        QT_MOC_LITERAL(205, 10),  // "contrasena"
        QT_MOC_LITERAL(216, 13),  // "createAccount"
        QT_MOC_LITERAL(230, 6),  // "nombre"
        QT_MOC_LITERAL(237, 6),  // "logout"
        QT_MOC_LITERAL(244, 9),  // "getFincas"
        QT_MOC_LITERAL(254, 8),  // "getFinca"
        QT_MOC_LITERAL(263, 2),  // "id"
        QT_MOC_LITERAL(266, 11),  // "createFinca"
        QT_MOC_LITERAL(278, 10),  // "nHectareas"
        QT_MOC_LITERAL(289, 9),  // "nPotreros"
        QT_MOC_LITERAL(299, 9),  // "capacidad"
        QT_MOC_LITERAL(309, 11),  // "updateFinca"
        QT_MOC_LITERAL(321, 11),  // "deleteFinca"
        QT_MOC_LITERAL(333, 18),  // "fincaTieneAnimales"
        QT_MOC_LITERAL(352, 7),  // "idFinca"
        QT_MOC_LITERAL(360, 16),  // "getGanadoByFinca"
        QT_MOC_LITERAL(377, 9),  // "getGanado"
        QT_MOC_LITERAL(387, 12),  // "createGanado"
        QT_MOC_LITERAL(400, 4),  // "data"
        QT_MOC_LITERAL(405, 12),  // "updateGanado"
        QT_MOC_LITERAL(418, 12),  // "deleteGanado"
        QT_MOC_LITERAL(431, 12),  // "getAllGanado"
        QT_MOC_LITERAL(444, 19),  // "validarProgenitores"
        QT_MOC_LITERAL(464, 15),  // "fechaNacimiento"
        QT_MOC_LITERAL(480, 7),  // "idPadre"
        QT_MOC_LITERAL(488, 7),  // "idMadre"
        QT_MOC_LITERAL(496, 13),  // "getFotoBase64"
        QT_MOC_LITERAL(510, 10),  // "updateFoto"
        QT_MOC_LITERAL(521, 10),  // "base64Data"
        QT_MOC_LITERAL(532, 17),  // "leerArchivoBase64"
        QT_MOC_LITERAL(550, 8),  // "filePath"
        QT_MOC_LITERAL(559, 13),  // "getProduccion"
        QT_MOC_LITERAL(573, 12),  // "updatePrenez"
        QT_MOC_LITERAL(586, 6),  // "prenez"
        QT_MOC_LITERAL(593, 12),  // "updateOrdeno"
        QT_MOC_LITERAL(606, 6),  // "ordeno"
        QT_MOC_LITERAL(613, 8),  // "addParto"
        QT_MOC_LITERAL(622, 7),  // "partoId"
        QT_MOC_LITERAL(630, 5),  // "fecha"
        QT_MOC_LITERAL(636, 11),  // "updateParto"
        QT_MOC_LITERAL(648, 11),  // "deleteParto"
        QT_MOC_LITERAL(660, 16),  // "addRegistroLeche"
        QT_MOC_LITERAL(677, 5),  // "valor"
        QT_MOC_LITERAL(683, 19),  // "updateRegistroLeche"
        QT_MOC_LITERAL(703, 13),  // "fechaOriginal"
        QT_MOC_LITERAL(717, 10),  // "fechaNueva"
        QT_MOC_LITERAL(728, 10),  // "nuevoValor"
        QT_MOC_LITERAL(739, 19),  // "deleteRegistroLeche"
        QT_MOC_LITERAL(759, 16),  // "addRegistroCarne"
        QT_MOC_LITERAL(776, 19),  // "updateRegistroCarne"
        QT_MOC_LITERAL(796, 19),  // "deleteRegistroCarne"
        QT_MOC_LITERAL(816, 11),  // "getEspecies"
        QT_MOC_LITERAL(828, 18),  // "getRazasPorEspecie"
        QT_MOC_LITERAL(847, 7),  // "especie"
        QT_MOC_LITERAL(855, 8),  // "getSexos"
        QT_MOC_LITERAL(864, 10),  // "getEstados"
        QT_MOC_LITERAL(875, 13),  // "currentScreen"
        QT_MOC_LITERAL(889, 10),  // "isLoggedIn"
        QT_MOC_LITERAL(900, 8)   // "userName"
    },
    "Presentation::AppViewModel",
    "currentScreenChanged",
    "",
    "sessionChanged",
    "errorOccurred",
    "message",
    "goToLogin",
    "goToRegister",
    "goToMenu",
    "goToInventario",
    "goToRegistroReproductivo",
    "goToRegistroLeche",
    "goToRegistroPeso",
    "login",
    "email",
    "contrasena",
    "createAccount",
    "nombre",
    "logout",
    "getFincas",
    "getFinca",
    "id",
    "createFinca",
    "nHectareas",
    "nPotreros",
    "capacidad",
    "updateFinca",
    "deleteFinca",
    "fincaTieneAnimales",
    "idFinca",
    "getGanadoByFinca",
    "getGanado",
    "createGanado",
    "data",
    "updateGanado",
    "deleteGanado",
    "getAllGanado",
    "validarProgenitores",
    "fechaNacimiento",
    "idPadre",
    "idMadre",
    "getFotoBase64",
    "updateFoto",
    "base64Data",
    "leerArchivoBase64",
    "filePath",
    "getProduccion",
    "updatePrenez",
    "prenez",
    "updateOrdeno",
    "ordeno",
    "addParto",
    "partoId",
    "fecha",
    "updateParto",
    "deleteParto",
    "addRegistroLeche",
    "valor",
    "updateRegistroLeche",
    "fechaOriginal",
    "fechaNueva",
    "nuevoValor",
    "deleteRegistroLeche",
    "addRegistroCarne",
    "updateRegistroCarne",
    "deleteRegistroCarne",
    "getEspecies",
    "getRazasPorEspecie",
    "especie",
    "getSexos",
    "getEstados",
    "currentScreen",
    "isLoggedIn",
    "userName"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_Presentation__AppViewModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   14, // methods
       3,  453, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  284,    2, 0x06,    4 /* Public */,
       3,    0,  285,    2, 0x06,    5 /* Public */,
       4,    1,  286,    2, 0x06,    6 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  289,    2, 0x02,    8 /* Public */,
       7,    0,  290,    2, 0x02,    9 /* Public */,
       8,    0,  291,    2, 0x02,   10 /* Public */,
       9,    0,  292,    2, 0x02,   11 /* Public */,
      10,    0,  293,    2, 0x02,   12 /* Public */,
      11,    0,  294,    2, 0x02,   13 /* Public */,
      12,    0,  295,    2, 0x02,   14 /* Public */,
      13,    2,  296,    2, 0x02,   15 /* Public */,
      16,    3,  301,    2, 0x02,   18 /* Public */,
      18,    0,  308,    2, 0x02,   22 /* Public */,
      19,    0,  309,    2, 0x02,   23 /* Public */,
      20,    1,  310,    2, 0x02,   24 /* Public */,
      22,    4,  313,    2, 0x02,   26 /* Public */,
      26,    5,  322,    2, 0x02,   31 /* Public */,
      27,    1,  333,    2, 0x02,   37 /* Public */,
      28,    1,  336,    2, 0x02,   39 /* Public */,
      30,    1,  339,    2, 0x02,   41 /* Public */,
      31,    1,  342,    2, 0x02,   43 /* Public */,
      32,    1,  345,    2, 0x02,   45 /* Public */,
      34,    1,  348,    2, 0x02,   47 /* Public */,
      35,    1,  351,    2, 0x02,   49 /* Public */,
      36,    0,  354,    2, 0x02,   51 /* Public */,
      37,    3,  355,    2, 0x02,   52 /* Public */,
      41,    1,  362,    2, 0x02,   56 /* Public */,
      42,    2,  365,    2, 0x02,   58 /* Public */,
      44,    1,  370,    2, 0x02,   61 /* Public */,
      46,    1,  373,    2, 0x02,   63 /* Public */,
      47,    2,  376,    2, 0x02,   65 /* Public */,
      49,    2,  381,    2, 0x02,   68 /* Public */,
      51,    3,  386,    2, 0x02,   71 /* Public */,
      54,    3,  393,    2, 0x02,   75 /* Public */,
      55,    2,  400,    2, 0x02,   79 /* Public */,
      56,    3,  405,    2, 0x02,   82 /* Public */,
      58,    4,  412,    2, 0x02,   86 /* Public */,
      62,    2,  421,    2, 0x02,   91 /* Public */,
      63,    3,  426,    2, 0x02,   94 /* Public */,
      64,    4,  433,    2, 0x02,   98 /* Public */,
      65,    2,  442,    2, 0x02,  103 /* Public */,
      66,    0,  447,    2, 0x02,  106 /* Public */,
      67,    1,  448,    2, 0x02,  107 /* Public */,
      69,    0,  451,    2, 0x02,  109 /* Public */,
      70,    0,  452,    2, 0x02,  110 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   14,   15,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   17,   14,   15,
    QMetaType::Void,
    QMetaType::QVariantList,
    QMetaType::QVariantMap, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   17,   23,   24,   25,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,   21,   17,   23,   24,   25,
    QMetaType::Bool, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QString,   29,
    QMetaType::QVariantList, QMetaType::QString,   29,
    QMetaType::QVariantMap, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QVariantMap,   33,
    QMetaType::Bool, QMetaType::QVariantMap,   33,
    QMetaType::Bool, QMetaType::QString,   21,
    QMetaType::QVariantList,
    QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   38,   39,   40,
    QMetaType::QString, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   21,   43,
    QMetaType::QString, QMetaType::QString,   45,
    QMetaType::QVariantMap, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   21,   48,
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool,   21,   50,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   21,   52,   53,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   21,   52,   53,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   21,   52,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::Double,   21,   53,   57,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Double,   21,   59,   60,   61,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   21,   53,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::Double,   21,   53,   57,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Double,   21,   59,   60,   61,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   21,   53,
    QMetaType::QStringList,
    QMetaType::QStringList, QMetaType::QString,   68,
    QMetaType::QStringList,
    QMetaType::QStringList,

 // properties: name, type, flags
      71, QMetaType::QString, 0x00015001, uint(0), 0,
      72, QMetaType::Bool, 0x00015001, uint(1), 0,
      73, QMetaType::QString, 0x00015001, uint(1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject Presentation::AppViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Presentation__AppViewModel.offsetsAndSizes,
    qt_meta_data_Presentation__AppViewModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_Presentation__AppViewModel_t,
        // property 'currentScreen'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'isLoggedIn'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'userName'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AppViewModel, std::true_type>,
        // method 'currentScreenChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sessionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorOccurred'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'goToLogin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToRegister'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToInventario'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToRegistroReproductivo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToRegistroLeche'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goToRegistroPeso'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'login'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createAccount'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'logout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'getFincas'
        QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>,
        // method 'getFinca'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createFinca'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateFinca'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'deleteFinca'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'fincaTieneAnimales'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getGanadoByFinca'
        QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getGanado'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createGanado'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'updateGanado'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'deleteGanado'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getAllGanado'
        QtPrivate::TypeAndForceComplete<QVariantList, std::false_type>,
        // method 'validarProgenitores'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getFotoBase64'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateFoto'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'leerArchivoBase64'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getProduccion'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updatePrenez'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateOrdeno'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'addParto'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateParto'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'deleteParto'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addRegistroLeche'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateRegistroLeche'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'deleteRegistroLeche'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addRegistroCarne'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'updateRegistroCarne'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'deleteRegistroCarne'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getEspecies'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'getRazasPorEspecie'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'getSexos'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'getEstados'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>
    >,
    nullptr
} };

void Presentation::AppViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AppViewModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->currentScreenChanged(); break;
        case 1: _t->sessionChanged(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->goToLogin(); break;
        case 4: _t->goToRegister(); break;
        case 5: _t->goToMenu(); break;
        case 6: _t->goToInventario(); break;
        case 7: _t->goToRegistroReproductivo(); break;
        case 8: _t->goToRegistroLeche(); break;
        case 9: _t->goToRegistroPeso(); break;
        case 10: { bool _r = _t->login((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->createAccount((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->logout(); break;
        case 13: { QVariantList _r = _t->getFincas();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: { QVariantMap _r = _t->getFinca((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->createFinca((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->updateFinca((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->deleteFinca((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { bool _r = _t->fincaTieneAnimales((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 19: { QVariantList _r = _t->getGanadoByFinca((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 20: { QVariantMap _r = _t->getGanado((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 21: { bool _r = _t->createGanado((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 22: { bool _r = _t->updateGanado((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 23: { bool _r = _t->deleteGanado((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 24: { QVariantList _r = _t->getAllGanado();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 25: { QString _r = _t->validarProgenitores((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 26: { QString _r = _t->getFotoBase64((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 27: { bool _r = _t->updateFoto((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 28: { QString _r = _t->leerArchivoBase64((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 29: { QVariantMap _r = _t->getProduccion((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 30: { bool _r = _t->updatePrenez((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 31: { bool _r = _t->updateOrdeno((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 32: { bool _r = _t->addParto((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 33: { bool _r = _t->updateParto((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 34: { bool _r = _t->deleteParto((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 35: { bool _r = _t->addRegistroLeche((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 36: { bool _r = _t->updateRegistroLeche((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 37: { bool _r = _t->deleteRegistroLeche((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 38: { bool _r = _t->addRegistroCarne((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 39: { bool _r = _t->updateRegistroCarne((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 40: { bool _r = _t->deleteRegistroCarne((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 41: { QStringList _r = _t->getEspecies();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 42: { QStringList _r = _t->getRazasPorEspecie((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 43: { QStringList _r = _t->getSexos();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 44: { QStringList _r = _t->getEstados();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AppViewModel::*)();
            if (_t _q_method = &AppViewModel::currentScreenChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AppViewModel::*)();
            if (_t _q_method = &AppViewModel::sessionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AppViewModel::*)(const QString & );
            if (_t _q_method = &AppViewModel::errorOccurred; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<AppViewModel *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->currentScreen(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->isLoggedIn(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->userName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *Presentation::AppViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Presentation::AppViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Presentation__AppViewModel.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Presentation::AppViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 45)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 45;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Presentation::AppViewModel::currentScreenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Presentation::AppViewModel::sessionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Presentation::AppViewModel::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
