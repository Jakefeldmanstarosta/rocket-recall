#include "mainwindow.h"
#include "quizeditwindow.h"

#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

// NEW
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent),
    m_storage("./data", "1.0"),
    m_auth(m_storage),
    m_mainMenuController(m_storage),
    m_createQuizController(m_storage)
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
    stacked->addWidget(loginPage);    // index 0
    stacked->addWidget(quizListPage); // index 1
    stacked->setCurrentIndex(0);

    // connect signals
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::handleLogin);

    connect(createBtn, &QPushButton::clicked, this, &MainWindow::handleCreate);

    connect(quizListPage, &QuizListWindow::createQuizRequested, this, &MainWindow::handleCreateQuiz);

    connect(quizListPage, &QuizListWindow::playQuizRequested, this, &MainWindow::handlePlayQuiz);

    connect(quizListPage, &QuizListWindow::editQuizRequested, this, &MainWindow::handleEditQuiz);
}
void MainWindow::handleLogin()
{
    const std::string u = username->text().toStdString();
    const std::string p = password->text().toStdString();

    std::ifstream file("db.csv");
    if (!file.is_open())
    {
        status->setText("Login failed: db.csv not found.");
        QMessageBox::warning(this, "Login failed",
                             "User database file db.csv could not be opened.");
        return;
    }

    std::string line;
    bool first = true;
    bool ok = false;

    while (std::getline(file, line))
    {
        if (first)
        {
            first = false; // skip header row
            continue;
        }

        std::stringstream ss(line);
        std::string fileUser, filePass;
        std::getline(ss, fileUser, ',');
        std::getline(ss, filePass, ',');

        if (fileUser == u && filePass == p)
        {
            ok = true;
            break;
        }
    }

    if (!ok)
    {
        status->setText("Login failed.");
        return;
    }

    // Build user from CSV login
    m_currentUser = User(u, p);

    // --- TEMP TEST QUIZ ---
    if (m_currentUser->getQuizzes().empty()) {
        Quiz q(1, "Test Quiz");
        Question a("What is 2 + 2?");
        a.addChoice(Choice("A", "3"));
        a.addChoice(Choice("B", "4"));
        a.addChoice(Choice("C", "5"));
        a.setCorrectIndex(1);

        Question b("Capital of France?");
        b.addChoice(Choice("A", "Berlin"));
        b.addChoice(Choice("B", "Rome"));
        b.addChoice(Choice("C", "Paris"));
        b.setCorrectIndex(2);

        q.addQuestion(a);
        q.addQuestion(b);

        m_currentUser->addQuiz(q);

        m_storage.saveUser(*m_currentUser);
    }

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

    bool existed = false;
    {
        std::ifstream check("db.csv");
        existed = check.good();
    }

    if (existed)
    {
        std::ifstream file("db.csv");
        if (file.is_open())
        {
            std::string line;
            bool first = true;
            while (std::getline(file, line))
            {
                if (first)
                {
                    first = false; // skip header
                    continue;
                }

                std::stringstream ss(line);
                std::string fileUser, filePass;
                std::getline(ss, fileUser, ',');
                std::getline(ss, filePass, ',');

                if (fileUser == u)
                {
                    status->setText("User exists.");
                    return;
                }
            }
        }
    }

    std::ofstream out("db.csv", std::ios::app);
    if (!out.is_open())
    {
        status->setText("Error creating account.");
        return;
    }

    if (!existed)
    {
        out << "Username,Password\n";
    }
    out << u << "," << p << "\n";

    // Newly created user
    m_currentUser = User(u, p);

    // --- TEMP TEST QUIZ ---
    if (m_currentUser->getQuizzes().empty()) {
        Quiz q(1, "Test Quiz");
        Question a("What is 2 + 2?");
        a.addChoice(Choice("A", "3"));
        a.addChoice(Choice("B", "4"));
        a.addChoice(Choice("C", "5"));
        a.setCorrectIndex(1);

        Question b("Capital of France?");
        b.addChoice(Choice("A", "Berlin"));
        b.addChoice(Choice("B", "Rome"));
        b.addChoice(Choice("C", "Paris"));
        b.setCorrectIndex(2);

        q.addQuestion(a);
        q.addQuestion(b);

        m_currentUser->addQuiz(q);

        m_storage.saveUser(*m_currentUser);
    }

    quizListPage->setUser(*m_currentUser);

    stacked->setCurrentIndex(1);
    m_mainMenuController.showMenu(*m_currentUser);
}


void MainWindow::handleCreateQuiz()
{
    if (!m_currentUser) return;

    // 1) Start a brand-new quiz for this user
    m_createQuizController.startNewQuiz(*m_currentUser, "Untitled quiz");

    Quiz *newQuiz = m_createQuizController.getQuiz();
    if (!newQuiz) return;

    // 2) Open the edit window
    QuizEditWindow dlg(this);
    dlg.setQuiz(*m_currentUser, *newQuiz, m_createQuizController);

    // 3) After Done/Close
    if (dlg.exec() == QDialog::Accepted)
    {
        // ✅ Refresh the quiz list UI
        quizListPage->setUser(*m_currentUser);
        quizListPage->populateList();

        // ✅ Return to quiz list page (main menu)
        stacked->setCurrentIndex(1);
    }
}




void MainWindow::handlePlayQuiz(const Quiz &quiz)
{
    if (playPage)
        stacked->removeWidget(playPage);

    playPage = new PlayQuizWindow(quiz, this);
    stacked->addWidget(playPage);
    stacked->setCurrentWidget(playPage);
}

void MainWindow::handleEditQuiz(const Quiz &quiz)
{
    if (!m_currentUser) return;

    // Find the modifiable quiz object in the current user
    Quiz *editable = m_currentUser->findQuizById(quiz.getId());
    if (!editable) return;

    auto *dlg = new QuizEditWindow(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setQuiz(*m_currentUser, *editable, m_editQuizController);

    dlg->show();
}
