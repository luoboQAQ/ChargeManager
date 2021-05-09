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
  `stime` datetime DEFAULT NULL,
  `ctime` time DEFAULT NULL,
  `is_using` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`serial_num`),
  KEY `cardid` (`cardid`),
  CONSTRAINT `record_ibfk_1` FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
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

