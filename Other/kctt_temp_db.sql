PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS ��������;

CREATE TABLE �������� (
-- ����
"ID"      			INTEGER			PRIMARY KEY			AUTOINCREMENT,
"�������"			VARCHAR(60)		NOT NULL,
"���"				VARCHAR(60)		NOT NULL,
"��������"		    VARCHAR(60),
"��� ���������"	  	VARCHAR(60)		NOT NULL,
"����� ���������"	VARCHAR(60)		NOT NULL,
"���"			    VARCHAR(3)		NOT NULL,
"��� ��������"	  	DATE,

"����� �����"		  VARCHAR(60),
"�����"			      VARCHAR(60),
"�����"			      VARCHAR(5),

"��������"		    VARCHAR(200),
"�������� �����"	VARCHAR(200),
"�������"   			VARCHAR(100),
"e-mail"     			VARCHAR(100)

);


