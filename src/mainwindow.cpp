#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stackedWidget(nullptr)
    , homeWidget(nullptr)
    , sessionTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // Configuration de base
    setWindowTitle("SAI - Smart Assistant Interface");
    setMinimumSize(1200, 800);
    resize(1600, 900);
    
    // Configuration des couleurs de catégories
    categoryColors << "#ff6b35" << "#4facfe" << "#fa709a" 
                   << "#a8edea" << "#667eea" << "#43e97b";
    
    setupDatabase();
    setupUI();
    applyStyles();
}

MainWindow::~MainWindow()
{
    if (database.isOpen()) {
        database.close();
    }
    delete ui;
}

void MainWindow::setupDatabase()
{
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName("localhost");
    database.setDatabaseName("sai");
    database.setUserName("postgres");
    database.setPassword("password");
    
    if (!database.open()) {
        // Fallback: utiliser SQLite pour développement
        database = QSqlDatabase::addDatabase("QSQLITE");
        QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dbPath);
        database.setDatabaseName(dbPath + "/sai.db");
        
        if (!database.open()) {
            QMessageBox::critical(this, "Erreur base de données", 
                                 "Impossible de se connecter à la base de données.");
            return;
        }
        
        // Créer les tables de base pour SQLite
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS Category ("
                   "c_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "c_name VARCHAR,"
                   "c_color VARCHAR,"
                   "c_archived INTEGER DEFAULT 0)");
                   
        query.exec("CREATE TABLE IF NOT EXISTS Project ("
                   "p_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "p_name VARCHAR,"
                   "p_creation_date TIMESTAMP,"
                   "p_description VARCHAR,"
                   "p_c_id INTEGER,"
                   "p_archived INTEGER DEFAULT 0)");
                   
        query.exec("CREATE TABLE IF NOT EXISTS Task ("
                   "t_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "t_name VARCHAR,"
                   "t_creation_date TIMESTAMP,"
                   "t_description VARCHAR,"
                   "t_step_date TIMESTAMP,"
                   "t_p_id INTEGER,"
                   "t_c_id INTEGER,"
                   "t_estimated_time DOUBLE,"
                   "t_archived INTEGER DEFAULT 0)");
                   
        // Insérer des données de test
        query.exec("INSERT OR IGNORE INTO Category (c_id, c_name, c_color) VALUES "
                   "(1, 'Développement', '#ff6b35'),"
                   "(2, 'Design', '#4facfe'),"
                   "(3, 'Marketing', '#fa709a'),"
                   "(4, 'Recherche', '#a8edea'),"
                   "(5, 'Administration', '#667eea'),"
                   "(6, 'Formation', '#43e97b')");
                   
        query.exec("INSERT OR IGNORE INTO Project (p_id, p_name, p_description, p_c_id) VALUES "
                   "(1, 'Application SAI', 'Développement de l\\'application de gestion de tâches', 1)");
                   
        query.exec("INSERT OR IGNORE INTO Task (t_name, t_description, t_step_date, t_c_id, t_p_id, t_estimated_time, t_creation_date) VALUES "
                   "('Conception Interface Utilisateur', 'Créer les maquettes et l\\'interface utilisateur', datetime('now', '+1 day'), 1, 1, 2.0, datetime('now')),"
                   "('Développement Backend API', 'Implémenter les API REST pour le backend', datetime('now', '+2 days'), 1, 1, 3.0, datetime('now', '-1 hour')),"
                   "('Tests & Optimisation', 'Tests unitaires et optimisation des performances', datetime('now', '+3 days'), 1, 1, 1.5, datetime('now', '-2 hours')),"
                   "('Recherche Utilisateur', 'Étude des besoins utilisateurs', datetime('now', '+7 days'), 4, 1, 2.5, datetime('now', '-3 hours')),"
                   "('Documentation Technique', 'Rédaction de la documentation', datetime('now', '+14 days'), 5, 1, 1.0, datetime('now', '-4 hours')),"
                   "('Formation Équipe', 'Formation de l\\'équipe sur les nouvelles fonctionnalités', datetime('now', '+21 days'), 6, 1, 2.0, datetime('now', '-5 hours'))");
    }
}

void MainWindow::setupUI()
{
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    setupHomeView();
    stackedWidget->addWidget(homeWidget);
    stackedWidget->setCurrentWidget(homeWidget);
}

