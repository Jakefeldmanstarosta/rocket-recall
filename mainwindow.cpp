#include "mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_storage("./data", "1.0"),
    m_auth(m_storage),
    m_mainMenuController(m_storage)
{
    stacked = new QStackedWidget(this);
    setCentralWidget(stacked);

    // --- Login Page ---
    loginPage = new QWidget();
    auto *loginLayout = new QVBoxLayout(loginPage);

    username = new QLineEdit();
    password = new QLineEdit();
    password->setEchoMode(QLineEdit::Password);

    auto *loginBtn = new QPushButton("Login");
    auto *createBtn = new QPushButton("Create Account");
    status = new QLabel();

    loginLayout->addWidget(new QLabel("Username:"));
    loginLayout->addWidget(username);
    loginLayout->addWidget(new QLabel("Password:"));
    loginLayout->addWidget(password);
    loginLayout->addWidget(loginBtn);
    loginLayout->addWidget(createBtn);
    loginLayout->addWidget(status);

    // --- Quiz List Page ---
    quizListPage = new QuizListWindow();

    // add to stacked widget
    stacked->addWidget(loginPage);      // index 0
    stacked->addWidget(quizListPage);   // index 1
    stacked->setCurrentIndex(0);

    // connect signals
    connect(loginBtn, &QPushButton::clicked,
            this, &MainWindow::handleLogin);

    connect(createBtn, &QPushButton::clicked,
            this, &MainWindow::handleCreate);

    connect(quizListPage, &QuizListWindow::createQuizRequested,
            this, &MainWindow::handleCreateQuiz);

    connect(quizListPage, &QuizListWindow::playQuizRequested,
            this, &MainWindow::handlePlayQuiz);

    connect(quizListPage, &QuizListWindow::editQuizRequested,
            this, &MainWindow::handleEditQuiz);
}

void MainWindow::handleLogin()
{
    auto user = m_auth.login(
        username->text().toStdString(),
        password->text().toStdString());

    if (!user)
    {
        status->setText("Login failed.");
        return;
    }

    m_currentUser = *user;
    quizListPage->setUser(*m_currentUser);

    stacked->setCurrentIndex(1);
    m_mainMenuController.showMenu(*m_currentUser);
}

void MainWindow::handleCreate()
{
    std::string u = username->text().toStdString();
    std::string p = password->text().toStdString();

    if (u.empty() || p.empty())
    {
        status->setText("Invalid.");
        return;
    }

    if (m_auth.userExists(u))
    {
        status->setText("User exists.");
        return;
    }

    m_currentUser = m_auth.createAccount(u, p);
    quizListPage->setUser(*m_currentUser);

    stacked->setCurrentIndex(1);
    m_mainMenuController.showMenu(*m_currentUser);
}

void MainWindow::handleCreateQuiz()
{
    QMessageBox::information(this, "TODO",
                             "Create quiz editor not implemented.");
}

void MainWindow::handlePlayQuiz(const Quiz &quiz)
{
    qDebug() << "Play quiz:"
             << QString::fromStdString(quiz.getTitle());
}

void MainWindow::handleEditQuiz(const Quiz &quiz)
{
    qDebug() << "Edit quiz:"
             << QString::fromStdString(quiz.getTitle());
}
