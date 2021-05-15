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
  KEY `sno` (`sno`),
  CONSTRAINT `card_ibfk_1` FOREIGN KEY (`sno`) REFERENCES `student` (`sno`)
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
  CONSTRAINT `record_ibfk_1` FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE RESTRICT
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



### 用户表

#### 建表语句

```sql
CREATE TABLE `sa` (
  `user` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`user`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

### 学生表

#### 建表语句

```sql
CREATE TABLE `student` (
  `sno` char(5) NOT NULL,
  `sname` varchar(10) DEFAULT NULL,
  `sdc` varchar(10) DEFAULT NULL,
  `sclass` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci DEFAULT NULL,
  `sage` int DEFAULT NULL,
  `ssex` char(2) DEFAULT NULL,
  PRIMARY KEY (`sno`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
```

#### 测试数据插入

```sql
INSERT INTO `chm`.`student` (`sno`, `sname`, `sdc`, `sclass`, `sage`, `ssex`) VALUES ('20001', '小红', '计算机', '1901', 20, '女');
```

### 挂失记录表

#### 建表语句

```sql
CREATE TABLE `loss_record` (
  `ltime` datetime NOT NULL,
  `cardid` char(5) DEFAULT NULL,
  `opeartor_id` char(5) DEFAULT NULL,
  `state` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`ltime`)
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
  PRIMARY KEY (`ctime`) USING BTREE
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

