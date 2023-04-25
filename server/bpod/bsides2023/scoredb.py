import os
import uuid
import datetime
import MySQLdb
from .token import Token

class ScoreDb(object):

    def __init__(self, version):
        self.db = None
        self.version = version

    def __enter__(self):   
        self.db = MySQLdb.connect(
            host=os.environ['RDS_HOSTNAME'],
            port=int(os.environ['RDS_PORT']),
            user=os.environ['RDS_USERNAME'],
            passwd=os.environ['RDS_PASSWORD'],
            db=os.environ['RDS_DB_NAME'])
        try:
            self.init_db()
        except:
            self.db.close()
            raise
        return self

    def __exit__(self ,type, value, traceback):
        self.db.close()
        self.db = None

    def sql_write(self, cmd):
        cursor = self.db.cursor()
        try:
            try:
                cursor.execute(cmd)
                self.db.commit()
            except:
                self.db.rollback()
                raise
        except Exception as error:
            error = str(error).lower()
            if error.find('already exists') != -1:
                return
            raise

    def sql_read(self, cmd, count=None):
        cursor = self.db.cursor()
        try:
            cursor.execute(cmd)
            if count == 1:
                results = cursor.fetchone()
                if results:
                    results = [results]
                else:
                    results = list()
            else:
                results = cursor.fetchall()
            list(results)
            if count:
                results = results[:count]
        except:
            raise
        new_results = list()
        for row in results:
            new_results.append(list(row))
        results = new_results
        return results

    def sql_drop_table(self, name):
        sql = "DROP TABLE IF EXISTS {}".format(name)
        self.sql_write(sql)

    def sql_table_name(self, name):
        table_name = '{}_V{}'.format(name, self.version).upper()
        return table_name

    def sql_version(self):
        return self.sql_read("SELECT VERSION()", 1)[0][0]

    def sql_tables(self):
        results = self.sql_read('show tables;')
        tables = list()
        for row in results:
            for col in row:
                tables.append(col)
        return tables

    def score_table_name(self, game):
        return self.sql_table_name(game + '_score')

    def name_table_name(self):
        return self.sql_table_name('name')

    def create_score_table(self, game):
        sql = """
            CREATE TABLE {}
            (
                IPSN        CHAR(20) NOT NULL,
                SCORE       INT,
                TIMESTAMP   DATETIME
            )""".format(self.score_table_name(game))
        self.sql_write(sql)
        # add in defaults scores
        if len(self.score_board(game, count=1)) == 0:
            for i in range(0, 20):
                t = Token()
                t.randomise()
                ipsn = t.ipsn
                name = chr(ord('A') + i) * 3
                assert len(ipsn) == 11
                assert len(name) == 3
                score = (20 - i) * 100
                assert score % 100 == 0
                self.add_score(ipsn, game, score)
                self.add_name(ipsn, name)

    def create_name_table(self):
        sql = """
            CREATE TABLE {}
            (
                IPSN        CHAR(20) NOT NULL PRIMARY KEY,
                NAME        CHAR(20) NOT NULL
            )""".format(self.name_table_name())
        self.sql_write(sql)

    def add_name(self, ipsn, name):
        timestamp = datetime.datetime.now()
        timestamp = timestamp.strftime('%Y-%m-%d %H:%M:%S')
        sql = """
            INSERT INTO {} (
                IPSN,
                NAME
            )
            VALUES('{}', '{}')
            ON DUPLICATE KEY UPDATE NAME = '{}'
        """.format(
            self.name_table_name(),
            ipsn,
            name,
            name
        )
        self.sql_write(sql)

    def add_score(self, ipsn, game, score):
        # try to avoid duplicates of the same ipsn score
        # timestamp will help to resolve these later if it does happen
        sql = """
            SELECT IPSN, SCORE FROM {} WHERE IPSN = '{}' AND SCORE = {}
        """.format(
            self.score_table_name(game),
            ipsn,
            score
        )
        results = self.sql_read(sql)
        if len(results) > 0:
            # don't add another ipsn/score of the same values
            # user already has this score recorded
            return
        # add score
        timestamp = datetime.datetime.now()
        timestamp = timestamp.strftime('%Y-%m-%d %H:%M:%S')
        sql = """
            INSERT INTO {} (
                IPSN,
                SCORE,
                TIMESTAMP
            )
            VALUES('{}', {}, '{}')
            """.format(
                self.score_table_name(game),
                ipsn,
                score,
                timestamp
            )
        self.sql_write(sql)

    def score_board(self, game, count=None):
        tscore = self.score_table_name(game)
        tname = self.name_table_name()
        sql = """
            SELECT * FROM 
            (
                SELECT
                    {}.IPSN as IPSN,
                    {}.NAME as NAME,
                    {}.SCORE as SCORE,
                    {}.TIMESTAMP as TIMESTAMP
                FROM {}
                LEFT JOIN {} ON {}.IPSN = {}.IPSN
                ORDER BY SCORE DESC, TIMESTAMP ASC
            ) AS A
            RIGHT JOIN
            (
                SELECT IPSN, MAX(SCORE) AS MAX_SCORE FROM {} GROUP BY IPSN
            ) AS B
            ON A.IPSN = B.IPSN AND A.SCORE = B.MAX_SCORE
            ORDER BY SCORE DESC, TIMESTAMP ASC
        """.format(
            tscore, # ipsn
            tname,  # name
            tscore, # score
            tscore, # timestamp
            tscore,
            tname,
            tscore,
            tname,
            tscore, # for max score
        )
        return self.sql_read(sql, count=count)

    def name_table(self, count=None):
        sql = """
            SELECT IPSN, NAME FROM {}
        """.format(self.name_table_name())
        return self.sql_read(sql, count=count)

    def reset_db(self):
        self.sql_drop_table(self.name_table_name())
        self.sql_drop_table(self.score_table_name('snake'))
        self.sql_drop_table(self.score_table_name('tetris'))

    def init_db(self):
        self.create_name_table()
        self.create_score_table('snake')
        self.create_score_table('tetris')
