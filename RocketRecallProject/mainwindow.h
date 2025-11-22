//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

//#include <QMainWindow>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

//class MainWindow : public QMainWindow
//{
//    Q_OBJECT

//public:
//    MainWindow(QWidget *parent = nullptr);
//    ~MainWindow();

//private:
//    Ui::MainWindow *ui;
//};
//#endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <optional>

#include "QuizApp.h"   // your backend classes
#include "quizlistwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//class QuizListWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin();
    void handleCreateAccount();
    void handleCreateQuiz();
    void handlePlayQuiz(const Quiz &quiz);
    void handleEditQuiz(const Quiz &quiz);


private:
    Ui::MainWindow *ui;


    // Backend objects
    StorageService      m_storage;
    AuthService         m_auth;
    MainMenuController  m_mainMenuController;
    PlayQuizController  m_playQuizController;
    EditQuizController  m_editQuizController;

    QuizListWindow *m_quizListWindow = nullptr;

    std::optional<User> m_currentUser;
};

#endif // MAINWINDOW_H
