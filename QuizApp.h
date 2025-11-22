#ifndef QUIZAPP_H
#define QUIZAPP_H

#include <chrono>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// Simple aliases matching the UML idea of DateTime / Duration
using DateTime = std::chrono::system_clock::time_point;
using Duration = std::chrono::milliseconds;

// Forward declarations where needed
class Quiz;
class QuizStats;
class Attempt;
class OrderStrategy;

// -------------------- DOMAIN CLASSES -------------------- //

class Choice
{
public:
    Choice() = default;
    Choice(const std::string &label, const std::string &text);

    const std::string &getLabel() const;
    const std::string &getText() const;

    void setLabel(const std::string &label);
    void setText(const std::string &text);

private:
    std::string m_label;
    std::string m_text;
};

class Question
{
public:
    Question() = default;
    explicit Question(const std::string &prompt);

    const std::string &getPrompt() const;
    void setPrompt(const std::string &prompt);

    void addChoice(const Choice &choice);
    int choiceCount() const;

    const Choice &getChoice(int idx) const;
    Choice &getChoice(int idx);

    void setCorrectIndex(int idx);
    int getCorrectIndex() const;

    // stats methods
    void addStat(int choiceIdx); // increments count for that choice
    int returnStats(int choiceIdx) const;
    bool isCorrect(int choiceIdx) const;

private:
    std::string m_prompt;
    std::vector<Choice> m_choices;
    int m_correctIdx{-1};
    std::vector<int> m_stats; // parallel to m_choices
};

class Attempt
{
public:
    Attempt() = default;
    Attempt(const DateTime &timestamp,
            int score,
            Duration elapsed,
            const std::vector<int> &wrongQuestionIds);

    const DateTime &getTimestamp() const;
    int getScore() const;
    Duration getElapsed() const;
    const std::vector<int> &getWrongQuestionIds() const;

private:
    DateTime m_timestamp{};
    int m_score{0};
    Duration m_elapsed{0};
    std::vector<int> m_wrongQuestionIds;
};

class QuizStats
{
public:
    QuizStats() = default;

    void recordAttempt(const Attempt &a);

    int getAttempts() const;
    int topScore() const;
    Duration topSpeed() const;
    const std::vector<Attempt> &getHistory() const;

private:
    int m_attempts{0};
    int m_bestScore{0};
    Duration m_bestTime{Duration::max()};
    std::vector<Attempt> m_history;
};

class Quiz
{
public:
    Quiz() = default;
    Quiz(int id, const std::string &title);

    int getId() const;
    const std::string &getTitle() const;
    void setTitle(const std::string &title);

    void addQuestion(const Question &q);
    bool removeQuestion(int idx);
    bool updateQuestion(int idx, const Question &q);
    int questionCount() const;

    const Question &getQuestion(int idx) const;
    Question &getQuestion(int idx);

    // Returns a permutation of [0..questionCount()-1]
    std::vector<int> shuffle(OrderStrategy &orderer) const;

private:
    int m_id{0};
    std::string m_title;
    std::vector<Question> m_questions;
};

class User
{
public:
    User() = default;
    User(const std::string &username, const std::string &passwordHash);

    const std::string &getName() const;

    bool verifyPassword(const std::string &pw) const;

    void addQuiz(const Quiz &q);
    bool deleteQuiz(int id);
    Quiz *findQuizById(int id);
    const Quiz *findQuizById(int id) const;

    std::vector<Quiz> &getQuizzes();
    const std::vector<Quiz> &getQuizzes() const;

    std::map<int, QuizStats> &getStatsByQuiz();
    const std::map<int, QuizStats> &getStatsByQuiz() const;

private:
    std::string m_username;
    std::string m_passwordHash;
    std::vector<Quiz> m_quizzes;
    std::map<int, QuizStats> m_statsByQuiz; // key = quiz id
};

// -------------------- ORDER STRATEGIES -------------------- //

class OrderStrategy
{
public:
    virtual ~OrderStrategy() = default;

    // Given a quiz, return an order of question indices
    virtual std::vector<int> order(const Quiz &quiz) = 0;
};

class RandomOrder : public OrderStrategy
{
public:
    std::vector<int> order(const Quiz &quiz) override;
};

class DifficultyOrder : public OrderStrategy
{
public:
    std::vector<int> order(const Quiz &quiz) override;
};

