#include "QuizApp.h"

#include <algorithm>
#include <iostream>
#include <random>

// -------------------- Choice -------------------- //

Choice::Choice(const std::string &label, const std::string &text)
    : m_label(label)
    , m_text(text)
{}

const std::string &Choice::getLabel() const
{
    return m_label;
}
const std::string &Choice::getText() const
{
    return m_text;
}

void Choice::setLabel(const std::string &label)
{
    m_label = label;
}
void Choice::setText(const std::string &text)
{
    m_text = text;
}

// -------------------- Question -------------------- //

Question::Question(const std::string &prompt)
    : m_prompt(prompt)
{}

const std::string &Question::getPrompt() const
{
    return m_prompt;
}
void Question::setPrompt(const std::string &prompt)
{
    m_prompt = prompt;
}

void Question::addChoice(const Choice &choice)
{
    m_choices.push_back(choice);
    if (static_cast<int>(m_stats.size()) < static_cast<int>(m_choices.size()))
        m_stats.resize(m_choices.size(), 0);
}

int Question::choiceCount() const
{
    return static_cast<int>(m_choices.size());
}

const Choice &Question::getChoice(int idx) const
{
    return m_choices.at(idx);
}

Choice &Question::getChoice(int idx)
{
    return m_choices.at(idx);
}

void Question::setCorrectIndex(int idx)
{
    m_correctIdx = idx;
}

int Question::getCorrectIndex() const
{
    return m_correctIdx;
}

void Question::addStat(int choiceIdx)
{
    if (choiceIdx < 0)
        return;
    if (choiceIdx >= static_cast<int>(m_stats.size()))
        m_stats.resize(choiceIdx + 1, 0);
    m_stats[choiceIdx]++;
}

int Question::returnStats(int choiceIdx) const
{
    if (choiceIdx < 0 || choiceIdx >= static_cast<int>(m_stats.size()))
        return 0;
    return m_stats[choiceIdx];
}

bool Question::isCorrect(int choiceIdx) const
{
    return choiceIdx == m_correctIdx;
}

// -------------------- Attempt -------------------- //

Attempt::Attempt(const DateTime &timestamp,
                 int score,
                 Duration elapsed,
                 const std::vector<int> &wrongQuestionIds)
    : m_timestamp(timestamp)
    , m_score(score)
    , m_elapsed(elapsed)
    , m_wrongQuestionIds(wrongQuestionIds)
{}

const DateTime &Attempt::getTimestamp() const
{
    return m_timestamp;
}
int Attempt::getScore() const
{
    return m_score;
}
Duration Attempt::getElapsed() const
{
    return m_elapsed;
}
const std::vector<int> &Attempt::getWrongQuestionIds() const
{
    return m_wrongQuestionIds;
}

// -------------------- QuizStats -------------------- //

void QuizStats::recordAttempt(const Attempt &a)
{
    m_history.push_back(a);
    m_attempts++;
    if (a.getScore() > m_bestScore)
        m_bestScore = a.getScore();
    if (a.getElapsed() < m_bestTime)
        m_bestTime = a.getElapsed();
}

int QuizStats::getAttempts() const
{
    return m_attempts;
}
int QuizStats::topScore() const
{
    return m_bestScore;
}
Duration QuizStats::topSpeed() const
{
    return m_bestTime;
}
const std::vector<Attempt> &QuizStats::getHistory() const
{
    return m_history;
}

// -------------------- Quiz -------------------- //

Quiz::Quiz(int id, const std::string &title)
    : m_id(id)
    , m_title(title)
{}

int Quiz::getId() const
{
    return m_id;
}
const std::string &Quiz::getTitle() const
{
    return m_title;
}
void Quiz::setTitle(const std::string &title)
{
    m_title = title;
}

void Quiz::addQuestion(const Question &q)
{
    m_questions.push_back(q);
}

bool Quiz::removeQuestion(int idx)
{
    if (idx < 0 || idx >= static_cast<int>(m_questions.size()))
        return false;
    m_questions.erase(m_questions.begin() + idx);
    return true;
}

