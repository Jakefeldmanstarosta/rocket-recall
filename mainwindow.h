#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QStackedWidget>
#include <optional>

#include "QuizApp.h"
#include "quizlistwindow.h"
#include "PlayQuizWindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleLogin();
    void handleCreate();
    void handleCreateQuiz();
    void handlePlayQuiz(const Quiz &quiz);
    void handleEditQuiz(const Quiz &quiz);

private:
    QStackedWidget *stacked;

    QWidget *loginPage;
    QuizListWindow *quizListPage;

    QLineEdit *username;
    QLineEdit *password;
    QLabel *status;

    PlayQuizWindow *playPage = nullptr;

    StorageService m_storage;
    AuthService m_auth;
    MainMenuController m_mainMenuController;
    PlayQuizController m_playQuizController;
    EditQuizController m_editQuizController;

    std::optional<User> m_currentUser;
};