// -------------------- QUIZ SESSION -------------------- //

class QuizSession
{
public:
    QuizSession() = default;
    explicit QuizSession(const Quiz &quiz);

    void start(OrderStrategy &orderer);

    void selectAnswer(int choiceIdx);
    bool next();
    bool prev();
    bool finished() const;

    Duration elapsed() const;

    Attempt computeSummary() const;

    int currentQuestionIndex() const; // index in m_order
    int currentQuestionId() const;    // actual question index in quiz
    int getScore() const;

    const Quiz &getQuiz() const;

private:
    const Quiz *m_quiz{nullptr};
    std::vector<int> m_order; // mapping session index -> quiz question index
    int m_currentIdx{0};
    int m_score{0};

    DateTime m_startTime{};
    std::optional<DateTime> m_endTime;
    std::vector<int> m_responses; // same length as m_order, -1 = unanswered
};

// -------------------- STORAGE + AUTH SERVICES -------------------- //

class StorageService
{
public:
    StorageService(const std::string &rootPath, const std::string &fileFormatVersion);

    const std::string &getRootPath() const;
    const std::string &getFileFormatVersion() const;

    // NOTE: simple, in-memory / stub implementations are provided in .cpp
    std::vector<User> loadUsers();
    void saveUsers(const std::vector<User> &users);

    std::optional<User> loadUser(const std::string &username);
    void saveUser(const User &user);

    bool exportQuiz(const User &user, int quizId, const std::string &path);
    bool importQuiz(User &user, const std::string &path);

private:
    std::string m_rootPath;
    std::string m_fileFormatVersion;
    std::vector<User> m_cachedUsers; // simple stub storage
};

class AuthService
{
public:
    explicit AuthService(StorageService &storage);

    User createAccount(const std::string &username, const std::string &password);
    std::optional<User> login(const std::string &username, const std::string &password);
    bool userExists(const std::string &username);

private:
    StorageService &m_storage;
};

// -------------------- CONTROLLERS (UI FACING) -------------------- //

class LoginController
{
public:
    explicit LoginController(AuthService &auth);

    void showLoginScreen();
    void handleLogin(const std::string &username, const std::string &password);
    void handleCreateAccount(const std::string &username, const std::string &password);

private:
    AuthService &m_auth;
};

class MainMenuController
{
public:
    explicit MainMenuController(StorageService &storage);

    void showMenu(User &user);
    void handleCreateQuiz(User &user, const std::string &title);
    void handleEditQuiz(User &user, int quizId);
    void handlePlayQuiz(User &user, int quizId, OrderStrategy &orderer);

private:
    StorageService &m_storage;
};

class EditQuizController
{
public:
    void showEditor(Quiz &quiz);
    void addQuestion(Quiz &quiz, const Question &q);
    void removeQuestion(Quiz &quiz, int idx);
    void updateQuestion(Quiz &quiz, int idx, const Question &q);
    void saveChanges(User &user);
};

class CreateQuizController
{
public:
    explicit CreateQuizController(StorageService &storage);

    // Start a brand-new quiz for this user with the given title
    void startNewQuiz(User &user, const std::string &title);

    // Mutating the quiz being built
    void addQuestion(const Question &q);
    void removeQuestion(int idx);
    void updateQuestion(int idx, const Question &q);

    // Commit the quiz to the user + storage
    void saveAndFinish();

    // Helpers
    bool hasActiveQuiz() const;
    Quiz* getQuiz();

private:
    StorageService &m_storage;
    User *m_user{ nullptr };
    std::optional<Quiz> m_workingQuiz;
};


class PlayQuizController
{
public:
    PlayQuizController() = default;

    void startSession(const Quiz &quiz, OrderStrategy &strategy);
    void showQuestion() const;
    void handleAnswer(int choiceIdx);
    void handleNext();
    void handlePrev();
    void finishSession();

    const std::optional<QuizSession> &getSession() const;

private:
    std::optional<QuizSession> m_session;
};

class ResultsController
{
public:
    void showResults(const User &user, const Quiz &quiz, const Attempt &attempt);
    void record(const Attempt &attempt, QuizStats &stats);
    void redo(const Quiz &quiz, OrderStrategy &orderer);
    void returnToMenu(const User &user);
};

#endif // QUIZAPP_H
