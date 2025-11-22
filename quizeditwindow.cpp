#include "quizeditwindow.h"
#include "ui_quizeditwindow.h"

#include <QInputDialog>
#include <QMessageBox>

QuizEditWindow::QuizEditWindow(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::QuizEditWindow)
{
    ui->setupUi(this);
}

QuizEditWindow::~QuizEditWindow()
{
    delete ui;
}

void QuizEditWindow::setQuiz(User &user, Quiz &quiz, EditQuizController &controller)
{
    m_user = &user;
    m_quiz = &quiz;
    m_controller = &controller;

    ui->titleLineEdit->setText(
        QString::fromStdString(m_quiz->getTitle()));

    refreshQuestionList();
}

void QuizEditWindow::setQuiz(User &user, Quiz &quiz, CreateQuizController &controller)
{
    m_user = &user;
    m_quiz = &quiz;
    m_createController = &controller;

    ui->titleLineEdit->setText(
        QString::fromStdString(m_quiz->getTitle()));

    refreshQuestionList();
}


void QuizEditWindow::refreshQuestionList()
{
    ui->questionsList->clear();
    if (!m_quiz) return;

    const int count = m_quiz->questionCount();
    for (int i = 0; i < count; ++i)
    {
        const Question &q = m_quiz->getQuestion(i);
        ui->questionsList->addItem(
            QString::fromStdString(q.getPrompt()));
    }
}

// ---------- slots ----------

void QuizEditWindow::on_addQuestionButton_clicked()
{
    if (!m_quiz || !m_controller) return;

    bool ok = false;
    QString prompt = QInputDialog::getText(
        this,
        "Add Question",
        "Question text:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || prompt.trimmed().isEmpty())
        return;

    // Very simple question for now (no choices yet)
    Question q(prompt.trimmed().toStdString());
    m_controller->addQuestion(*m_quiz, q);

    refreshQuestionList();
}

void QuizEditWindow::on_removeQuestionButton_clicked()
{
    if (!m_quiz || !m_controller) return;

    int row = ui->questionsList->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Remove Question",
                             "Please select a question to remove.");
        return;
    }

    m_controller->removeQuestion(*m_quiz, row);
    refreshQuestionList();
}

void QuizEditWindow::on_doneButton_clicked()
{
    if (!m_quiz || !m_user) {
        close();
        return;
    }

    m_quiz->setTitle(ui->titleLineEdit->text().toStdString());

    if (m_controller) {
        m_controller->saveChanges(*m_user);   // editing mode
    }
    else if (m_createController) {
        m_createController->saveAndFinish();  // creating mode
    }

    accept();
}