void MainWindow::setupHomeView()
{
    homeWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(homeWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Header
    headerWidget = new QWidget;
    headerWidget->setObjectName("header");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(30, 20, 30, 20);
    
    QLabel *logoLabel = new QLabel("SAI");
    logoLabel->setObjectName("logo");
    
    QWidget *userWidget = new QWidget;
    QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
    userLayout->setSpacing(15);
    
    userLabel = new QLabel("Sam Guillot");
    userLabel->setObjectName("userName");
    
    avatarLabel = new QLabel("SG");
    avatarLabel->setObjectName("avatar");
    avatarLabel->setFixedSize(45, 45);
    avatarLabel->setAlignment(Qt::AlignCenter);
    
    userLayout->addWidget(userLabel);
    userLayout->addWidget(avatarLabel);
    
    headerLayout->addWidget(logoLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(userWidget);
    
    // Container principal
    QWidget *contentContainer = new QWidget;
    QHBoxLayout *containerLayout = new QHBoxLayout(contentContainer);
    containerLayout->setSpacing(30);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    
    // Sidebar
    sidebarWidget = new QWidget;
    sidebarWidget->setObjectName("sidebar");
    sidebarWidget->setFixedWidth(300);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(30, 30, 30, 30);
    sidebarLayout->setSpacing(25);
    
    // Section Catégories
    QWidget *categoriesSection = new QWidget;
    QVBoxLayout *categoriesSectionLayout = new QVBoxLayout(categoriesSection);
    
    QWidget *categoriesHeader = new QWidget;
    QHBoxLayout *categoriesHeaderLayout = new QHBoxLayout(categoriesHeader);
    categoriesHeaderLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *categoriesTitle = new QLabel("Catégories");
    categoriesTitle->setObjectName("sectionTitle");
    
    QPushButton *addCategoryBtn = new QPushButton("+");
    addCategoryBtn->setObjectName("addBtn");
    addCategoryBtn->setFixedSize(30, 30);
    
    categoriesHeaderLayout->addWidget(categoriesTitle);
    categoriesHeaderLayout->addWidget(addCategoryBtn);
    
    categoryList = new QListWidget;
    categoryList->setObjectName("categoryList");
    loadCategories();
    
    categoriesSectionLayout->addWidget(categoriesHeader);
    categoriesSectionLayout->addWidget(categoryList);
    
    // Section Projets
    QWidget *projectsSection = new QWidget;
    QVBoxLayout *projectsSectionLayout = new QVBoxLayout(projectsSection);
    
    QLabel *projectsTitle = new QLabel("Projets");
    projectsTitle->setObjectName("sectionTitle");
    
    projectList = new QListWidget;
    projectList->setObjectName("projectList");
    
    // Ajouter le projet par défaut
    QListWidgetItem *projectItem = new QListWidgetItem("Application SAI");
    projectItem->setData(Qt::UserRole, 1); // project ID
    projectList->addItem(projectItem);
    
    projectsSectionLayout->addWidget(projectsTitle);
    projectsSectionLayout->addWidget(projectList);
    
    sidebarLayout->addWidget(categoriesSection);
    sidebarLayout->addSpacing(40);
    sidebarLayout->addWidget(projectsSection);
    sidebarLayout->addStretch();
    
    // Zone principale
    QWidget *mainContentWidget = new QWidget;
    mainContentWidget->setObjectName("mainContent");
    QVBoxLayout *mainContentLayout = new QVBoxLayout(mainContentWidget);
    mainContentLayout->setContentsMargins(40, 40, 40, 40);
    mainContentLayout->setSpacing(40);
    
    // Header du contenu principal
    QWidget *contentHeader = new QWidget;
    QHBoxLayout *contentHeaderLayout = new QHBoxLayout(contentHeader);
    contentHeaderLayout->setSpacing(40);
    
    startButton = new QPushButton("Commencer");
    startButton->setObjectName("startBtn");
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartSessionClicked);
    
    QWidget *pomodoroControl = new QWidget;
    pomodoroControl->setObjectName("pomodoroControl");
    QHBoxLayout *pomodoroLayout = new QHBoxLayout(pomodoroControl);
    pomodoroLayout->setSpacing(20);
    
    QLabel *methodLabel = new QLabel("Méthode :");
    methodLabel->setObjectName("methodLabel");
    
    methodSelector = new QComboBox;
    methodSelector->setObjectName("methodSelector");
    methodSelector->addItems({"Pomodoro", "Time Blocking", "Deep Work"});
    
    pomodoroLayout->addWidget(methodLabel);
    pomodoroLayout->addWidget(methodSelector);
    
    contentHeaderLayout->addWidget(startButton);
    contentHeaderLayout->addStretch();
    contentHeaderLayout->addWidget(pomodoroControl);
    
    // Zone des tâches
    taskScrollArea = new QScrollArea;
    taskScrollArea->setWidgetResizable(true);
    taskScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    taskScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    QWidget *taskWidget = new QWidget;
    taskLayout = new QVBoxLayout(taskWidget);
    taskLayout->setSpacing(20);
    taskLayout->setContentsMargins(20, 20, 20, 20);
    
    loadRecentTasks();
    
    taskScrollArea->setWidget(taskWidget);
    
    mainContentLayout->addWidget(contentHeader);
    mainContentLayout->addWidget(taskScrollArea);
    
    containerLayout->addWidget(sidebarWidget);
    containerLayout->addWidget(mainContentWidget);
    
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(contentContainer);
    
    // Connexions
    connect(categoryList, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        int categoryId = item->data(Qt::UserRole).toInt();
        onCategoryClicked(categoryId);
    });
}

void MainWindow::loadCategories()
{
    categoryList->clear();
    
    QSqlQuery query("SELECT c_id, c_name, c_color FROM Category WHERE c_archived = 0 ORDER BY c_id");
    
    int colorIndex = 0;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString color = query.value(2).toString();
        
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, id);
        item->setData(Qt::UserRole + 1, color);
        
        // Appliquer le style avec la couleur
        QString itemStyle = QString("background: linear-gradient(45deg, %1, %2); "
                                   "color: white; font-weight: 600; "
                                   "border-radius: 15px; padding: 15px; margin: 5px;"
                                   "text-shadow: 0 1px 3px rgba(0, 0, 0, 0.3);")
                           .arg(color)
                           .arg(colorIndex < categoryColors.size() ? categoryColors[colorIndex] : color);
        
        categoryList->addItem(item);
        colorIndex++;
    }
}

