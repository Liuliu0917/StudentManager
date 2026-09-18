-- MySQL dump 10.13  Distrib 8.0.46, for Win64 (x86_64)
--
-- Host: localhost    Database: student_db
-- ------------------------------------------------------
-- Server version	8.0.46

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `student_db`
--

/*!40000 DROP DATABASE IF EXISTS `student_db`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `student_db` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `student_db`;

--
-- Table structure for table `admin`
--

DROP TABLE IF EXISTS `admin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `admin` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` varchar(50) COLLATE utf8mb4_unicode_ci NOT NULL,
  `role` enum('admin','student') COLLATE utf8mb4_unicode_ci DEFAULT 'admin',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `admin`
--

LOCK TABLES `admin` WRITE;
/*!40000 ALTER TABLE `admin` DISABLE KEYS */;
INSERT INTO `admin` VALUES (1,'admin','admin123','admin'),(2,'student','123456','student');
/*!40000 ALTER TABLE `admin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `course` (
  `cId` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '课程号',
  `name` varchar(30) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '课程名',
  `teacher` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '任课教师',
  `credit` int DEFAULT NULL COMMENT '学分',
  PRIMARY KEY (`cId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course`
--

LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES ('C001','数据库原理','王老师',4),('C002','操作系统','李老师',3),('C003','计算机网络','赵老师',3),('C004','数据结构','陈老师',4),('C005','软件工程','刘老师',3),('C006','Java程序设计','杨老师',3),('C007','Python程序设计','黄老师',2),('C008','Web开发技术','周老师',3);
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trg_course_delete` BEFORE DELETE ON `course` FOR EACH ROW BEGIN
    DELETE FROM score WHERE cId = OLD.cId;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `score`
--

DROP TABLE IF EXISTS `score`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `score` (
  `sId` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '学号',
  `cId` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '课程号',
  `score` int DEFAULT NULL COMMENT '分数',
  PRIMARY KEY (`sId`,`cId`),
  KEY `cId` (`cId`),
  CONSTRAINT `score_ibfk_1` FOREIGN KEY (`sId`) REFERENCES `student` (`sId`),
  CONSTRAINT `score_ibfk_2` FOREIGN KEY (`cId`) REFERENCES `course` (`cId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `score`
--

LOCK TABLES `score` WRITE;
/*!40000 ALTER TABLE `score` DISABLE KEYS */;
INSERT INTO `score` VALUES ('2023001','C001',85),('2023001','C002',92),('2023001','C003',78),('2023001','C004',88),('2023002','C001',78),('2023002','C003',85),('2023002','C005',90),('2023003','C001',92),('2023003','C002',88),('2023003','C004',95),('2023004','C001',75),('2023004','C002',82),('2023004','C006',91),('2023005','C001',68),('2023005','C003',75),('2023005','C007',88),('2023006','C002',95),('2023006','C004',87),('2023006','C008',92),('2023007','C001',90),('2023007','C005',85),('2023008','C003',78),('2023008','C006',82),('2023009','C001',88),('2023009','C007',95),('2023010','C002',76),('2023010','C008',89),('2023011','C004',93),('2023011','C005',87),('2023012','C001',82),('2023012','C006',90);
/*!40000 ALTER TABLE `score` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student`
--

DROP TABLE IF EXISTS `student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `student` (
  `sId` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '学号',
  `name` varchar(20) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '姓名',
  `sex` varchar(5) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '性别',
  `age` int DEFAULT NULL COMMENT '年龄',
  `major` varchar(30) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '专业',
  `classId` varchar(20) COLLATE utf8mb4_unicode_ci DEFAULT NULL COMMENT '班级号',
  PRIMARY KEY (`sId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student`
--

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES ('2023001','张三','男',20,'计算机科学','CS2101'),('2023002','李四','女',19,'软件工程','SE2102'),('2023003','王五','男',21,'网络工程','NE2103'),('2023004','赵六','女',20,'计算机科学','CS2101'),('2023005','孙七','男',22,'软件工程','SE2102'),('2023006','周八','女',19,'网络工程','NE2103'),('2023007','吴九','男',21,'计算机科学','CS2101'),('2023008','郑十','女',20,'软件工程','SE2102'),('2023009','钱十一','男',19,'网络工程','NE2103'),('2023010','刘十二','女',22,'计算机科学','CS2101'),('2023011','陈十三','男',20,'软件工程','SE2102'),('2023012','杨十四','女',21,'网络工程','NE2103');
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trg_student_delete` BEFORE DELETE ON `student` FOR EACH ROW BEGIN
    -- 自动删除该学生的所有成绩
    DELETE FROM score WHERE sId = OLD.sId;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Temporary view structure for view `v_course_stat`
--

DROP TABLE IF EXISTS `v_course_stat`;
/*!50001 DROP VIEW IF EXISTS `v_course_stat`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `v_course_stat` AS SELECT 
 1 AS `课程号`,
 1 AS `课程名称`,
 1 AS `任课教师`,
 1 AS `学分`,
 1 AS `选课人数`,
 1 AS `平均分`,
 1 AS `最高分`,
 1 AS `最低分`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `v_student_score`
--

DROP TABLE IF EXISTS `v_student_score`;
/*!50001 DROP VIEW IF EXISTS `v_student_score`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `v_student_score` AS SELECT 
 1 AS `学号`,
 1 AS `姓名`,
 1 AS `专业`,
 1 AS `课程名称`,
 1 AS `任课教师`,
 1 AS `分数`,
 1 AS `等级`*/;
SET character_set_client = @saved_cs_client;

--
-- Current Database: `student_db`
--

USE `student_db`;

--
-- Final view structure for view `v_course_stat`
--

/*!50001 DROP VIEW IF EXISTS `v_course_stat`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_course_stat` AS select `c`.`cId` AS `课程号`,`c`.`name` AS `课程名称`,`c`.`teacher` AS `任课教师`,`c`.`credit` AS `学分`,count(`s`.`sId`) AS `选课人数`,round(avg(`s`.`score`),2) AS `平均分`,max(`s`.`score`) AS `最高分`,min(`s`.`score`) AS `最低分` from (`course` `c` left join `score` `s` on((`c`.`cId` = `s`.`cId`))) group by `c`.`cId`,`c`.`name`,`c`.`teacher`,`c`.`credit` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `v_student_score`
--

/*!50001 DROP VIEW IF EXISTS `v_student_score`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `v_student_score` AS select `s`.`sId` AS `学号`,`st`.`name` AS `姓名`,`st`.`major` AS `专业`,`c`.`name` AS `课程名称`,`c`.`teacher` AS `任课教师`,`s`.`score` AS `分数`,(case when (`s`.`score` >= 90) then '优秀' when (`s`.`score` >= 80) then '良好' when (`s`.`score` >= 70) then '中等' when (`s`.`score` >= 60) then '及格' else '不及格' end) AS `等级` from ((`score` `s` join `student` `st` on((`s`.`sId` = `st`.`sId`))) join `course` `c` on((`s`.`cId` = `c`.`cId`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-09-18 20:52:26
