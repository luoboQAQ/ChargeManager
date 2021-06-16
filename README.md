# 机房收费管理系统

基于Qt+MySQL的机房收费管理系统。

## 表的设置

### 管理员表

#### 建表语句

```sql
CREATE TABLE `admin` (
  `aid` char(5),
  `aname` varchar(20),
  `apasswd` varchar(20),
  PRIMARY KEY (`aid`)
);
```

#### 测试数据

```sql
INSERT INTO `admin` (`aid`, `aname`, `apasswd`) VALUES ('001', 'admin', '001');
```

### 学生表

#### 建表语句

```sql
CREATE TABLE `student` (
  `sno` char(5),
  `sname` varchar(10),
  `spasswd` varchar(20),
  `sdc` varchar(10),
  `sclass` varchar(20),
  `sage` int,
  `ssex` char(2),
  PRIMARY KEY (`sno`)
);
```

#### 测试数据插入

```sql
INSERT INTO `student` (`sno`, `sname`, `spasswd`, `sdc`, `sclass`, `sage`, `ssex`) VALUES ('20001', '小红', '123', '计算机', '1901', 20, '女');
```

### 上机卡表

#### 建表语句

```sql
CREATE TABLE `card` (
  `cardid` char(5),
  `createday` datetime,
  `sno` char(5),
  `banlance` double,
  `state` tinyint(1),
  PRIMARY KEY (`cardid`),
  FOREIGN KEY (`sno`) REFERENCES `student` (`sno`) ON DELETE CASCADE ON UPDATE RESTRICT
) ;
```

#### 测试数据插入

```sql
INSERT INTO `card` (`cardid`, `createday`, `sno`, `banlance`, `state`) VALUES ('001', '2021-05-08 00:00:00', '20001', 10, 1);
```

### 计算机表

#### 建表语句

```sql
CREATE TABLE `computer` (
  `computer_id` char(5),
  `computer_state` varchar(25),
  PRIMARY KEY (`computer_id`)
);
```

#### 测试数据插入

```sql
INSERT INTO `computer` (`computer_id`, `computer_state`) VALUES ('S001', '空闲');
INSERT INTO `computer` (`computer_id`, `computer_state`) VALUES ('S002', '空闲');
INSERT INTO `computer` (`computer_id`, `computer_state`) VALUES ('S003', '空闲');
INSERT INTO `computer` (`computer_id`, `computer_state`) VALUES ('S004', '空闲');
INSERT INTO `computer` (`computer_id`, `computer_state`) VALUES ('S005', '空闲');
```

### 上机记录表

#### 建表语句

```sql
CREATE TABLE `record` (
  `serial_num` varchar(20),
  `cardid` char(5),
  `stime` datetime,
  `ctime` time,
  `cost` decimal(6,1),
  `computer_id` char(5),
  `is_using` tinyint(1),
  PRIMARY KEY (`serial_num`),
  FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE RESTRICT,
  FOREIGN KEY (`computer_id`) REFERENCES `computer` (`computer_id`) ON DELETE RESTRICT
);
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

### 挂失记录表

#### 建表语句

```sql
CREATE TABLE `loss_record` (
  `serial_num` varchar(20)NOT NULL,
  `ltime` datetime,
  `cardid` char(5),
  `opeartor_id` char(5),
  `state` tinyint(1),
  PRIMARY KEY (`serial_num`),
  FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE CASCADE
);
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
  `serial_num` varchar(20),
  `ctime` datetime,
  `cardid` char(5),
  `money` double,
  `opeartor_id` char(5),
  `state` int,
  PRIMARY KEY (`serial_num`),
  FOREIGN KEY (`cardid`) REFERENCES `card` (`cardid`) ON DELETE CASCADE
);
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
  `serial_num` varchar(20),
  `stime` datetime,
  `aid` char(5),
  `change_way` int,
  `sno` char(5),
  PRIMARY KEY (`serial_num`),
  FOREIGN KEY (`aid`) REFERENCES `admin` (`aid`),
  FOREIGN KEY (`sno`) REFERENCES `student` (`sno`) ON DELETE RESTRICT ON UPDATE RESTRICT
);
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
