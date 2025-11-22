#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "quizlistwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    // simple in-memory storage; path/version don't really matter yet
    m_storage("./data", "1.0"),
    m_auth(m_storage),
    m_mainMenuController(m_storage)
{
    ui->setupUi(this);

    m_quizListWindow = new QuizListWindow(nullptr);

    // start logged out: show loginPage, hide menuPage
    ui->loginPage->show();

    // Connect buttons to our slots
    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::handleLogin);

    connect(ui->createAccountButton, &QPushButton::clicked,
            this, &MainWindow::handleCreateAccount);

    connect(m_quizListWindow, &QuizListWindow::createQuizRequested,
            this, &MainWindow::handleCreateQuiz);

    connect(m_quizListWindow, &QuizListWindow::playQuizRequested,
            this, &MainWindow::handlePlayQuiz);

    connect(m_quizListWindow, &QuizListWindow::editQuizRequested,
            this, &MainWindow::handleEditQuiz);


    ui->statusLabel->setText("Not logged in");


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::handleLogin()
{
    const std::string username = ui->usernameEdit->text().toStdString();
    const std::string password = ui->passwordEdit->text().toStdString();

    auto user = m_auth.login(username, password);
    if (user)
    {
        // store a copy of the logged-in user
        m_currentUser = *user;

//        ui->statusLabel->setText(
//            QString("Logged in as %1").arg(QString::fromStdString(username)));

        //setting up the current user as a parameter to show the users list
        m_quizListWindow->setUser(*m_currentUser);
        m_quizListWindow->show();
        // This prints info from your backend to the console for now
        m_mainMenuController.showMenu(*m_currentUser);
    }
    else
    {
        ui->statusLabel->setText("Login failed");
        QMessageBox::warning(this, tr("Login failed"),
                             tr("Invalid username or password."));
    }
}

void MainWindow::handleCreateAccount()
{
    const std::string username = ui->usernameEdit->text().toStdString();
    const std::string password = ui->passwordEdit->text().toStdString();

    if (username.empty() || password.empty())
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Username and password cannot be empty."));
        return;
    }

    if (m_auth.userExists(username))
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("User already exists."));
        return;
    }

    User u = m_auth.createAccount(username, password);
    m_currentUser = u;

    //ui->statusLabel->setText(
        //QString("Account created and logged in as %1")
           // .arg(QString::fromStdString(username)));

    m_quizListWindow->setUser(*m_currentUser);
    m_quizListWindow->show();

    m_mainMenuController.showMenu(*m_currentUser);
}

void MainWindow::handleCreateQuiz()
{
    QMessageBox::information(this, "Create Quiz", "TODO: open quiz editor window.");
}


void MainWindow::handlePlayQuiz(const Quiz &quiz)
{
    // For now just prove the wiring works
    qDebug() << "MainWindow: play quiz"
             << QString::fromStdString(quiz.getTitle());

    // Later:
    // m_playQuizController.startQuiz(*m_currentUser, quiz);
    // show PlayQuiz UI
}

void MainWindow::handleEditQuiz(const Quiz &quiz)
{
    qDebug() << "MainWindow: edit quiz"
             << QString::fromStdString(quiz.getTitle());

    // Later:
    // m_editQuizController.editQuiz(*m_currentUser, quiz);
    // show EditQuiz UI
}