void MainWindow::loadRecentTasks()
{
    // Nettoyer le layout existant
    QLayoutItem *child;
    while ((child = taskLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    QSqlQuery query;
    query.prepare("SELECT t.t_id, t.t_name, t.t_description, t.t_estimated_time, t.t_step_date, "
                  "c.c_color, c.c_name "
                  "FROM Task t "
                  "LEFT JOIN Category c ON t.t_c_id = c.c_id "
                  "WHERE t.t_archived = 0 "
                  "ORDER BY t.t_creation_date DESC LIMIT 6");
    
    if (!query.exec()) {
        qDebug() << "Erreur chargement tâches:" << query.lastError().text();
        return;
    }
    
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(15);
    
    int row = 0;
    int col = 0;
    int stepNumber = 1;
    
    while (query.next()) {
        QString taskName = query.value(1).toString();
        QString description = query.value(2).toString();
        double estimatedHours = query.value(3).toDouble();
        QDateTime stepDate = query.value(4).toDateTime();
        QString categoryColor = query.value(5).toString();
        
        QString duration = QString("%1h").arg(estimatedHours);
        QString deadline;
        
        if (stepDate.isValid()) {
            QDate today = QDate::currentDate();
            int daysTo = today.daysTo(stepDate.date());
            if (daysTo == 0) {
                deadline = "Aujourd'hui";
            } else if (daysTo == 1) {
                deadline = "Demain";
            } else if (daysTo <= 7) {
                deadline = "Cette semaine";
            } else {
                deadline = stepDate.toString("dd/MM");
            }
        } else {
            deadline = "Pas d'échéance";
        }
        
        QWidget *stepContainer = new QWidget;
        QHBoxLayout *stepLayout = new QHBoxLayout(stepContainer);
        stepLayout->setSpacing(30);
        stepLayout->setContentsMargins(0, 0, 0, 0);
        
        // Numéro d'étape
        QLabel *stepNumberLabel = new QLabel(QString::number(stepNumber));
        stepNumberLabel->setObjectName("stepNumber");
        stepNumberLabel->setFixedSize(60, 60);
        stepNumberLabel->setAlignment(Qt::AlignCenter);
        
        // Carte de tâche
        QWidget *taskCard = createTaskCard(taskName, description, duration, deadline, 
                                         categoryColor.isEmpty() ? "#667eea" : categoryColor);
        taskCard->setMaximumWidth(350);
        
        stepLayout->addWidget(stepNumberLabel);
        stepLayout->addWidget(taskCard);
        stepLayout->addStretch();
        
        // Positionnement en escalier
        int leftMargin = (stepNumber - 1) % 3 * 40;
        stepContainer->setContentsMargins(leftMargin, 0, 0, 10);
        
        gridLayout->addWidget(stepContainer, row, col);
        
        stepNumber++;
        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }
    
    taskLayout->addLayout(gridLayout);
    taskLayout->addStretch();
}

QWidget* MainWindow::createTaskCard(const QString& title, const QString& description, 
                                   const QString& duration, const QString& deadline, 
                                   const QString& categoryColor, bool isPriority)
{
    QWidget *card = new QWidget;
    card->setObjectName("taskCard");
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 25, 25, 25);
    cardLayout->setSpacing(15);
    
    // Titre avec indicateur de priorité
    QWidget *titleWidget = new QWidget;
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(10);
    
    if (isPriority) {
        QLabel *priorityDot = new QLabel;
        priorityDot->setObjectName("priorityIndicator");
        priorityDot->setFixedSize(12, 12);
        titleLayout->addWidget(priorityDot);
    }
    
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("taskTitle");
    titleLabel->setWordWrap(true);
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    
    // Métadonnées
    QWidget *metaWidget = new QWidget;
    QVBoxLayout *metaLayout = new QVBoxLayout(metaWidget);
    metaLayout->setContentsMargins(0, 0, 0, 0);
    metaLayout->setSpacing(10);
    
    if (!duration.isEmpty()) {
        QLabel *durationLabel = new QLabel(QString("⏱️ %1").arg(duration));
        durationLabel->setObjectName("taskMeta");
        metaLayout->addWidget(durationLabel);
    }
    
    if (!deadline.isEmpty()) {
        QLabel *deadlineLabel = new QLabel(QString("📅 %1").arg(deadline));
        deadlineLabel->setObjectName("taskMeta");
        metaLayout->addWidget(deadlineLabel);
    }
    
    cardLayout->addWidget(titleWidget);
    cardLayout->addWidget(metaWidget);
    
    return card;
}

void MainWindow::onCategoryClicked(int categoryId)
{
    QSqlQuery query;
    query.prepare("SELECT c_name, c_color FROM Category WHERE c_id = ?");
    query.addBindValue(categoryId);
    
    if (!query.exec() || !query.next()) {
        return;
    }
    
    QString categoryName = query.value(0).toString();
    QString categoryColor = query.value(1).toString();
    
    CategoryPage *categoryPage = new CategoryPage(categoryId, categoryName, categoryColor, this);
    connect(categoryPage, &CategoryPage::backRequested, this, &MainWindow::onHomeButtonClicked);
    connect(categoryPage, &CategoryPage::taskCreated, this, &MainWindow::refreshTaskDisplay);
    
    stackedWidget->addWidget(categoryPage);
    stackedWidget->setCurrentWidget(categoryPage);
}

void MainWindow::onHomeButtonClicked()
{
    stackedWidget->setCurrentWidget(homeWidget);
    
    // Supprimer les pages de catégorie précédentes
    for (int i = stackedWidget->count() - 1; i >= 1; --i) {
        QWidget *widget = stackedWidget->widget(i);
        stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }
    
    refreshTaskDisplay();
}

void MainWindow::onStartSessionClicked()
{
    startButton->setText("⏱️ Session en cours...");
    startButton->setEnabled(false);
    
    // Simuler une session de 3 secondes pour démonstration
    QTimer::singleShot(3000, [this]() {
        startButton->setText("Commencer");
        startButton->setEnabled(true);
    });
}

void MainWindow::refreshTaskDisplay()
{
    loadRecentTasks();
    loadCategories();
}

void MainWindow::applyStyles()
{
    QString styleSheet = R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, 
                                       stop:0 #667eea, stop:1 #764ba2);
        }
        
        #header {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 20px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        #logo {
            font-size: 28px;
            font-weight: 800;
            color: white;
            text-shadow: 0 2px 10px rgba(0, 0, 0, 0.3);
        }
        
        #userName {
            color: white;
            font-size: 16px;
            font-weight: 600;
        }
        
        #avatar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #ff6b6b, stop:1 #4ecdc4);
            color: white;
            font-weight: bold;
            border-radius: 22px;
            font-size: 14px;
        }
        
        #sidebar {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 25px;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
        
        #sectionTitle {
            font-size: 20px;
            color: #2d3748;
            font-weight: 600;
        }
        
        #addBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #4f46e5, stop:1 #7c3aed);
            color: white;
            border: none;
            border-radius: 15px;
            font-size: 18px;
            font-weight: bold;
        }
        
        #addBtn:hover {
            transform: scale(1.1);
        }
        
        #categoryList, #projectList {
            background: transparent;
            border: none;
            outline: none;
        }
        
        #categoryList::item, #projectList::item {
            background: rgba(102, 126, 234, 0.1);
            color: #4f46e5;
            border-radius: 12px;
            padding: 15px;
            margin: 5px 0;
            font-weight: 600;
        }
        
        #categoryList::item:hover, #projectList::item:hover {
            transform: translateX(5px);
            background: rgba(102, 126, 234, 0.15);
        }
        
        #mainContent {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 25px;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
        
        #startBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #00c851, stop:1 #007e33);
            color: white;
            border: none;
            padding: 20px 50px;
            border-radius: 25px;
            font-size: 22px;
            font-weight: 700;
        }
        
        #startBtn:hover {
            transform: translateY(-2px);
        }
        
        #pomodoroControl {
            background: rgba(102, 126, 234, 0.1);
            border: 2px solid rgba(102, 126, 234, 0.2);
            border-radius: 20px;
            padding: 20px 30px;
        }
        
        #methodLabel {
            color: #4f46e5;
            font-weight: 600;
            font-size: 16px;
        }
        
        #methodSelector {
            background: white;
            border: 2px solid #e2e8f0;
            border-radius: 12px;
            padding: 8px 15px;
            font-size: 16px;
            color: #2d3748;
        }
        
        #methodSelector:focus {
            border-color: #4f46e5;
            outline: none;
        }
        
        #stepNumber {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #4f46e5, stop:1 #7c3aed);
            color: white;
            font-weight: bold;
            font-size: 24px;
            border-radius: 30px;
        }
        
        #taskCard {
            background: white;
            border-radius: 20px;
            border-left: 5px solid #ff6b35;
            margin: 10px 0;
        }
        
        #taskCard:hover {
            transform: translateY(-5px);
        }
        
        #taskTitle {
            font-size: 20px;
            font-weight: 700;
            color: #2d3748;
        }
        
        #taskMeta {
            color: #64748b;
            font-size: 14px;
        }
        
        #priorityIndicator {
            background: #ff4757;
            border-radius: 6px;
        }
        
        QScrollArea {
            border: none;
            background: transparent;
        }
        
        QScrollBar:vertical {
            background: rgba(255, 255, 255, 0.1);
            width: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical {
            background: rgba(102, 126, 234, 0.5);
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: rgba(102, 126, 234, 0.7);
        }
    )";
    
    setStyleSheet(styleSheet);
}


