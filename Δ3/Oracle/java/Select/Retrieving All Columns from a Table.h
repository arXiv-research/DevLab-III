CREATE TABLE EMP (EMPNO NUMBER(4) NOT NULL,
                      ENAME VARCHAR2(10),
                      HIREDATE DATE);

INSERT INTO EMP VALUES (1, 'SMITH', TO_DATE('17-DEC-1980', 'DD-MON-YYYY'));
INSERT INTO EMP VALUES (2, 'ALLEN', TO_DATE('20-FEB-1981', 'DD-MON-YYYY'));
INSERT INTO EMP VALUES (3, 'WARD',  TO_DATE('22-FEB-1981', 'DD-MON-YYYY'));
INSERT INTO EMP VALUES (4, 'JONES', TO_DATE('2-APR-1981',  'DD-MON-YYYY'));
INSERT INTO EMP VALUES (5, 'MARTIN',TO_DATE('28-SEP-1981', 'DD-MON-YYYY'));

SQL> SELECT * FROM emp;

     EMPNO ENAME      HIREDATE
---------- ---------- ---------
         1 SMITH      17-DEC-80
         2 ALLEN      20-FEB-81
         3 WARD       22-FEB-81
         4 JONES      02-APR-81
         5 MARTIN     28-SEP-81

SQL>
