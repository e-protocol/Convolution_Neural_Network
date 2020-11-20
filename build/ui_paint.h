/********************************************************************************
** Form generated from reading UI file 'paint.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINT_H
#define UI_PAINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Paint
{
public:
    QGraphicsView *graphicsView;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser_3;
    QTextBrowser *textBrowser_2;
    QLCDNumber *lcdNumber;
    QTextBrowser *textBrowser_4;
    QLCDNumber *lcdNumber_2;
    QTextBrowser *textBrowser_5;

    void setupUi(QWidget *Paint)
    {
        if (Paint->objectName().isEmpty())
            Paint->setObjectName(QString::fromUtf8("Paint"));
        Paint->resize(628, 661);
        graphicsView = new QGraphicsView(Paint);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 20, 400, 400));
        pushButton_1 = new QPushButton(Paint);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(10, 430, 121, 41));
        pushButton_2 = new QPushButton(Paint);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 480, 121, 41));
        pushButton_3 = new QPushButton(Paint);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(10, 530, 121, 41));
        pushButton_4 = new QPushButton(Paint);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(10, 580, 121, 41));
        textEdit = new QTextEdit(Paint);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(310, 430, 101, 31));
        textBrowser_3 = new QTextBrowser(Paint);
        textBrowser_3->setObjectName(QString::fromUtf8("textBrowser_3"));
        textBrowser_3->setGeometry(QRect(220, 430, 81, 31));
        textBrowser_2 = new QTextBrowser(Paint);
        textBrowser_2->setObjectName(QString::fromUtf8("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(220, 470, 81, 31));
        lcdNumber = new QLCDNumber(Paint);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(310, 470, 101, 31));
        lcdNumber->setDigitCount(6);
        textBrowser_4 = new QTextBrowser(Paint);
        textBrowser_4->setObjectName(QString::fromUtf8("textBrowser_4"));
        textBrowser_4->setGeometry(QRect(220, 510, 81, 31));
        lcdNumber_2 = new QLCDNumber(Paint);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(310, 510, 101, 31));
        lcdNumber_2->setDigitCount(6);
        textBrowser_5 = new QTextBrowser(Paint);
        textBrowser_5->setObjectName(QString::fromUtf8("textBrowser_5"));
        textBrowser_5->setGeometry(QRect(420, 30, 201, 391));

        retranslateUi(Paint);

        QMetaObject::connectSlotsByName(Paint);
    } // setupUi

    void retranslateUi(QWidget *Paint)
    {
        Paint->setWindowTitle(QCoreApplication::translate("Paint", "ConvNeuron", nullptr));
        pushButton_1->setText(QCoreApplication::translate("Paint", "Clear", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Paint", "Answer", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Paint", "Train", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Paint", "Add to Learn", nullptr));
        textBrowser_3->setHtml(QCoreApplication::translate("Paint", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Answer:</p></body></html>", nullptr));
        textBrowser_2->setHtml(QCoreApplication::translate("Paint", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Epoch:</p></body></html>", nullptr));
        textBrowser_4->setHtml(QCoreApplication::translate("Paint", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Examples:</p></body></html>", nullptr));
        textBrowser_5->setHtml(QCoreApplication::translate("Paint", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Cantarell'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Paint: public Ui_Paint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINT_H