// ========== TaskCard Implementation ==========
TaskCard::TaskCard(int taskId, const QString& title, const QString& description,
                   const QString& duration, const QString& deadline, 
                   const QString& categoryColor, bool isPriority, QWidget *parent)
    : QWidget(parent), taskId(taskId), title(title), description(description),
      duration(duration), deadline(deadline), categoryColor(categoryColor),
      isPriority(isPriority), isHovered(false)
{
    setupUI();
    setFixedHeight(120);
    setMinimumWidth(300);
}

void TaskCard::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 15, 20, 15);
    mainLayout->setSpacing(10);
    
    // En-tête avec titre et boutons
    QWidget *headerWidget = new QWidget;
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(10);
    
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-weight: 700; color: #2d3748; font-size: 16px;");
    titleLabel->setWordWrap(true);
    
    QWidget *actionsWidget = new QWidget;
    QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
    actionsLayout->setContentsMargins(0, 0, 0, 0);
    actionsLayout->setSpacing(8);
    
    editButton = new QPushButton("✏️");
    editButton->setFixedSize(28, 28);
    editButton->setStyleSheet("QPushButton { background: rgba(255, 255, 255, 0.9); "
                              "border: 1px solid rgba(255, 255, 255, 0.3); "
                              "border-radius: 14px; font-size: 12px; } "
                              "QPushButton:hover { background: white; transform: scale(1.1); }");
    connect(editButton, &QPushButton::clicked, this, &TaskCard::onEditClicked);
    
    deleteButton = new QPushButton("🗑️");
    deleteButton->setFixedSize(28, 28);
    deleteButton->setStyleSheet(editButton->styleSheet());
    connect(deleteButton, &QPushButton::clicked, this, &TaskCard::onDeleteClicked);
    
    actionsLayout->addWidget(editButton);
    actionsLayout->addWidget(deleteButton);
    
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(actionsWidget);
    
    // Métadonnées
    QWidget *metaWidget = new QWidget;
    QHBoxLayout *metaLayout = new QHBoxLayout(metaWidget);
    metaLayout->setContentsMargins(0, 0, 0, 0);
    metaLayout->setSpacing(15);
    
    if (!duration.isEmpty()) {
        QLabel *durationLabel = new QLabel(QString("⏱️ %1").arg(duration));
        durationLabel->setStyleSheet("color: #64748b; font-size: 13px;");
        metaLayout->addWidget(durationLabel);
    }
    
    if (!deadline.isEmpty()) {
        QLabel *deadlineLabel = new QLabel(QString("📅 %1").arg(deadline));
        deadlineLabel->setStyleSheet("color: #64748b; font-size: 13px;");
        metaLayout->addWidget(deadlineLabel);
    }
    
    metaLayout->addStretch();
    
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(metaWidget);
    mainLayout->addStretch();
}