bool Quiz::updateQuestion(int idx, const Question &q)
{
    if (idx < 0 || idx >= static_cast<int>(m_questions.size()))
        return false;
    m_questions[idx] = q;
    return true;
}

int Quiz::questionCount() const
{
    return static_cast<int>(m_questions.size());
}

const Question &Quiz::getQuestion(int idx) const
{
    return m_questions.at(idx);
}

Question &Quiz::getQuestion(int idx)
{
    return m_questions.at(idx);
}

std::vector<int> Quiz::shuffle(OrderStrategy &orderer) const
{
    return orderer.order(*this);
}

// -------------------- User -------------------- //

User::User(const std::string &username, const std::string &passwordHash)
    : m_username(username)
    , m_passwordHash(passwordHash)
{}

const std::string &User::getName() const
{
    return m_username;
}

bool User::verifyPassword(const std::string &pw) const
{
    // In a real app you would hash + compare; here we just compare directly.
    return pw == m_passwordHash;
}

void User::addQuiz(const Quiz &q)
{
    m_quizzes.push_back(q);
}

bool User::deleteQuiz(int id)
{
    auto it = std::remove_if(m_quizzes.begin(), m_quizzes.end(), [id](const Quiz &q) {
        return q.getId() == id;
    });
    if (it == m_quizzes.end())
        return false;
    m_quizzes.erase(it, m_quizzes.end());
    m_statsByQuiz.erase(id);
    return true;
}

Quiz *User::findQuizById(int id)
{
    for (auto &q : m_quizzes) {
        if (q.getId() == id)
            return &q;
    }
    return nullptr;
}

const Quiz *User::findQuizById(int id) const
{
    for (const auto &q : m_quizzes) {
        if (q.getId() == id)
            return &q;
    }
    return nullptr;
}

std::vector<Quiz> &User::getQuizzes()
{
    return m_quizzes;
}
const std::vector<Quiz> &User::getQuizzes() const
{
    return m_quizzes;
}

std::map<int, QuizStats> &User::getStatsByQuiz()
{
    return m_statsByQuiz;
}
const std::map<int, QuizStats> &User::getStatsByQuiz() const
{
    return m_statsByQuiz;
}

// -------------------- OrderStrategy implementations -------------------- //

std::vector<int> RandomOrder::order(const Quiz &quiz)
{
    int n = quiz.questionCount();
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i)
        indices[i] = i;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);
    return indices;
}

std::vector<int> DifficultyOrder::order(const Quiz &quiz)
{
    // Very simple heuristic: questions are returned in their natural order.
    // You could inspect question stats to order by hardest/easiest.
    int n = quiz.questionCount();
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i)
        indices[i] = i;
    return indices;
}

// -------------------- QuizSession -------------------- //

QuizSession::QuizSession(const Quiz &quiz)
    : m_quiz(&quiz)
{}

void QuizSession::start(OrderStrategy &orderer)
{
    if (!m_quiz)
        return;
    m_order = orderer.order(*m_quiz);
    m_currentIdx = 0;
    m_score = 0;
    m_startTime = std::chrono::system_clock::now();
    m_endTime.reset();

    m_responses.assign(m_order.size(), -1);
}

void QuizSession::selectAnswer(int choiceIdx)
{
    if (!m_quiz || m_order.empty())
        return;

    if (m_currentIdx < 0 || m_currentIdx >= static_cast<int>(m_order.size()))
        return;

    int qIdx = m_order[m_currentIdx];
    const Question &q = m_quiz->getQuestion(qIdx);

    // Update score (re-score this question in case user changes answer)
    if (m_responses[m_currentIdx] != -1) {
        // subtract previous contribution
        if (q.isCorrect(m_responses[m_currentIdx]))
            m_score--;
    }

    m_responses[m_currentIdx] = choiceIdx;

    if (q.isCorrect(choiceIdx))
        m_score++;
}

bool QuizSession::next()
{
    if (m_currentIdx + 1 < static_cast<int>(m_order.size())) {
        ++m_currentIdx;
        return true;
    }
    return false;
}

