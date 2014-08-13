PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE movies(fid INTEGER NOT NULL PRIMARY KEY, mid INTEGER, title TEXT NOT NULL, releaseDate TEXT NOT NULL, posterPath TEXT);
CREATE TABLE videos(fid INTEGER NOT NULL PRIMARY KEY);
INSERT INTO "videos" VALUES(0);
INSERT INTO "videos" VALUES(10);
CREATE TABLE shows(id INTEGER NOT NULL PRIMARY KEY, title TEXT NOT NULL, releaseDate TEXT NOT NULL, posterPath TEXT);
INSERT INTO "shows" VALUES(1215,'Californication','2007-08-13','/home/afiestas/.local/share/jungle/tvshow-1215');
INSERT INTO "shows" VALUES(1399,'Game of Thrones','2011-11-02','/home/afiestas/.local/share/jungle/tvshow-1399');
INSERT INTO "shows" VALUES(1402,'The Walking Dead','2010-10-31','/home/afiestas/.local/share/jungle/tvshow-1402');
INSERT INTO "shows" VALUES(1418,'The Big Bang Theory','2007-09-24','/home/afiestas/.local/share/jungle/tvshow-1418');
INSERT INTO "shows" VALUES(1420,'New Girl','2011-09-20','/home/afiestas/.local/share/jungle/tvshow-1420');
INSERT INTO "shows" VALUES(1435,'The Good Wife','2009-09-22','/home/afiestas/.local/share/jungle/tvshow-1435');
INSERT INTO "shows" VALUES(2691,'Two and a Half Men','2003-09-22','/home/afiestas/.local/share/jungle/tvshow-2691');
INSERT INTO "shows" VALUES(18165,'The Vampire Diaries','2009-09-10','/home/afiestas/.local/share/jungle/tvshow-18165');
INSERT INTO "shows" VALUES(60573,'Silicon Valley','2014-04-06','/home/afiestas/.local/share/jungle/tvshow-60573');
CREATE TABLE tvseasons(id INTEGER NOT NULL, show INTEGER NOT NULL, seasonNum INTEGER NOT NULL, airDate TEXT NOT NULL, posterPath TEXT NOT NULL,PRIMARY KEY(show, seasonNum));
INSERT INTO "tvseasons" VALUES(0,1399,0,'2010-12-05','/home/afiestas/.local/share/jungle/tvshow-1399-season0');
INSERT INTO "tvseasons" VALUES(0,1399,1,'2011-11-02','/home/afiestas/.local/share/jungle/tvshow-1399-season1');
INSERT INTO "tvseasons" VALUES(0,1399,2,'2012-04-01','/home/afiestas/.local/share/jungle/tvshow-1399-season2');
INSERT INTO "tvseasons" VALUES(0,1399,3,'2013-03-31','/home/afiestas/.local/share/jungle/tvshow-1399-season3');
INSERT INTO "tvseasons" VALUES(0,1399,4,'2014-04-06','/home/afiestas/.local/share/jungle/tvshow-1399-season4');
INSERT INTO "tvseasons" VALUES(0,1215,1,'2007-08-13','/home/afiestas/.local/share/jungle/tvshow-1215-season1');
INSERT INTO "tvseasons" VALUES(0,1215,2,'2008-09-28','/home/afiestas/.local/share/jungle/tvshow-1215-season2');
INSERT INTO "tvseasons" VALUES(0,1215,3,'2009-09-27','/home/afiestas/.local/share/jungle/tvshow-1215-season3');
INSERT INTO "tvseasons" VALUES(0,1215,4,'2011-01-09','/home/afiestas/.local/share/jungle/tvshow-1215-season4');
INSERT INTO "tvseasons" VALUES(0,1215,5,'2012-01-08','/home/afiestas/.local/share/jungle/tvshow-1215-season5');
INSERT INTO "tvseasons" VALUES(0,1215,6,'2013-01-13','/home/afiestas/.local/share/jungle/tvshow-1215-season6');
INSERT INTO "tvseasons" VALUES(0,1215,7,'2014-04-13','/home/afiestas/.local/share/jungle/tvshow-1215-season7');
CREATE TABLE tvepisodes(episodeNum INTEGER NOT NULL, season INTEGER NOT NULL, show INTEGER NOT NULL, fid INTEGER, airDate TEXT NOT NULL, name TEXT NOT NULL, overview TEXT NOT NULL, stillPath TEXT, PRIMARY KEY(episodeNum, season, show));
INSERT INTO "tvepisodes" VALUES(5,4,1399,141332,'2014-05-04','First of His Name','Cersei and Tywin plan the Crown''s next move; Dany discusses future plans; Jon Snow begins a new mission.','/home/afiestas/.local/share/jungle/tvshow-1399-season4-ep5');
INSERT INTO "tvepisodes" VALUES(9,4,1399,257440,'2014-06-08','The Watchers on the Wall','Jon Snow and the Night''s Watch face a big challenge.',NULL);
INSERT INTO "tvepisodes" VALUES(10,4,1399,203438,'2014-06-15','The Children','Circumstances change after an unexpected arrival from north of the Wall. Dany must face harsh realities. Bran learns more about his destiny. Tyrion sees the truth about his situation.',NULL);
INSERT INTO "tvepisodes" VALUES(2,4,1399,257440,'2014-04-13','The Lion and the Rose','Tyrion lends Jaime a hand. Joffrey and Margaery host a breakfast. At Dragonstone, Stannis loses patience with Davos. Ramsay finds a purpose for his pet. North of the Wall, Bran sees where they must go.','/home/afiestas/.local/share/jungle/tvshow-1399-season4-ep2');
INSERT INTO "tvepisodes" VALUES(8,4,1399,257437,'2014-06-01','The Mountain and the Viper','Unexpected visitors arrive in Mole''s Town. Littlefinger''s motives are questioned. Ramsay tries to prove himself to his father. Tyrion''s fate is decided.','/home/afiestas/.local/share/jungle/tvshow-1399-season4-ep8');
INSERT INTO "tvepisodes" VALUES(6,7,1215,164857,'2014-05-18','Kickoff','A "Santa Monica Cop" kickoff party is hosted by Rath, where Hank conducts an intimate notes session with the show''s leading lady, and Hashtag Black eyes Julia. Meanwhile, Charlie manages head-case client Goldie; and Stu makes Charlie an offer.',NULL);
INSERT INTO "tvepisodes" VALUES(7,7,1215,185316,'2014-05-25','Smile','Rath wants a rewrite of Hank''s script. Charlie and Marcy mull Stu''s offer.',NULL);
INSERT INTO "tvepisodes" VALUES(8,7,1215,257435,'2014-06-01','30 Minutes or Less','Hank has problems with Hashtag Black. Stu''s offer leads to fisticuffs.',NULL);

CREATE TABLE watched(fid INTEGER NOT NULL PRIMARY KEY);
COMMIT;