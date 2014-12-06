SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema parkinglots
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `parkinglots` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `parkinglots` ;

-- -----------------------------------------------------
-- Table `parkinglots`.`maclist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`maclist` (
  `macid` INT NOT NULL,
  `serialnum` VARCHAR(128) NOT NULL DEFAULT 'INVALID',
  `name` VARCHAR(128) NULL DEFAULT 'INVALID',
  `info` VARCHAR(128) NULL DEFAULT 'EMPTY',
  `firmwareversion` INT NULL DEFAULT -1,
  `dstip` VARCHAR(64) NULL DEFAULT 'INVALID',
  `dstport` INT NULL DEFAULT -1,
  `ieeeadd` VARCHAR(48) NULL,
  `panid` VARCHAR(16) NULL,
  `epanid` VARCHAR(16) NULL,
  `ifregisted` TINYINT NULL DEFAULT 0,
  `ifconnected` TINYINT NULL DEFAULT 0,
  `sensornum` INT NULL DEFAULT 0,
  `relaynum` INT NULL DEFAULT 0,
  `ansensornum` INT NULL DEFAULT 0,
  `anrelaynum` INT NULL DEFAULT 0,
  PRIMARY KEY (`macid`),
  INDEX `idx_macinfo_serialnum` (`serialnum` ASC),
  INDEX `idx_macinfo_name` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`userlist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`userlist` (
  `userid` INT NOT NULL,
  `usertype` INT NULL,
  `username` VARCHAR(128) NULL,
  `password` VARCHAR(128) NULL,
  PRIMARY KEY (`userid`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`parklist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`parklist` (
  `parkid` INT NOT NULL,
  `parkname` VARCHAR(128) NULL,
  `parkinfo` VARCHAR(255) NULL,
  `parkposition` VARCHAR(255) NULL,
  PRIMARY KEY (`parkid`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`uprelations`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`uprelations` (
  `userid` INT NOT NULL,
  `parkid` INT NOT NULL,
  PRIMARY KEY (`userid`, `parkid`),
  INDEX `fk_upr_parkid_idx` (`parkid` ASC),
  CONSTRAINT `fk_upr_userid`
    FOREIGN KEY (`userid`)
    REFERENCES `parkinglots`.`userlist` (`userid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_upr_parkid`
    FOREIGN KEY (`parkid`)
    REFERENCES `parkinglots`.`parklist` (`parkid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`pmrelations`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`pmrelations` (
  `parkid` INT NOT NULL,
  `macid` INT NOT NULL,
  PRIMARY KEY (`parkid`, `macid`),
  INDEX `fk_pmr_macid_idx` (`macid` ASC),
  CONSTRAINT `fk_pmr_parkid`
    FOREIGN KEY (`parkid`)
    REFERENCES `parkinglots`.`parklist` (`parkid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_pmr_macid`
    FOREIGN KEY (`macid`)
    REFERENCES `parkinglots`.`maclist` (`macid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`sensorlist`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`sensorlist` (
  `deviceid` CHAR(48) NOT NULL,
  `macid` INT NOT NULL,
  `devicetype` TINYINT NULL DEFAULT -1,
  `no` VARCHAR(128) NULL DEFAULT -1,
  `devicename` VARCHAR(128) NULL,
  `deviceinfo` VARCHAR(128) NULL,
  `occupied` TINYINT NULL,
  `status` INT NULL DEFAULT -1,
  `temperature` INT NULL,
  `batteryvoltage` INT NULL,
  `phyaddress` VARCHAR(128) NULL,
  `createtime` DATETIME NOT NULL DEFAULT '1999-12-31 23:50:50',
  `lastacttime` DATETIME NULL,
  `lastdetailpara` VARCHAR(255) NULL,
  PRIMARY KEY (`deviceid`),
  INDEX `fk_sensorlist_macid_idx` (`macid` ASC),
  INDEX `idx_sensorlist_lastact` (`lastacttime` ASC),
  INDEX `idx_sensorlist_devname` (`devicename` ASC),
  INDEX `idx_sensorlist_occp` (`occupied` ASC),
  CONSTRAINT `fk_sensorlist_macid`
    FOREIGN KEY (`macid`)
    REFERENCES `parkinglots`.`maclist` (`macid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`sensorevent`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`sensorevent` (
  `evtid` INT NOT NULL,
  `deviceid` CHAR(48) NOT NULL,
  `eventid` INT NULL,
  `eventparamid` INT NULL,
  `eventparamtype` INT NULL,
  `eventparamvalue` TEXT NULL,
  `eventtime` DATETIME NULL,
  PRIMARY KEY (`evtid`),
  INDEX `idx_sensor_tm` (`eventtime` ASC),
  CONSTRAINT `fk_sensorevent_deviceid`
    FOREIGN KEY (`deviceid`)
    REFERENCES `parkinglots`.`sensorlist` (`deviceid`)
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `parkinglots`.`macevent`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`macevent` (
  `evtid` INT NOT NULL AUTO_INCREMENT,
  `macevt_sourceid` INT UNSIGNED NULL,
  `macevt_destinid` INT UNSIGNED NULL,
  `macevt_msgtype` INT UNSIGNED NULL,
  `macevt_donecode` INT NULL,
  `macevt_ip` VARCHAR(255) NULL,
  `macevt_comment` VARCHAR(255) NULL,
  `macevt_logtime` DATETIME NOT NULL,
  PRIMARY KEY (`evtid`),
  INDEX `idx_macevt_sourceid` (`macevt_sourceid` ASC),
  INDEX `idx_macevt_dstid` (`macevt_destinid` ASC),
  INDEX `idx_macevt_evttime` (`macevt_logtime` ASC))
ENGINE = InnoDB;

USE `parkinglots` ;

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_user_parks`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_user_parks` (`userid` INT, `parks` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_user` (`parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT, `password` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_macs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_macs` (`parkid` INT, `macs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_park_user` (`macid` INT, `serialnum` INT, `name` INT, `info` INT, `firmwareversion` INT, `dstip` INT, `dstport` INT, `ieeeadd` INT, `panid` INT, `epanid` INT, `ifregisted` INT, `ifconnected` INT, `parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT, `password` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_dev_mac_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_dev_mac_park_user` (`deviceid` INT, `devicetype` INT, `no` INT, `devicename` INT, `deviceinfo` INT, `occupied` INT, `status` INT, `temperature` INT, `batteryvoltage` INT, `phyaddress` INT, `createtime` INT, `lastacttime` INT, `lastdetailpara` INT, `macid` INT, `serialnum` INT, `name` INT, `info` INT, `firmwareversion` INT, `dstip` INT, `dstport` INT, `ieeeadd` INT, `panid` INT, `epanid` INT, `ifregisted` INT, `ifconnected` INT, `parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT, `password` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_snr_mac_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_snr_mac_park_user` (`deviceid` INT, `devicetype` INT, `no` INT, `devicename` INT, `deviceinfo` INT, `occupied` INT, `status` INT, `temperature` INT, `batteryvoltage` INT, `phyaddress` INT, `createtime` INT, `lastacttime` INT, `lastdetailpara` INT, `macid` INT, `serialnum` INT, `name` INT, `info` INT, `firmwareversion` INT, `dstip` INT, `dstport` INT, `ieeeadd` INT, `panid` INT, `epanid` INT, `ifregisted` INT, `ifconnected` INT, `parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT, `password` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_rly_mac_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_rly_mac_park_user` (`deviceid` INT, `devicetype` INT, `no` INT, `devicename` INT, `deviceinfo` INT, `occupied` INT, `status` INT, `temperature` INT, `batteryvoltage` INT, `phyaddress` INT, `createtime` INT, `lastacttime` INT, `lastdetailpara` INT, `macid` INT, `serialnum` INT, `name` INT, `info` INT, `firmwareversion` INT, `dstip` INT, `dstport` INT, `ieeeadd` INT, `panid` INT, `epanid` INT, `ifregisted` INT, `ifconnected` INT, `parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT, `password` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_devs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_devs` (`parkid` INT, `userid` INT, `devs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_snrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_snrs` (`parkid` INT, `userid` INT, `snrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_rlys`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_rlys` (`parkid` INT, `userid` INT, `rlys` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_anrlys`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_anrlys` (`parkid` INT, `userid` INT, `anrlys` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_idlesnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_idlesnrs` (`parkid` INT, `userid` INT, `idlesnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_busysnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_busysnrs` (`parkid` INT, `userid` INT, `busysnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_park_ansnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_park_ansnrs` (`parkid` INT, `userid` INT, `ansnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_summary_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_summary_park_user` (`parkid` INT, `parkname` INT, `idlesnrs` INT, `busysnrs` INT, `parkinfo` INT, `parkposition` INT, `macs` INT, `devs` INT, `snrs` INT, `rlys` INT, `anrlys` INT, `ansnrs` INT, `userid` INT, `username` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_devs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_devs` (`macid` INT, `parkid` INT, `userid` INT, `devs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_snrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_snrs` (`macid` INT, `parkid` INT, `userid` INT, `snrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_rlys`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_rlys` (`macid` INT, `parkid` INT, `userid` INT, `rlys` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_anrlys`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_anrlys` (`macid` INT, `parkid` INT, `userid` INT, `anrlys` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_idlesnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_idlesnrs` (`macid` INT, `parkid` INT, `userid` INT, `idlesnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_busysnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_busysnrs` (`macid` INT, `parkid` INT, `userid` INT, `busysnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_mac_ansnrs`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_mac_ansnrs` (`macid` INT, `parkid` INT, `userid` INT, `ansnrs` INT);

-- -----------------------------------------------------
-- Placeholder table for view `parkinglots`.`view_summary_mac_park_user`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `parkinglots`.`view_summary_mac_park_user` (`macid` INT, `idlesnrs` INT, `busysnrs` INT, `serialnum` INT, `name` INT, `info` INT, `firmwareversion` INT, `dstip` INT, `dstport` INT, `ieeeadd` INT, `panid` INT, `epanid` INT, `ifregisted` INT, `ifconnected` INT, `devs` INT, `snrs` INT, `rlys` INT, `anrlys` INT, `ansnrs` INT, `parkid` INT, `parkname` INT, `parkinfo` INT, `parkposition` INT, `userid` INT, `usertype` INT, `username` INT);

-- -----------------------------------------------------
-- View `parkinglots`.`view_user_parks`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_user_parks`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_user_parks` AS
select
uprelations.userid,
count(uprelations.parkid) as parks
from
	uprelations group by userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_user` AS
select
parklist.parkid as parkid,
parklist.parkname AS parkname,
parklist.parkinfo AS parkinfo,
parklist.parkposition AS parkposition,
uprelations.userid AS userid,
userlist.usertype AS usertype,
userlist.username AS username,
userlist.password AS password
from
(
	 parklist
	 left join uprelations on uprelations.parkid = parklist.parkid
	 left join userlist on uprelations.userid = userlist.userid
);


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_macs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_macs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_macs` AS
select
pmrelations.parkid,
count(pmrelations.macid) as macs
from
	pmrelations group by parkid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_park_user` AS
select
maclist.macid AS macid,
maclist.serialnum AS serialnum,
maclist.name AS name,
maclist.info AS info,
maclist.firmwareversion AS firmwareversion,
maclist.dstip AS dstip,
maclist.dstport AS dstport,
maclist.ieeeadd AS ieeeadd,
maclist.panid AS panid,
maclist.epanid AS epanid,
maclist.ifregisted AS ifregisted,
maclist.ifconnected AS ifconnected,
pmrelations.parkid AS parkid,
parklist.parkname AS parkname,
parklist.parkinfo AS parkinfo,
parklist.parkposition AS parkposition,
uprelations.userid AS userid,
userlist.usertype AS usertype,
userlist.username AS username,
userlist.password AS password
from
(
	 maclist
	 left join pmrelations on pmrelations.macid = maclist.macid
	 left join parklist on   parklist.parkid = pmrelations.parkid
	 left join uprelations on uprelations.parkid = parklist.parkid
	 left join userlist on uprelations.userid = userlist.userid
);


-- -----------------------------------------------------
-- View `parkinglots`.`view_dev_mac_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_dev_mac_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_dev_mac_park_user` AS
select
sensorlist.deviceid AS deviceid,
sensorlist.devicetype AS devicetype,
sensorlist.no AS no,
sensorlist.devicename AS devicename,
sensorlist.deviceinfo AS deviceinfo,
sensorlist.occupied AS occupied,
sensorlist.status AS status,
sensorlist.temperature AS temperature,
sensorlist.batteryvoltage AS batteryvoltage,
sensorlist.phyaddress AS phyaddress,
sensorlist.createtime AS createtime,
sensorlist.lastacttime AS lastacttime,
sensorlist.lastdetailpara AS lastdetailpara,
maclist.macid AS macid,
maclist.serialnum AS serialnum,
maclist.name AS name,
maclist.info AS info,
maclist.firmwareversion AS firmwareversion,
maclist.dstip AS dstip,
maclist.dstport AS dstport,
maclist.ieeeadd AS ieeeadd,
maclist.panid AS panid,
maclist.epanid AS epanid,
maclist.ifregisted AS ifregisted,
maclist.ifconnected AS ifconnected,
pmrelations.parkid AS parkid,
parklist.parkname AS parkname,
parklist.parkinfo AS parkinfo,
parklist.parkposition AS parkposition,
uprelations.userid AS userid,
userlist.usertype AS usertype,
userlist.username AS username,
userlist.password AS password
from
(
	 sensorlist
	 left join maclist  on sensorlist.macid = maclist.macid
	 left join pmrelations on pmrelations.macid = maclist.macid
	 left join parklist on   parklist.parkid = pmrelations.parkid
	 left join uprelations on uprelations.parkid = parklist.parkid
	 left join userlist on uprelations.userid = userlist.userid
);


-- -----------------------------------------------------
-- View `parkinglots`.`view_snr_mac_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_snr_mac_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_snr_mac_park_user` AS
select
sensorlist.deviceid AS deviceid,
sensorlist.devicetype AS devicetype,
sensorlist.no AS no,
sensorlist.devicename AS devicename,
sensorlist.deviceinfo AS deviceinfo,
sensorlist.occupied AS occupied,
sensorlist.status AS status,
sensorlist.temperature AS temperature,
sensorlist.batteryvoltage AS batteryvoltage,
sensorlist.phyaddress AS phyaddress,
sensorlist.createtime AS createtime,
sensorlist.lastacttime AS lastacttime,
sensorlist.lastdetailpara AS lastdetailpara,
maclist.macid AS macid,
maclist.serialnum AS serialnum,
maclist.name AS name,
maclist.info AS info,
maclist.firmwareversion AS firmwareversion,
maclist.dstip AS dstip,
maclist.dstport AS dstport,
maclist.ieeeadd AS ieeeadd,
maclist.panid AS panid,
maclist.epanid AS epanid,
maclist.ifregisted AS ifregisted,
maclist.ifconnected AS ifconnected,
pmrelations.parkid AS parkid,
parklist.parkname AS parkname,
parklist.parkinfo AS parkinfo,
parklist.parkposition AS parkposition,
uprelations.userid AS userid,
userlist.usertype AS usertype,
userlist.username AS username,
userlist.password AS password
from
(
	 sensorlist
	 left join maclist  on sensorlist.macid = maclist.macid
	 left join pmrelations on pmrelations.macid = maclist.macid
	 left join parklist on   parklist.parkid = pmrelations.parkid
	 left join uprelations on uprelations.parkid = parklist.parkid
	 left join userlist on uprelations.userid = userlist.userid
)
where sensorlist.deviceid like '0100%'
;

-- -----------------------------------------------------
-- View `parkinglots`.`view_rly_mac_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_rly_mac_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_rly_mac_park_user` AS
select
sensorlist.deviceid AS deviceid,
sensorlist.devicetype AS devicetype,
sensorlist.no AS no,
sensorlist.devicename AS devicename,
sensorlist.deviceinfo AS deviceinfo,
sensorlist.occupied AS occupied,
sensorlist.status AS status,
sensorlist.temperature AS temperature,
sensorlist.batteryvoltage AS batteryvoltage,
sensorlist.phyaddress AS phyaddress,
sensorlist.createtime AS createtime,
sensorlist.lastacttime AS lastacttime,
sensorlist.lastdetailpara AS lastdetailpara,
maclist.macid AS macid,
maclist.serialnum AS serialnum,
maclist.name AS name,
maclist.info AS info,
maclist.firmwareversion AS firmwareversion,
maclist.dstip AS dstip,
maclist.dstport AS dstport,
maclist.ieeeadd AS ieeeadd,
maclist.panid AS panid,
maclist.epanid AS epanid,
maclist.ifregisted AS ifregisted,
maclist.ifconnected AS ifconnected,
pmrelations.parkid AS parkid,
parklist.parkname AS parkname,
parklist.parkinfo AS parkinfo,
parklist.parkposition AS parkposition,
uprelations.userid AS userid,
userlist.usertype AS usertype,
userlist.username AS username,
userlist.password AS password
from
(
	 sensorlist
	 left join maclist  on sensorlist.macid = maclist.macid
	 left join pmrelations on pmrelations.macid = maclist.macid
	 left join parklist on   parklist.parkid = pmrelations.parkid
	 left join uprelations on uprelations.parkid = parklist.parkid
	 left join userlist on uprelations.userid = userlist.userid
)
where sensorlist.deviceid like '0101%'
;

-- -----------------------------------------------------
-- View `parkinglots`.`view_park_devs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_devs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_devs` AS
select
view_dev_mac_park_user.parkid,
view_dev_mac_park_user.userid,
count(view_dev_mac_park_user.deviceid) as devs
from
	view_dev_mac_park_user group by parkid,userid;



-- -----------------------------------------------------
-- View `parkinglots`.`view_park_snrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_snrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_snrs` AS
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as snrs
from
	view_snr_mac_park_user group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_rlys`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_rlys`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_rlys` AS
select
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as rlys
from
	view_rly_mac_park_user group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_anrlys`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_anrlys`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_anrlys` AS
select
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as anrlys
from
	view_rly_mac_park_user where status > 0 group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_idlesnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_idlesnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_idlesnrs` AS
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as idlesnrs
from
	view_snr_mac_park_user where occupied = 0 and status = 0 group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_busysnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_busysnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_busysnrs` AS
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as busysnrs
from
	view_snr_mac_park_user where occupied >0 and status = 0 group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_park_ansnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_park_ansnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_park_ansnrs` AS
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as ansnrs
from
	view_snr_mac_park_user where  status > 0 group by parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_summary_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_summary_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_summary_park_user` AS
select
		view_park_user.parkid as parkid,
		view_park_user.parkname AS parkname,
		view_park_idlesnrs.idlesnrs as idlesnrs,
		view_park_busysnrs.busysnrs as busysnrs,
		view_park_user.parkinfo AS parkinfo,
		view_park_user.parkposition AS parkposition,
		view_park_macs.macs as macs,
		view_park_devs.devs as devs,
		view_park_snrs.snrs as snrs,
		view_park_rlys.rlys as rlys,
		view_park_anrlys.anrlys as anrlys,
		view_park_ansnrs.ansnrs as ansnrs,
		view_park_user.userid AS userid,
		view_park_user.username AS username
from
		view_park_user
			left join view_park_macs on view_park_user.parkid = view_park_macs.parkid
			left join view_park_devs on view_park_user.parkid = view_park_devs.parkid and view_park_user.userid =view_park_devs.userid
			left join view_park_snrs on view_park_user.parkid = view_park_snrs.parkid and view_park_user.userid =view_park_snrs.userid
			left join view_park_rlys on view_park_user.parkid = view_park_rlys.parkid and view_park_user.userid =view_park_rlys.userid
			left join view_park_anrlys on view_park_user.parkid = view_park_anrlys.parkid and view_park_user.userid =view_park_anrlys.userid
			left join view_park_idlesnrs on view_park_user.parkid = view_park_idlesnrs.parkid and view_park_user.userid =view_park_idlesnrs.userid
			left join view_park_busysnrs on view_park_user.parkid = view_park_busysnrs.parkid and view_park_user.userid =view_park_busysnrs.userid
			left join view_park_ansnrs on view_park_user.parkid = view_park_ansnrs.parkid and view_park_user.userid =view_park_ansnrs.userid
;

-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_devs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_devs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_devs` AS
select
view_dev_mac_park_user.macid,
view_dev_mac_park_user.parkid,
view_dev_mac_park_user.userid,
count(view_dev_mac_park_user.deviceid) as devs
from
	view_dev_mac_park_user group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_snrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_snrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_snrs` AS

select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as snrs
from
	view_snr_mac_park_user group by macid,parkid,userid;



-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_rlys`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_rlys`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_rlys` AS

select
view_rly_mac_park_user.macid,
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as rlys
from
	view_rly_mac_park_user group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_anrlys`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_anrlys`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_anrlys` AS

select
view_rly_mac_park_user.macid,
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as anrlys
from
	view_rly_mac_park_user where status > 0 group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_idlesnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_idlesnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_idlesnrs` AS

select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as idlesnrs
from
	view_snr_mac_park_user where occupied = 0 and status = 0 group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_busysnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_busysnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_busysnrs` AS

select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as busysnrs
from
	view_snr_mac_park_user where occupied >0 and status = 0 group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_mac_ansnrs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_mac_ansnrs`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_mac_ansnrs` AS

select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as ansnrs
from
	view_snr_mac_park_user where  status > 0 group by macid,parkid,userid;


-- -----------------------------------------------------
-- View `parkinglots`.`view_summary_mac_park_user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `parkinglots`.`view_summary_mac_park_user`;
USE `parkinglots`;
CREATE  OR REPLACE VIEW `view_summary_mac_park_user` AS

select
		view_mac_park_user.macid as macid,
		view_mac_idlesnrs.idlesnrs as idlesnrs,
		view_mac_busysnrs.busysnrs as busysnrs,
		view_mac_park_user.serialnum AS serialnum,
		view_mac_park_user.name AS name,
		view_mac_park_user.info AS info,
		view_mac_park_user.firmwareversion AS firmwareversion,
		view_mac_park_user.dstip AS dstip,
		view_mac_park_user.dstport AS dstport,
		view_mac_park_user.ieeeadd AS ieeeadd,
		view_mac_park_user.panid AS panid,
		view_mac_park_user.epanid AS epanid,
		view_mac_park_user.ifregisted AS ifregisted,
		view_mac_park_user.ifconnected AS ifconnected,
		view_mac_devs.devs as devs,
		view_mac_snrs.snrs as snrs,
		view_mac_rlys.rlys as rlys,
		view_mac_anrlys.anrlys as anrlys,
		view_mac_ansnrs.ansnrs as ansnrs,
		view_mac_park_user.parkid AS parkid,
		view_mac_park_user.parkname AS parkname,
		view_mac_park_user.parkinfo AS parkinfo,
		view_mac_park_user.parkposition AS parkposition,
		view_mac_park_user.userid AS userid,
		view_mac_park_user.usertype AS usertype,
		view_mac_park_user.username AS username
from
		view_mac_park_user
			left join view_mac_devs on view_mac_park_user.parkid = view_mac_devs.parkid and view_mac_park_user.userid =view_mac_devs.userid and view_mac_park_user.macid =view_mac_devs.macid
			left join view_mac_snrs on view_mac_park_user.parkid = view_mac_snrs.parkid and view_mac_park_user.userid =view_mac_snrs.userid and view_mac_park_user.macid =view_mac_snrs.macid
			left join view_mac_rlys on view_mac_park_user.parkid = view_mac_rlys.parkid and view_mac_park_user.userid =view_mac_rlys.userid and view_mac_park_user.macid =view_mac_rlys.macid
			left join view_mac_anrlys on view_mac_park_user.parkid = view_mac_anrlys.parkid and view_mac_park_user.userid =view_mac_anrlys.userid and view_mac_park_user.macid =view_mac_anrlys.macid
			left join view_mac_idlesnrs on view_mac_park_user.parkid = view_mac_idlesnrs.parkid and view_mac_park_user.userid =view_mac_idlesnrs.userid and view_mac_park_user.macid =view_mac_idlesnrs.macid
			left join view_mac_busysnrs on view_mac_park_user.parkid = view_mac_busysnrs.parkid and view_mac_park_user.userid =view_mac_busysnrs.userid and view_mac_park_user.macid =view_mac_busysnrs.macid
			left join view_mac_ansnrs on view_mac_park_user.parkid = view_mac_ansnrs.parkid and view_mac_park_user.userid =view_mac_ansnrs.userid and view_mac_park_user.macid =view_mac_ansnrs.macid
;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