bool QuizSession::prev()
{
    if (m_currentIdx > 0) {
        --m_currentIdx;
        return true;
    }
    return false;
}

bool QuizSession::finished() const
{
    return m_endTime.has_value();
}

Duration QuizSession::elapsed() const
{
    DateTime end = m_endTime.value_or(std::chrono::system_clock::now());
    return std::chrono::duration_cast<Duration>(end - m_startTime);
}

Attempt QuizSession::computeSummary() const
{
    if (!m_quiz)
        return Attempt();

    std::vector<int> wrongIds;
    for (std::size_t i = 0; i < m_order.size(); ++i) {
        int qIdx = m_order[i];
        const Question &q = m_quiz->getQuestion(qIdx);
        int response = m_responses[i];
        if (response == -1 || !q.isCorrect(response))
            wrongIds.push_back(qIdx);
    }

    DateTime now = std::chrono::system_clock::now();
    return Attempt(now, m_score, elapsed(), wrongIds);
}

int QuizSession::currentQuestionIndex() const
{
    return m_currentIdx;
}

int QuizSession::currentQuestionId() const
{
    if (m_order.empty())
        return -1;
    return m_order[m_currentIdx];
}

int QuizSession::getScore() const
{
    return m_score;
}

const Quiz &QuizSession::getQuiz() const
{
    return *m_quiz;
}

// -------------------- StorageService -------------------- //

StorageService::StorageService(const std::string &rootPath, const std::string &fileFormatVersion)
    : m_rootPath(rootPath)
    , m_fileFormatVersion(fileFormatVersion)
{}

const std::string &StorageService::getRootPath() const
{
    return m_rootPath;
}

const std::string &StorageService::getFileFormatVersion() const
{
    return m_fileFormatVersion;
}

// For now, keep everything in memory. You can replace this later with
// real file I/O using QFile / QTextStream, JSON, etc.
std::vector<User> StorageService::loadUsers()
{
    return m_cachedUsers;
}

void StorageService::saveUsers(const std::vector<User> &users)
{
    m_cachedUsers = users;
}

std::optional<User> StorageService::loadUser(const std::string &username)
{
    for (const auto &u : m_cachedUsers) {
        if (u.getName() == username)
            return u;
    }
    return std::nullopt;
}

void StorageService::saveUser(const User &user)
{
    for (auto &u : m_cachedUsers) {
        if (u.getName() == user.getName()) {
            u = user;
            return;
        }
    }
    m_cachedUsers.push_back(user);
}

bool StorageService::exportQuiz(const User & /*user*/, int /*quizId*/, const std::string & /*path*/)
{
    // TODO: implement real export
    return true;
}

bool StorageService::importQuiz(User & /*user*/, const std::string & /*path*/)
{
    // TODO: implement real import
    return true;
}

// -------------------- AuthService -------------------- //

AuthService::AuthService(StorageService &storage)
    : m_storage(storage)
{}

User AuthService::createAccount(const std::string &username, const std::string &password)
{
    // No hashing for now; just store password directly
    User u(username, password);
    auto users = m_storage.loadUsers();
    users.push_back(u);
    m_storage.saveUsers(users);
    return u;
}

std::optional<User> AuthService::login(const std::string &username, const std::string &password)
{
    auto optUser = m_storage.loadUser(username);
    if (!optUser)
        return std::nullopt;

    if (optUser->verifyPassword(password))
        return optUser;
    return std::nullopt;
}

bool AuthService::userExists(const std::string &username)
{
    return m_storage.loadUser(username).has_value();
}

// -------------------- Controllers -------------------- //

LoginController::LoginController(AuthService &auth)
    : m_auth(auth)
{}

void LoginController::showLoginScreen()
{
    // In a Qt UI, this would show the login dialog.
    std::cout << "[LoginController] showLoginScreen()\n";
}

void LoginController::handleLogin(const std::string &username, const std::string &password)
{
    auto user = m_auth.login(username, password);
    if (user)
        std::cout << "Login successful for " << username << "\n";
    else
        std::cout << "Login failed for " << username << "\n";
}

