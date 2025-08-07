#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QStackedWidget>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QDateTime>
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
#include <QEnterEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TaskCard;
class CategoryPage;
class TaskDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCategoryClicked(int categoryId);
    void onHomeButtonClicked();
    void onStartSessionClicked();
    void refreshTaskDisplay();

private:
    void setupDatabase();
    void setupUI();
    void setupHomeView();
    void loadCategories();
    void loadRecentTasks();
    QWidget* createTaskCard(const QString& title, const QString& description, 
                           const QString& duration, const QString& deadline, 
                           const QString& categoryColor, bool isPriority = false);
    void applyStyles();
    
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    QWidget *homeWidget;
    QWidget *headerWidget;
    QWidget *sidebarWidget;
    QListWidget *categoryList;
    QListWidget *projectList;
    QScrollArea *taskScrollArea;
    QVBoxLayout *taskLayout;
    QPushButton *startButton;
    QComboBox *methodSelector;
    QLabel *userLabel;
    QLabel *avatarLabel;
    
    QSqlDatabase database;
    QTimer *sessionTimer;
    
    // Couleurs des catégories
    QStringList categoryColors;
};

// Classe pour les cartes de tâches personnalisées
class TaskCard : public QWidget
{
    Q_OBJECT
    
public:
    TaskCard(int taskId, const QString& title, const QString& description,
             const QString& duration, const QString& deadline, 
             const QString& categoryColor, bool isPriority = false, QWidget *parent = nullptr);
    
    int getTaskId() const { return taskId; }
    
signals:
    void editRequested(int taskId);
    void deleteRequested(int taskId);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    
private slots:
    void onEditClicked();
    void onDeleteClicked();
    
private:
    void setupUI();
    
    int taskId;
    QString title;
    QString description;
    QString duration;
    QString deadline;
    QString categoryColor;
    bool isPriority;
    bool isHovered;
    
    QPushButton *editButton;
    QPushButton *deleteButton;
};

// Classe pour la page de catégorie
class CategoryPage : public QWidget
{
    Q_OBJECT
    
public:
    CategoryPage(int categoryId, const QString& categoryName, 
                 const QString& categoryColor, QWidget *parent = nullptr);
    
signals:
    void backRequested();
    void taskCreated();
    
private slots:
    void onBackClicked();
    void onNewTaskClicked();
    void onTaskEdited(int taskId);
    void onTaskDeleted(int taskId);
    void refreshTasks();
    
private:
    void setupUI();
    void loadTasks();
    void applyStyles();
    QWidget* createTaskColumn(const QString& title, const QString& headerClass);
    void addTaskToColumn(QWidget* column, int taskId, const QString& title, 
                        const QString& duration, const QString& deadline);
    
    int categoryId;
    QString categoryName;
    QString categoryColor;
    
    QWidget *todayColumn;
    QWidget *weekColumn;
    QWidget *missedColumn;
    QPushButton *backButton;
    QPushButton *newTaskButton;
    QLabel *categoryBadge;
    QLabel *totalTasksLabel;
    QLabel *inProgressLabel;
};

// Dialogue pour créer/modifier une tâche
class TaskDialog : public QDialog
{
    Q_OBJECT
    
public:
    TaskDialog(int categoryId = -1, int taskId = -1, QWidget *parent = nullptr);
    
private slots:
    void onAccept();
    
private:
    void setupUI();
    void loadTaskData();
    void saveTask();
    
    int categoryId;
    int taskId;
    bool isEditMode;
    
    QLineEdit *nameEdit;
    QTextEdit *descriptionEdit;
    QDateTimeEdit *deadlineEdit;
    QLineEdit *durationEdit;
    QComboBox *projectCombo;
    QCheckBox *redundantCheck;
};

#endif // MAINWINDOW_H