void TaskCard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = this->rect();
    
    // Fond avec bordure colorée
    QColor bgColor = isHovered ? QColor(255, 255, 255) : QColor(250, 250, 250);
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 15, 15);
    
    // Bordure gauche colorée
    QColor borderColor(categoryColor);
    if (!borderColor.isValid()) {
        borderColor = QColor("#ff6b35");
    }
    
    painter.setBrush(borderColor);
    painter.drawRoundedRect(0, 0, 5, rect.height(), 3, 3);
    
    QWidget::paintEvent(event);
}

void TaskCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Animation de clic
        setStyleSheet("transform: scale(0.98);");
        QTimer::singleShot(150, [this]() {
            setStyleSheet("");
        });
    }
    QWidget::mousePressEvent(event);
}

void TaskCard::enterEvent(QEnterEvent *event)
{
    isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void TaskCard::leaveEvent(QEvent *event)
{
    isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void TaskCard::onEditClicked()
{
    emit editRequested(taskId);
}

void TaskCard::onDeleteClicked()
{
    int ret = QMessageBox::question(this, "Confirmer la suppression",
                                   QString("Êtes-vous sûr de vouloir supprimer la tâche '%1' ?").arg(title),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        emit deleteRequested(taskId);
    }
}

// ========== CategoryPage Implementation ==========
CategoryPage::CategoryPage(int categoryId, const QString& categoryName, 
                          const QString& categoryColor, QWidget *parent)
    : QWidget(parent), categoryId(categoryId), categoryName(categoryName), categoryColor(categoryColor)
{
    setupUI();
    applyStyles();
    loadTasks();
}

void CategoryPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // Header
    QWidget *headerWidget = new QWidget;
    headerWidget->setObjectName("header");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(30, 20, 30, 20);
    
    QLabel *logoLabel = new QLabel("SAI");
    logoLabel->setObjectName("logo");
    
    QWidget *userWidget = new QWidget;
    QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
    userLayout->setSpacing(15);
    
    QLabel *userLabel = new QLabel("Sam Guillot");
    userLabel->setObjectName("userName");
    
    QLabel *avatarLabel = new QLabel("SG");
    avatarLabel->setObjectName("avatar");
    avatarLabel->setFixedSize(45, 45);
    avatarLabel->setAlignment(Qt::AlignCenter);
    
    userLayout->addWidget(userLabel);
    userLayout->addWidget(avatarLabel);
    
    headerLayout->addWidget(logoLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(userWidget);
    
    // Container principal
    QWidget *contentContainer = new QWidget;
    QHBoxLayout *containerLayout = new QHBoxLayout(contentContainer);
    containerLayout->setSpacing(30);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    
    // Sidebar
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setObjectName("sidebar");
    sidebarWidget->setFixedWidth(300);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(30, 30, 30, 30);
    sidebarLayout->setSpacing(30);
    
    // Bouton retour
    backButton = new QPushButton("← Retour");
    backButton->setObjectName("backButton");
    connect(backButton, &QPushButton::clicked, this, &CategoryPage::onBackClicked);
    
    // Info catégorie
    QWidget *categoryInfo = new QWidget;
    QVBoxLayout *categoryInfoLayout = new QVBoxLayout(categoryInfo);
    categoryInfoLayout->setAlignment(Qt::AlignCenter);
    categoryInfoLayout->setSpacing(15);
    
    categoryBadge = new QLabel(categoryName);
    categoryBadge->setObjectName("categoryBadge");
    categoryBadge->setAlignment(Qt::AlignCenter);
    
    QWidget *statsWidget = new QWidget;
    QGridLayout *statsLayout = new QGridLayout(statsWidget);
    statsLayout->setSpacing(15);
    
    QWidget *totalCard = new QWidget;
    totalCard->setObjectName("statCard");
    QVBoxLayout *totalLayout = new QVBoxLayout(totalCard);
    totalLayout->setAlignment(Qt::AlignCenter);
    
    totalTasksLabel = new QLabel("0");
    totalTasksLabel->setObjectName("statNumber");
    QLabel *totalLabel = new QLabel("TÂCHES TOTALES");
    totalLabel->setObjectName("statLabel");
    
    totalLayout->addWidget(totalTasksLabel);
    totalLayout->addWidget(totalLabel);
    
    QWidget *progressCard = new QWidget;
    progressCard->setObjectName("statCard");
    QVBoxLayout *progressLayout = new QVBoxLayout(progressCard);
    progressLayout->setAlignment(Qt::AlignCenter);
    
    inProgressLabel = new QLabel("0");
    inProgressLabel->setObjectName("statNumber");
    QLabel *progressLabelText = new QLabel("EN COURS");
    progressLabelText->setObjectName("statLabel");
    
    progressLayout->addWidget(inProgressLabel);
    progressLayout->addWidget(progressLabelText);
    
    statsLayout->addWidget(totalCard, 0, 0);
    statsLayout->addWidget(progressCard, 0, 1);
    
    categoryInfoLayout->addWidget(categoryBadge);
    categoryInfoLayout->addWidget(statsWidget);
    
    sidebarLayout->addWidget(backButton);
    sidebarLayout->addWidget(categoryInfo);
    sidebarLayout->addStretch();
    
    // Zone principale
    QWidget *mainContentWidget = new QWidget;
    mainContentWidget->setObjectName("mainContent");
    QVBoxLayout *mainContentLayout = new QVBoxLayout(mainContentWidget);
    mainContentLayout->setContentsMargins(40, 40, 40, 40);
    mainContentLayout->setSpacing(40);
    
    // Header du contenu
    QWidget *contentHeader = new QWidget;
    QHBoxLayout *contentHeaderLayout = new QHBoxLayout(contentHeader);
    
    QLabel *pageTitle = new QLabel(categoryName);
    pageTitle->setObjectName("pageTitle");
    
    newTaskButton = new QPushButton("+ Nouvelle tâche");
    newTaskButton->setObjectName("newTaskBtn");
    connect(newTaskButton, &QPushButton::clicked, this, &CategoryPage::onNewTaskClicked);
    
    contentHeaderLayout->addWidget(pageTitle);
    contentHeaderLayout->addStretch();
    contentHeaderLayout->addWidget(newTaskButton);
    
    // Colonnes de tâches
    QWidget *columnsWidget = new QWidget;
    QHBoxLayout *columnsLayout = new QHBoxLayout(columnsWidget);
    columnsLayout->setSpacing(25);
    
    todayColumn = createTaskColumn("Aujourd'hui", "today");
    weekColumn = createTaskColumn("Cette semaine", "week");
    missedColumn = createTaskColumn("Manquées", "missed");
    
    columnsLayout->addWidget(todayColumn);
    columnsLayout->addWidget(weekColumn);
    columnsLayout->addWidget(missedColumn);
    
    mainContentLayout->addWidget(contentHeader);
    mainContentLayout->addWidget(columnsWidget);
    
    containerLayout->addWidget(sidebarWidget);
    containerLayout->addWidget(mainContentWidget);
    
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(contentContainer);
}

QWidget* CategoryPage::createTaskColumn(const QString& title, const QString& headerClass)
{
    QWidget *column = new QWidget;
    column->setObjectName("taskColumn");
    column->setProperty("columnType", headerClass);
    
    QVBoxLayout *layout = new QVBoxLayout(column);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(20);
    
    QLabel *header = new QLabel(title);
    header->setObjectName("columnHeader");
    header->setProperty("headerClass", headerClass);
    header->setAlignment(Qt::AlignCenter);
    
    layout->addWidget(header);
    layout->addStretch();
    
    return column;
}

void CategoryPage::loadTasks()
{
    // Nettoyer les colonnes
    for (QWidget *column : {todayColumn, weekColumn, missedColumn}) {
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(column->layout());
        if (layout) {
            // Garder seulement le header (premier élément)
            while (layout->count() > 2) { // header + stretch
                QLayoutItem *item = layout->takeAt(1);
                if (item->widget()) {
                    item->widget()->deleteLater();
                }
                delete item;
            }
        }
    }
    
    QSqlQuery query;
    query.prepare("SELECT t_id, t_name, t_estimated_time, t_step_date FROM Task "
                  "WHERE t_c_id = ? AND t_archived = 0 ORDER BY t_step_date ASC");
    query.addBindValue(categoryId);
    
    if (!query.exec()) {
        qDebug() << "Erreur chargement tâches catégorie:" << query.lastError().text();
        return;
    }
    
    int totalTasks = 0;
    int inProgressTasks = 0;
    QDate today = QDate::currentDate();
    
    while (query.next()) {
        int taskId = query.value(0).toInt();
        QString taskName = query.value(1).toString();
        double estimatedHours = query.value(2).toDouble();
        QDateTime stepDate = query.value(3).toDateTime();
        
        totalTasks++;
        
        QString duration = QString("%1h").arg(estimatedHours);
        QString deadline;
        QWidget *targetColumn = weekColumn; // par défaut
        
        if (stepDate.isValid()) {
            int daysTo = today.daysTo(stepDate.date());
            if (daysTo < 0) {
                deadline = "En retard";
                targetColumn = missedColumn;
            } else if (daysTo == 0) {
                deadline = "Aujourd'hui";
                targetColumn = todayColumn;
                inProgressTasks++;
            } else if (daysTo == 1) {
                deadline = "Demain";
                targetColumn = todayColumn;
                inProgressTasks++;
            } else if (daysTo <= 7) {
                deadline = "Cette semaine";
                targetColumn = weekColumn;
            } else {
                deadline = stepDate.toString("dd/MM");
                targetColumn = weekColumn;
            }
        } else {
            deadline = "Pas d'échéance";
        }
        
        addTaskToColumn(targetColumn, taskId, taskName, duration, deadline);
    }
    
    totalTasksLabel->setText(QString::number(totalTasks));
    inProgressLabel->setText(QString::number(inProgressTasks));
}

void CategoryPage::addTaskToColumn(QWidget* column, int taskId, const QString& title, 
                                  const QString& duration, const QString& deadline)
{
    TaskCard *card = new TaskCard(taskId, title, "", duration, deadline, categoryColor);
    connect(card, &TaskCard::editRequested, this, &CategoryPage::onTaskEdited);
    connect(card, &TaskCard::deleteRequested, this, &CategoryPage::onTaskDeleted);
    
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(column->layout());
    if (layout) {
        layout->insertWidget(layout->count() - 1, card); // Avant le stretch
    }
}

void CategoryPage::onBackClicked()
{
    emit backRequested();
}

void CategoryPage::onNewTaskClicked()
{
    TaskDialog dialog(categoryId, -1, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTasks();
        emit taskCreated();
    }
}

void CategoryPage::onTaskEdited(int taskId)
{
    TaskDialog dialog(categoryId, taskId, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshTasks();
        emit taskCreated();
    }
}

void CategoryPage::onTaskDeleted(int taskId)
{
    QSqlQuery query;
    query.prepare("UPDATE Task SET t_archived = 1 WHERE t_id = ?");
    query.addBindValue(taskId);
    
    if (query.exec()) {
        refreshTasks();
        emit taskCreated();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer la tâche.");
    }
}

void CategoryPage::refreshTasks()
{
    loadTasks();
}

void CategoryPage::applyStyles()
{
    QString styleSheet = R"(
        #header {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 20px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        
        #logo {
            font-size: 28px;
            font-weight: 800;
            color: white;
        }
        
        #userName {
            color: white;
            font-size: 16px;
            font-weight: 600;
        }
        
        #avatar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #ff6b6b, stop:1 #4ecdc4);
            color: white;
            font-weight: bold;
            border-radius: 22px;
            font-size: 14px;
        }
        
        #sidebar {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 25px;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
        
        #backButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #667eea, stop:1 #764ba2);
            color: white;
            border: none;
            padding: 12px 20px;
            border-radius: 15px;
            font-size: 16px;
            font-weight: 600;
        }
        
        #backButton:hover {
            transform: translateY(-2px);
        }
        
        #categoryBadge {
            background: )" + categoryColor + R"(;
            color: white;
            padding: 15px 25px;
            border-radius: 20px;
            font-size: 24px;
            font-weight: 700;
        }
        
        #statCard {
            background: rgba(102, 126, 234, 0.1);
            border: 2px solid rgba(102, 126, 234, 0.2);
            border-radius: 15px;
            padding: 15px;
        }
        
        #statNumber {
            font-size: 24px;
            font-weight: 700;
            color: #4f46e5;
        }
        
        #statLabel {
            font-size: 12px;
            color: #64748b;
            font-weight: 600;
        }
        
        #mainContent {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 25px;
            border: 1px solid rgba(255, 255, 255, 0.3);
        }
        
        #pageTitle {
            font-size: 32px;
            font-weight: 800;
            color: #2d3748;
        }
        
        #newTaskBtn {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #00c851, stop:1 #007e33);
            color: white;
            border: none;
            padding: 15px 30px;
            border-radius: 20px;
            font-size: 16px;
            font-weight: 600;
        }
        
        #newTaskBtn:hover {
            transform: translateY(-2px);
        }
        
        #taskColumn {
            background: rgba(248, 250, 252, 0.8);
            border-radius: 20px;
            border: 2px solid transparent;
            min-height: 400px;
        }
        
        #taskColumn[columnType="today"] {
            background: rgba(34, 197, 94, 0.1);
            border-color: rgba(34, 197, 94, 0.2);
        }
        
        #taskColumn[columnType="week"] {
            background: rgba(59, 130, 246, 0.1);
            border-color: rgba(59, 130, 246, 0.2);
        }
        
        #taskColumn[columnType="missed"] {
            background: rgba(239, 68, 68, 0.1);
            border-color: rgba(239, 68, 68, 0.2);
        }
        
        #columnHeader {
            padding: 15px 20px;
            border-radius: 15px;
            font-weight: 700;
            font-size: 18px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        #columnHeader[headerClass="today"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #22c55e, stop:1 #16a34a);
            color: white;
        }
        
        #columnHeader[headerClass="week"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #3b82f6, stop:1 #1d4ed8);
            color: white;
        }
        
        #columnHeader[headerClass="missed"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                       stop:0 #ef4444, stop:1 #b91c1c);
            color: white;
        }
    )";
    
    setStyleSheet(styleSheet);
}

