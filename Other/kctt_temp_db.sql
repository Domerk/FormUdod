PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS ��������;

CREATE TABLE �������� (
-- ����
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

PRIMARY KEY ("��� ���������", "����� ���������")
);

CREATE TABLE ������(
-- ����
"��� ���������"	  	VARCHAR(60)		NOT NULL,
"����� ���������"	VARCHAR(60)		NOT NULL,
"�����������"		VARCHAR(60)		NOT NULL
-- ���������
PRIMARY KEY ("��� ���������", "����� ���������", "�����������"),
FOREIGN KEY ("��� ���������", "����� ���������")  REFERENCES ��������("��� ���������", "����� ���������")
);


