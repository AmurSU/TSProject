PRAGMA encoding = "UTF-8";
PRAGMA foreign_keys = ON;

CREATE TABLE `patients` (
  `id`         INTEGER PRIMARY KEY AUTOINCREMENT,
  `code`       CHAR(8) NOT NULL,
  `surname`    VARCHAR(30) NOT NULL,
  `name`       VARCHAR(30) NOT NULL,
  `birth_date` DATE NOT NULL,
  `height`     REAL NOT NULL,
  `weight`     REAL NOT NULL,
  `sex`        CHAR(1) NOT NULL,
  `smokes`     BOOLEAN NOT NULL
);

CREATE INDEX `fullname_index`   ON `patients` (`surname`, `name`);
CREATE INDEX `birth_date_index` ON `patients` (`birth_date`);
CREATE INDEX `main_index`       ON `patients` (`surname`, `name`, `birth_date`);

CREATE TABLE `examinations` (
  `id`          INTEGER PRIMARY KEY AUTOINCREMENT,
  `patient_id`  INTEGER NOT NULL,
  `date`        DATE NOT NULL,
  `indication`  TEXT NOT NULL,
  `diagnosis`   TEXT NOT NULL,
  `nurse`       VARCHAR(50) NOT NULL,
  `doctor`      VARCHAR(50) NOT NULL,
  `inhale_temp` TEXT,
  `exhale_temp` TEXT,
  `volume`      TEXT,
  FOREIGN KEY(`patient_id`) REFERENCES `patients`(`id`) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE INDEX `date_index` ON `examinations` (`date` DESC);
CREATE INDEX `patients_date_index` ON `examinations` (`patient_id` ASC, `date` DESC);