// ========== TaskDialog Implementation ==========
TaskDialog::TaskDialog(int categoryId, int taskId, QWidget *parent)
    : QDialog(parent), categoryId(categoryId), taskId(taskId), isEditMode(taskId != -1)
{
    setWindowTitle(isEditMode ? "Modifier la tâche" : "Nouvelle tâche");
    setModal(true);
    resize(500, 400);
    setupUI();
    
    if (isEditMode) {
        loadTaskData();
    }
}

void TaskDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(15);
    
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("Ex: Développer l'API utilisateur");
    formLayout->addRow("Nom de la tâche:", nameEdit);
    
    descriptionEdit = new QTextEdit;
    descriptionEdit->setPlaceholderText("Détails de la tâche...");
    descriptionEdit->setMaximumHeight(100);
    formLayout->addRow("Description:", descriptionEdit);
    
    deadlineEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(1));
    deadlineEdit->setDisplayFormat("dd/MM/yyyy hh:mm");
    formLayout->addRow("Échéance:", deadlineEdit);
    
    durationEdit = new QLineEdit;
    durationEdit->setPlaceholderText("Ex: 2.5");
    durationEdit->setText("1.0");
    formLayout->addRow("Durée estimée (heures):", durationEdit);
    
    projectCombo = new QComboBox;
    QSqlQuery projectQuery("SELECT p_id, p_name FROM Project WHERE p_archived = 0");
    while (projectQuery.next()) {
        int pId = projectQuery.value(0).toInt();
        QString pName = projectQuery.value(1).toString();
        projectCombo->addItem(pName, pId);
    }
    formLayout->addRow("Projet:", projectCombo);
    
    redundantCheck = new QCheckBox("Tâche récurrente");
    formLayout->addRow("", redundantCheck);
    
    mainLayout->addLayout(formLayout);
    
    // Boutons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setText(isEditMode ? "Modifier" : "Créer");
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskDialog::reject);
    
    mainLayout->addWidget(buttonBox);
    
    // Style
    setStyleSheet(R"(
        QDialog {
            background: white;
            border-radius: 20px;
        }
        
        QLineEdit, QTextEdit, QDateTimeEdit, QComboBox {
            padding: 8px 12px;
            border: 2px solid #e2e8f0;
            border-radius: 8px;
            font-size: 14px;
        }
        
        QLineEdit:focus, QTextEdit:focus, QDateTimeEdit:focus, QComboBox:focus {
            border-color: #4f46e5;
        }
        
        QPushButton {
            padding: 10px 20px;
            border-radius: 8px;
            font-weight: 600;
        }
        
        QDialogButtonBox QPushButton {
            background: #4f46e5;
            color: white;
            border: none;
        }
        
        QDialogButtonBox QPushButton:hover {
            background: #3730a3;
        }
        
        QCheckBox {
            font-size: 14px;
        }
    )");
}

