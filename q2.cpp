#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
#include <ctime>

// Custom exception for invalid exam duration
class InvalidExamDurationException : public std::exception
{
public:
    const char *what() const noexcept override
    {
        return "Exam duration must be greater than 0.";
    }
};

// Custom exception for grading errors
class GradingErrorException : public std::exception
{
public:
    const char *what() const noexcept override
    {
        return "Error occurred during grading.";
    }
};

// Abstract base class for all exams
class Exam
{
protected:
    int examID;          // Unique exam identifier
    std::string subject; // Subject of the exam
    int duration;        // Duration in minutes

public:
    // Constructor with validation for duration
    Exam(int id, std::string subj, int dur)
        : examID(id), subject(subj), duration(dur)
    {
        if (dur <= 0)
            throw InvalidExamDurationException();
    }

    virtual ~Exam() {};

    // Pure virtual function for grading
    virtual int gradeExam() = 0;

    // Display exam details
    virtual void displayDetails() const
    {
        std::cout << "Exam ID: " << examID
                  << "\nSubject: " << subject
                  << "\nDuration: " << duration << " minutes\n";
    }
};

// Multiple choice exam class
class MultipleChoiceExam : public Exam
{
private:
    std::string questions[50];      // Array of questions
    std::string options[50][4];     // Options for each question
    char correctAnswers[50];        // Correct answer for each question
    char studentAnswers[50];        // Student's answers
    int totalQuestions;             // Number of questions added

public:
    // Constructor initializes arrays and question count
    MultipleChoiceExam(int id, std::string subj, int dur)
        : Exam(id, subj, dur), totalQuestions(0)
    {
        for (int i = 0; i < 50; ++i)
        {
            correctAnswers[i] = '\0';
            studentAnswers[i] = '\0';
        }
    }

    // Destructor (no dynamic memory, just for demonstration)
    ~MultipleChoiceExam()
    {
        std::cout << "[MultipleChoiceExam] Destructor called.\n";
    }

    // Add a question with options and correct answer
    void addQuestion(std::string qText, std::string opts[4], char correct)
    {
        if (totalQuestions >= 50)
            throw std::overflow_error("Too many questions.");
        questions[totalQuestions] = qText;
        for (int i = 0; i < 4; ++i)
        {
            options[totalQuestions][i] = opts[i];
        }
        correctAnswers[totalQuestions] = correct;
        totalQuestions++;
    }

    // Record student's answer for a question
    void submitAnswer(int index, char answer)
    {
        if (index >= totalQuestions || index < 0)
            throw std::out_of_range("Invalid question index.");
        studentAnswers[index] = answer;
    }

    // Grade the exam: +1 for correct, -1 for wrong, 0 for blank
    int gradeExam() override
    {
        if (totalQuestions <= 0)
        {
            throw GradingErrorException();
        }

        int score = 0;

        for (int i = 0; i < totalQuestions; ++i)
        {
            if (studentAnswers[i] == correctAnswers[i])
            {
                score++;
            }
            else if (studentAnswers[i] != '\0')
            {
                score--; // penalty for wrong answer
            }
        }

        return score;
    }

    // Display exam details and total questions
    void displayDetails() const override
    {
        Exam::displayDetails();
        std::cout << "Total Questions: " << totalQuestions << "\n";
    }
};

// Essay exam class
class EssayExam : public Exam
{
private:
    std::string topic; // Essay topic
    int score;         // Score assigned after grading

public:
    // Constructor initializes topic and sets score to -1 (ungraded)
    EssayExam(int id, std::string subj, int dur, std::string top)
        : Exam(id, subj, dur), topic(top), score(-1) {}

    // Destructor (for demonstration)
    ~EssayExam()
    {
        std::cout << "[EssayExam] Destructor called.\n";
    }

    // Assign a score to the essay (0-100)
    void assignScore(int s)
    {
        if (s < 0 || s > 100)
        {
            throw GradingErrorException();
        }
        score = s;
    }

    // Return the assigned score, throw if not graded
    int gradeExam() override
    {
        if (score == -1)
        {
            throw std::runtime_error("Essay has not been graded yet.");
        }
        return score;
    }

    // Display exam details and essay topic
    void displayDetails() const override
    {
        Exam::displayDetails();
        std::cout << "Essay Topic: " << topic << std::endl;
    }
};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0))); // seed random

    try
    {
        // Create a Multiple Choice Exam
        MultipleChoiceExam mcExam(101, "Math", 60);
        std::string opts1[] = {"2", "3", "4", "5"};
        std::string opts2[] = {"Blue", "Red", "Green", "Yellow"};

        mcExam.addQuestion("What is 2 + 2?", opts1, 'C'); // correct = 4
        mcExam.addQuestion("What color is the sky?", opts2, 'A');

        mcExam.submitAnswer(0, 'C'); // correct
        mcExam.submitAnswer(1, 'B'); // wrong

        mcExam.displayDetails();
        std::cout << "MC Exam Score: " << mcExam.gradeExam() << "\n\n";

        // Create an Essay Exam
        EssayExam essay(102, "English", 45, "Discuss the impact of social media.");

        essay.displayDetails();
        // essay.gradeExam(); // Uncomment to test ungraded exception
        essay.assignScore(85); // manually assign a score
        std::cout << "Essay Exam Score: " << essay.gradeExam() << "\n\n";

        // Trigger Invalid Duration Exception
        EssayExam badExam(103, "Science", 0, "Explain photosynthesis.");
    }
    catch (const InvalidExamDurationException &e)
    {
        std::cerr << "Caught InvalidExamDurationException: " << e.what() << "\n\n";
    }
    catch (const GradingErrorException &e)
    {
        std::cerr << "Caught GradingErrorException: " << e.what() << "\n\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught Standard Exception: " << e.what() << "\n\n";
    }

    return 0;
}
