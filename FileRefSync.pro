#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T10:14:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileRefSync
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    uchardet/LangModels/LangArabicModel.cpp \
    uchardet/LangModels/LangBulgarianModel.cpp \
    uchardet/LangModels/LangCroatianModel.cpp \
    uchardet/LangModels/LangCzechModel.cpp \
    uchardet/LangModels/LangDanishModel.cpp \
    uchardet/LangModels/LangEsperantoModel.cpp \
    uchardet/LangModels/LangEstonianModel.cpp \
    uchardet/LangModels/LangFinnishModel.cpp \
    uchardet/LangModels/LangFrenchModel.cpp \
    uchardet/LangModels/LangGermanModel.cpp \
    uchardet/LangModels/LangGreekModel.cpp \
    uchardet/LangModels/LangHebrewModel.cpp \
    uchardet/LangModels/LangHungarianModel.cpp \
    uchardet/LangModels/LangIrishModel.cpp \
    uchardet/LangModels/LangItalianModel.cpp \
    uchardet/LangModels/LangLatvianModel.cpp \
    uchardet/LangModels/LangLithuanianModel.cpp \
    uchardet/LangModels/LangMalteseModel.cpp \
    uchardet/LangModels/LangPolishModel.cpp \
    uchardet/LangModels/LangPortugueseModel.cpp \
    uchardet/LangModels/LangRomanianModel.cpp \
    uchardet/LangModels/LangRussianModel.cpp \
    uchardet/LangModels/LangSlovakModel.cpp \
    uchardet/LangModels/LangSloveneModel.cpp \
    uchardet/LangModels/LangSpanishModel.cpp \
    uchardet/LangModels/LangSwedishModel.cpp \
    uchardet/LangModels/LangThaiModel.cpp \
    uchardet/LangModels/LangTurkishModel.cpp \
    uchardet/LangModels/LangVietnameseModel.cpp \
    uchardet/CharDistribution.cpp \
    uchardet/JpCntx.cpp \
    uchardet/nsBig5Prober.cpp \
    uchardet/nsCharSetProber.cpp \
    uchardet/nsEscCharsetProber.cpp \
    uchardet/nsEscSM.cpp \
    uchardet/nsEUCJPProber.cpp \
    uchardet/nsEUCKRProber.cpp \
    uchardet/nsEUCTWProber.cpp \
    uchardet/nsGB2312Prober.cpp \
    uchardet/nsHebrewProber.cpp \
    uchardet/nsLatin1Prober.cpp \
    uchardet/nsMBCSGroupProber.cpp \
    uchardet/nsMBCSSM.cpp \
    uchardet/nsSBCharSetProber.cpp \
    uchardet/nsSBCSGroupProber.cpp \
    uchardet/nsSJISProber.cpp \
    uchardet/nsUniversalDetector.cpp \
    uchardet/nsUTF8Prober.cpp \
    uchardet/uchardet.cpp \
    texteditor.cpp

HEADERS  += mainwindow.h \
    uchardet/uchardet.h \
    uchardet/CharDistribution.h \
    uchardet/JpCntx.h \
    uchardet/nsBig5Prober.h \
    uchardet/nsCharSetProber.h \
    uchardet/nsCodingStateMachine.h \
    uchardet/nscore.h \
    uchardet/nsEscCharsetProber.h \
    uchardet/nsEUCJPProber.h \
    uchardet/nsEUCKRProber.h \
    uchardet/nsEUCTWProber.h \
    uchardet/nsGB2312Prober.h \
    uchardet/nsHebrewProber.h \
    uchardet/nsLatin1Prober.h \
    uchardet/nsMBCSGroupProber.h \
    uchardet/nsPkgInt.h \
    uchardet/nsSBCharSetProber.h \
    uchardet/nsSBCSGroupProber.h \
    uchardet/nsSJISProber.h \
    uchardet/nsUniversalDetector.h \
    uchardet/nsUTF8Prober.h \
    uchardet/prmem.h \
    texteditor.h

FORMS    += mainwindow.ui
