/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *loginPage;
    QLineEdit *usernameEdit;
    QPushButton *createAccountButton;
    QPushButton *loginButton;
    QLineEdit *passwordEdit;
    QLabel *statusLabel;
    QWidget *menuPage;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(70, 40, 651, 491));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        loginPage = new QWidget(verticalLayoutWidget);
        loginPage->setObjectName("loginPage");
        usernameEdit = new QLineEdit(loginPage);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(0, 50, 649, 24));
        createAccountButton = new QPushButton(loginPage);
        createAccountButton->setObjectName("createAccountButton");
        createAccountButton->setGeometry(QRect(0, 270, 649, 24));
        loginButton = new QPushButton(loginPage);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(0, 200, 649, 24));
        passwordEdit = new QLineEdit(loginPage);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(0, 130, 649, 24));
        statusLabel = new QLabel(loginPage);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(0, 330, 351, 131));

        verticalLayout->addWidget(loginPage);

        menuPage = new QWidget(centralwidget);
        menuPage->setObjectName("menuPage");
        menuPage->setGeometry(QRect(730, 60, 120, 80));
        verticalLayoutWidget_2 = new QWidget(menuPage);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(10, 0, 160, 80));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        createAccountButton->setText(QCoreApplication::translate("MainWindow", "Create Account", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "Not logged in", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
