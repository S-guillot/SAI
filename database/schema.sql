-- PostgreSQL schema converted from DBML
-- Database: sai

-- Create tables
CREATE TABLE IF NOT EXISTS "User" (
    u_id SERIAL PRIMARY KEY,
    u_name VARCHAR,
    u_last_connexion TIMESTAMP,
    u_productivity_index DOUBLE PRECISION,
    u_archived INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS Category (
    c_id SERIAL PRIMARY KEY,
    c_name VARCHAR,
    c_color VARCHAR UNIQUE NOT NULL,
    c_priority INTEGER NOT NULL,
    c_archived INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS Project (
    p_id INTEGER PRIMARY KEY,
    p_name VARCHAR UNIQUE,
    p_creation_date TIMESTAMP,
    p_description VARCHAR,
    p_priority INTEGER NOT NULL,
    p_step_date TIMESTAMP,
    p_statut INTEGER DEFAULT 0,
    p_likability_index DOUBLE PRECISION,
    p_time_passed_on DOUBLE PRECISION,
    p_archived INTEGER DEFAULT 0,
    p_last_targeted TIMESTAMP,
    p_c_id INTEGER,
    p_estimated_time DOUBLE PRECISION,
    FOREIGN KEY (p_c_id) REFERENCES Category(c_id)
);

CREATE TABLE IF NOT EXISTS Task (
    t_id SERIAL PRIMARY KEY,
    t_name VARCHAR UNIQUE,
    t_creation_date TIMESTAMP NOT NULL,
    t_description VARCHAR,
    t_priority INTEGER NOT NULL,
    t_step_date TIMESTAMP,
    t_p_id INTEGER UNIQUE,
    t_statut INTEGER DEFAULT 0,
    t_likability_index DOUBLE PRECISION,
    t_time_passed_on DOUBLE PRECISION,
    t_archived INTEGER DEFAULT 0,
    t_last_targeted TIMESTAMP,
    t_c_id INTEGER,
    t_estimated_time DOUBLE PRECISION,
    redondant INTEGER DEFAULT 0,
    FOREIGN KEY (t_p_id) REFERENCES Project(p_id),
    FOREIGN KEY (t_c_id) REFERENCES Category(c_id)
);

CREATE TABLE IF NOT EXISTS Task_User (
    u_id INTEGER,
    t_id INTEGER,
    PRIMARY KEY (u_id, t_id),
    FOREIGN KEY (u_id) REFERENCES "User"(u_id),
    FOREIGN KEY (t_id) REFERENCES Task(t_id)
);

CREATE TABLE IF NOT EXISTS Project_User (
    u_id INTEGER,
    p_id INTEGER,
    PRIMARY KEY (u_id, p_id),
    FOREIGN KEY (u_id) REFERENCES "User"(u_id),
    FOREIGN KEY (p_id) REFERENCES Project(p_id)
);

CREATE TABLE IF NOT EXISTS Notification (
    n_id SERIAL PRIMARY KEY,
    n_message VARCHAR NOT NULL,
    n_type VARCHAR NOT NULL,
    n_u_id INTEGER,
    n_importancy INTEGER,
    n_archived INTEGER DEFAULT 0,
    FOREIGN KEY (n_u_id) REFERENCES "User"(u_id)
);

CREATE TABLE IF NOT EXISTS Productivity_periode (
    pd_id SERIAL PRIMARY KEY,
    pd_periode INTEGER NOT NULL,
    pd_day INTEGER NOT NULL,
    pd_productivity_index DOUBLE PRECISION DEFAULT 1.0
);

CREATE TABLE IF NOT EXISTS Work_methode (
    wm_id SERIAL PRIMARY KEY,
    wm_name VARCHAR,
    wm_user_compatibility_index DOUBLE PRECISION DEFAULT 1.0,
    wm_use_count INTEGER,
    wm_archived INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS Freq_duration_management (
    fm_id SERIAL PRIMARY KEY,
    fm_pause_duration INTEGER,
    fm_work_duration INTEGER,
    fm_pause_frequency INTEGER,
    fm_work_frequency INTEGER,
    fm_wm_id INTEGER,
    FOREIGN KEY (fm_wm_id) REFERENCES Work_methode(wm_id)
);

-- Create indexes for better performance
CREATE INDEX IF NOT EXISTS idx_task_project ON Task(t_p_id);
CREATE INDEX IF NOT EXISTS idx_task_category ON Task(t_c_id);
CREATE INDEX IF NOT EXISTS idx_project_category ON Project(p_c_id);
CREATE INDEX IF NOT EXISTS idx_notification_user ON Notification(n_u_id);
CREATE INDEX IF NOT EXISTS idx_task_user_task ON Task_User(t_id);
CREATE INDEX IF NOT EXISTS idx_task_user_user ON Task_User(u_id);
CREATE INDEX IF NOT EXISTS idx_project_user_project ON Project_User(p_id);
CREATE INDEX IF NOT EXISTS idx_project_user_user ON Project_User(u_id);
CREATE INDEX IF NOT EXISTS idx_freq_duration_work_methode ON Freq_duration_management(fm_wm_id);