void TaskDialog::loadTaskData()
{
    QSqlQuery query;
    query.prepare("SELECT t_name, t_description, t_step_date, t_estimated_time, t_p_id, redondant "
                  "FROM Task WHERE t_id = ?");
    query.addBindValue(taskId);
    
    if (query.exec() && query.next()) {
        nameEdit->setText(query.value(0).toString());
        descriptionEdit->setPlainText(query.value(1).toString());
        
        QDateTime stepDate = query.value(2).toDateTime();
        if (stepDate.isValid()) {
            deadlineEdit->setDateTime(stepDate);
        }
        
        durationEdit->setText(QString::number(query.value(3).toDouble()));
        
        int projectId = query.value(4).toInt();
        for (int i = 0; i < projectCombo->count(); ++i) {
            if (projectCombo->itemData(i).toInt() == projectId) {
                projectCombo->setCurrentIndex(i);
                break;
            }
        }
        
        redundantCheck->setChecked(query.value(5).toInt() == 1);
    }
}

void TaskDialog::onAccept()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom de la tâche ne peut pas être vide.");
        return;
    }
    
    saveTask();
}

void TaskDialog::saveTask()
{
    QSqlQuery query;
    
    if (isEditMode) {
        query.prepare("UPDATE Task SET t_name = ?, t_description = ?, t_step_date = ?, "
                      "t_estimated_time = ?, t_p_id = ?, redondant = ? WHERE t_id = ?");
        query.addBindValue(nameEdit->text().trimmed());
        query.addBindValue(descriptionEdit->toPlainText().trimmed());
        query.addBindValue(deadlineEdit->dateTime());
        query.addBindValue(durationEdit->text().toDouble());
        query.addBindValue(projectCombo->currentData().toInt());
        query.addBindValue(redundantCheck->isChecked() ? 1 : 0);
        query.addBindValue(taskId);
    } else {
        query.prepare("INSERT INTO Task (t_name, t_description, t_step_date, t_estimated_time, "
                      "t_p_id, t_c_id, t_creation_date, redondant) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(nameEdit->text().trimmed());
        query.addBindValue(descriptionEdit->toPlainText().trimmed());
        query.addBindValue(deadlineEdit->dateTime());
        query.addBindValue(durationEdit->text().toDouble());
        query.addBindValue(projectCombo->currentData().toInt());
        query.addBindValue(categoryId);
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(redundantCheck->isChecked() ? 1 : 0);
    }
    
    if (query.exec()) {
        accept();
    } else {
        QMessageBox::warning(this, "Erreur", 
                           QString("Impossible de sauvegarder la tâche: %1")
                           .arg(query.lastError().text()));
    }
}