void LoginController::handleCreateAccount(const std::string &username, const std::string &password)
{
    if (m_auth.userExists(username)) {
        std::cout << "User already exists: " << username << "\n";
        return;
    }
    m_auth.createAccount(username, password);
    std::cout << "Account created: " << username << "\n";
}

MainMenuController::MainMenuController(StorageService &storage)
    : m_storage(storage)
{}

void MainMenuController::showMenu(User &user)
{
    std::cout << "[MainMenu] Welcome, " << user.getName() << "\n";
    std::cout << "You have " << user.getQuizzes().size() << " quizzes.\n";
}

void MainMenuController::handleCreateQuiz(User &user, const std::string &title)
{
    int id = static_cast<int>(user.getQuizzes().size()) + 1;
    user.addQuiz(Quiz(id, title));

    auto users = m_storage.loadUsers();
    // naive: overwrite same-name user
    bool updated = false;
    for (auto &u : users) {
        if (u.getName() == user.getName()) {
            u = user;
            updated = true;
            break;
        }
    }
    if (!updated)
        users.push_back(user);
    m_storage.saveUsers(users);
}

void MainMenuController::handleEditQuiz(User &user, int quizId)
{
    std::cout << "[MainMenu] Edit quiz " << quizId << "\n";
    // In a real Qt app you'd open your editor window here.
    (void) user;
}

void MainMenuController::handlePlayQuiz(User &user, int quizId, OrderStrategy &orderer)
{
    std::cout << "[MainMenu] Play quiz " << quizId << "\n";
    (void) user;
    (void) orderer;
}

void EditQuizController::showEditor(Quiz &quiz)
{
    std::cout << "[EditQuiz] Editing quiz: " << quiz.getTitle() << "\n";
}

void EditQuizController::addQuestion(Quiz &quiz, const Question &q)
{
    quiz.addQuestion(q);
}

void EditQuizController::removeQuestion(Quiz &quiz, int idx)
{
    quiz.removeQuestion(idx);
}

void EditQuizController::updateQuestion(Quiz &quiz, int idx, const Question &q)
{
    quiz.updateQuestion(idx, q);
}

void EditQuizController::saveChanges(User &user)
{
    std::cout << "[EditQuiz] Saving changes for user " << user.getName() << "\n";
}

void PlayQuizController::startSession(const Quiz &quiz, OrderStrategy &strategy)
{
    m_session.emplace(quiz);
    m_session->start(strategy);
}

void PlayQuizController::showQuestion() const
{
    if (!m_session)
        return;
    int qIdx = m_session->currentQuestionId();
    const Quiz &quiz = m_session->getQuiz();
    const Question &q = quiz.getQuestion(qIdx);

    std::cout << "[PlayQuiz] Q: " << q.getPrompt() << "\n";
}

void PlayQuizController::handleAnswer(int choiceIdx)
{
    if (!m_session)
        return;
    m_session->selectAnswer(choiceIdx);
}

void PlayQuizController::handleNext()
{
    if (!m_session)
        return;
    m_session->next();
}

void PlayQuizController::handlePrev()
{
    if (!m_session)
        return;
    m_session->prev();
}

void PlayQuizController::finishSession()
{
    if (!m_session)
        return;
    // Mark finished by setting end time
    (void) m_session->elapsed(); // just compute once
}

const std::optional<QuizSession> &PlayQuizController::getSession() const
{
    return m_session;
}

void ResultsController::showResults(const User &user, const Quiz &quiz, const Attempt &attempt)
{
    std::cout << "[Results] User: " << user.getName() << " Quiz: " << quiz.getTitle()
              << " Score: " << attempt.getScore() << "\n";
}

void ResultsController::record(const Attempt &attempt, QuizStats &stats)
{
    stats.recordAttempt(attempt);
}

void ResultsController::redo(const Quiz &quiz, OrderStrategy &orderer)
{
    QuizSession session(quiz);
    session.start(orderer);
    std::cout << "[Results] Redo quiz: " << quiz.getTitle() << "\n";
}

void ResultsController::returnToMenu(const User &user)
{
    std::cout << "[Results] Returning to menu for " << user.getName() << "\n";
}
