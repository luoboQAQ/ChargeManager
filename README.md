# 机房收费管理系统

基于Qt+MySQL的机房收费管理系统。

## 表的设置

### 上机卡表

#### 建表语句

```sql
CREATE TABLE `card` (
  `cardid` char(5) NOT NULL,
  `createday` datetime DEFAULT NULL,
  `sno` char(5) NOT NULL,
  `banlance` double DEFAULT NULL,
  `state` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`cardid`),
  KEY `card_ibfk_1` (`sno`),
  CONSTRAINT `card_ibfk_1` FOREIGN KEY (`sno`) REFERENCES `student` (`sno`) ON DELETE CASCADE ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 测试数据插入

```sql
INSERT INTO `chm`.`card` (`cardid`, `createday`, `sno`, `banlance`, `state`) VALUES ('001', '2021-05-08 00:00:00', '20001', 10, 1);
```

### 计算机表

#### 建表语句

```sql
CREATE TABLE `computer` (
  `computer_id` char(5) NOT NULL,
  `computer_state` varchar(25) DEFAULT NULL,
  PRIMARY KEY (`computer_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 测试数据插入

```sql
INSERT INTO `chm`.`computer` (`computer_id`, `computer_state`) VALUES ('S001', '空闲');
INSERT INTO `chm`.`computer` (`computer_id`, `computer_state`) VALUES ('S002', '空闲');
INSERT INTO `chm`.`computer` (`computer_id`, `computer_state`) VALUES ('S003', '空闲');
INSERT INTO `chm`.`computer` (`computer_id`, `computer_state`) VALUES ('S004', '空闲');
INSERT INTO `chm`.`computer` (`computer_id`, `computer_state`) VALUES ('S005', '空闲');
```

### 上机记录表

#### 建表语句

```sql
CREATE TABLE `record` (
  `serial_num` varchar(20) NOT NULL,
  `cardid` char(5) NOT NULL,
  `stime` datetime NOT NULL,
  `ctime` time NOT NULL,
  `cost` decimal(6,1) NOT NULL,
  `computer_id` char(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `is_using` tinyint(1) NOT NULL,
  PRIMARY KEY (`serial_num`),
  KEY `cardid` (`cardid`),
  KEY `computer_id` (`computer_id`),
  CONSTRAINT `record_ibfk_1` FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE RESTRICT,
  CONSTRAINT `record_ibfk_2` FOREIGN KEY (`computer_id`) REFERENCES `computer` (`computer_id`) ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 触发器

```sql
# 当插入上机记录时自动更新计算机状态
DROP TRIGGER IF EXISTS update_computer_i;
DELIMITER $
CREATE TRIGGER update_computer_i AFTER INSERT ON record FOR EACH ROW
BEGIN
IF NEW.is_using=1 THEN
	UPDATE computer SET computer_state='忙碌' WHERE computer_id=NEW.computer_id;
END IF;
END$
DELIMITER ;

# 当更新上机记录时自动更新计算机状态
DROP TRIGGER IF EXISTS update_computer_u;
DELIMITER $
CREATE TRIGGER update_computer_u AFTER UPDATE ON record FOR EACH ROW
BEGIN
IF NEW.is_using=0 THEN
	UPDATE computer SET computer_state='空闲' WHERE computer_id=NEW.computer_id;
END IF;
END$
DELIMITER ;
```



### 管理员表

#### 建表语句

```sql
CREATE TABLE `admin` (
  `aid` char(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `aname` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `apasswd` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`aid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 测试数据

```sql
INSERT INTO `chm`.`admin` (`aid`, `aname`, `apasswd`) VALUES ('001', 'admin', '001');
```

### 学生表

#### 建表语句

```sql
CREATE TABLE `student` (
  `sno` char(5) NOT NULL,
  `sname` varchar(10) DEFAULT NULL,
  `spasswd` varchar(20) NOT NULL,
  `sdc` varchar(10) DEFAULT NULL,
  `sclass` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `sage` int DEFAULT NULL,
  `ssex` char(2) DEFAULT NULL,
  PRIMARY KEY (`sno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 测试数据插入

```sql
INSERT INTO `chm`.`student` (`sno`, `sname`, `spasswd`, `sdc`, `sclass`, `sage`, `ssex`) VALUES ('20001', '小红', '123', '计算机', '1901', 20, '女');
```

### 挂失记录表

#### 建表语句

```sql
CREATE TABLE `loss_record` (
  `ltime` datetime NOT NULL,
  `cardid` char(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `opeartor_id` char(5) DEFAULT NULL,
  `state` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`ltime`,`cardid`) USING BTREE,
  KEY `cardid` (`cardid`),
  CONSTRAINT `loss_record_ibfk_1` FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 触发器

```sql
# 当插入挂失记录时自动更新卡状态
DROP TRIGGER IF EXISTS update_state;
DELIMITER $
CREATE TRIGGER update_state AFTER INSERT ON loss_record FOR EACH ROW
BEGIN
	CASE NEW.state
	WHEN 0 THEN
		UPDATE card SET state=0 WHERE cardid=NEW.cardid;
	WHEN 1 THEN
		UPDATE card SET state=1 WHERE cardid=NEW.cardid;
	END CASE;
END$
DELIMITER ;
```

### 充值记录表

#### 建表语句

```sql
CREATE TABLE `charge_record` (
  `ctime` datetime NOT NULL,
  `cardid` char(5) NOT NULL,
  `money` double NOT NULL,
  `opeartor_id` char(5) NOT NULL,
  `state` int NOT NULL,
  PRIMARY KEY (`ctime`,`cardid`) USING BTREE,
  KEY `cardid` (`cardid`),
  CONSTRAINT `charge_record_ibfk_1` FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 触发器

```sql
# 当插入充值记录时自动更新卡余额
DROP TRIGGER IF EXISTS update_balance;
DELIMITER $
CREATE TRIGGER update_balance AFTER INSERT ON charge_record FOR EACH ROW
BEGIN
	CASE NEW.state
	WHEN 0 THEN
		UPDATE card SET banlance=banlance-NEW.money WHERE cardid=NEW.cardid;
	WHEN 1 THEN
		UPDATE card SET banlance=banlance+NEW.money WHERE cardid=NEW.cardid;
	END CASE;
END$
DELIMITER ;
```

### 用户操作记录表

#### 建表语句

```sql
CREATE TABLE `user_record` (
  `stime` datetime NOT NULL,
  `aid` char(5) NOT NULL,
  `change_way` int DEFAULT NULL,
  `sno` char(5) DEFAULT NULL,
  PRIMARY KEY (`stime`,`aid`),
  KEY `aid` (`aid`),
  KEY `sno` (`sno`),
  CONSTRAINT `user_record_ibfk_1` FOREIGN KEY (`aid`) REFERENCES `admin` (`aid`),
  CONSTRAINT `user_record_ibfk_2` FOREIGN KEY (`sno`) REFERENCES `student` (`sno`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

## 视图的设置

### 登录视图

#### 建立语句

```sql
DROP VIEW IF EXISTS login;
CREATE VIEW login AS 
	SELECT sno AS user,spasswd AS passwd,1 AS tag 
	FROM student
UNION
	SELECT aid AS user,apasswd AS passwd,0 AS tag 
	FROM admin;
```

### 总花费视图

#### 建立语句

```sql
DROP VIEW IF EXISTS sumcost;
CREATE VIEW sumcost AS
SELECT cardid,DATE_FORMAT(stime, '%Y-%m-%d') day_time,SUM(cost) AS sumcost
FROM record 
WHERE is_using = 0 
GROUP BY cardid,day_time
ORDER BY cardid ASC,day_time ASC;
```

### 上机总时长视图

#### 建立语句

```sql
DROP VIEW IF EXISTS sumtime;
CREATE VIEW sumtime AS
SELECT cardid,DATE_FORMAT(stime, '%Y-%m-%d') day_time,SEC_TO_TIME(SUM(TIME_TO_SEC(ctime))) AS sumtime
FROM record
WHERE is_using=0
GROUP BY cardid,day_time
ORDER BY cardid ASC,day_time ASC;
```

### 平均使用时长视图

#### 建立语句

```sql
DROP VIEW IF EXISTS avgtime;
CREATE VIEW avgtime AS
SELECT cardid,DATE_FORMAT(stime, '%Y-%m-%d') day_time,SEC_TO_TIME(AVG(TIME_TO_SEC(ctime))) AS avgtime
FROM record
WHERE is_using=0
GROUP BY cardid,day_time
ORDER BY cardid ASC,day_time ASC;
```

### 上机次数视图

#### 建立语句

```sql
DROP VIEW IF EXISTS times;
CREATE VIEW times AS
SELECT cardid,DATE_FORMAT(stime, '%Y-%m-%d') day_time,COUNT(*) AS times
FROM record
WHERE is_using=0
GROUP BY cardid,day_time
ORDER BY cardid ASC,day_time ASC;
```
