--view_user_parks
select
uprelations.userid,
count(uprelations.parkid) as parks
from
	uprelations group by userid;

--view_park_user
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


--view_park_macs
select
pmrelations.parkid,
count(pmrelations.macid) as macs
from
	pmrelations group by parkid;

--view_mac_park_user
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
maclist.sensornum AS sensornum,
maclist.relaynum AS relaynum,
maclist.ansensornum AS ansensornum,
maclist.anrelaynum AS anrelaynum,
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


--view_dev_mac_park_user
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
maclist.sensornum AS sensornum,
maclist.relaynum AS relaynum,
maclist.ansensornum AS ansensornum,
maclist.anrelaynum AS anrelaynum,
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

--view_snr_mac_park_user
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
maclist.sensornum AS sensornum,
maclist.relaynum AS relaynum,
maclist.ansensornum AS ansensornum,
maclist.anrelaynum AS anrelaynum,
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
--view_rly_mac_park_user
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
maclist.sensornum AS sensornum,
maclist.relaynum AS relaynum,
maclist.ansensornum AS ansensornum,
maclist.anrelaynum AS anrelaynum,
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

--view_park_devs
select
view_dev_mac_park_user.parkid,
view_dev_mac_park_user.userid,
count(view_dev_mac_park_user.deviceid) as devs
from
	view_dev_mac_park_user group by parkid,userid;



--view_park_snrs
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as snrs
from
	view_snr_mac_park_user group by parkid,userid;


--view_park_rlys
select
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as rlys
from
	view_rly_mac_park_user group by parkid,userid;

--view_park_anrlys
select
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as anrlys
from
	view_rly_mac_park_user where status > 0 group by parkid,userid;

--view_park_idlesnrs
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as idlesnrs
from
	view_snr_mac_park_user where occupied = 0 and status = 0 group by parkid,userid;

--view_park_busysnrs
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as busysnrs
from
	view_snr_mac_park_user where occupied >0 and status = 0 group by parkid,userid;

--view_park_ansnrs
select
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as ansnrs
from
	view_snr_mac_park_user where  status > 0 group by parkid,userid;

--view_summary_park_user
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


--view_mac_devs
select
view_dev_mac_park_user.macid,
view_dev_mac_park_user.parkid,
view_dev_mac_park_user.userid,
count(view_dev_mac_park_user.deviceid) as devs
from
	view_dev_mac_park_user group by macid,parkid,userid;



--view_mac_snrs
select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as snrs
from
	view_snr_mac_park_user group by macid,parkid,userid;


--view_mac_rlys
select
view_rly_mac_park_user.macid,
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as rlys
from
	view_rly_mac_park_user group by macid,parkid,userid;

--view_mac_anrlys
select
view_rly_mac_park_user.macid,
view_rly_mac_park_user.parkid,
view_rly_mac_park_user.userid,
count(view_rly_mac_park_user.deviceid) as anrlys
from
	view_rly_mac_park_user where status > 0 group by macid,parkid,userid;

--view_mac_idlesnrs
select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as idlesnrs
from
	view_snr_mac_park_user where occupied = 0 and status = 0 group by macid,parkid,userid;

--view_mac_busysnrs
select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as busysnrs
from
	view_snr_mac_park_user where occupied >0 and status = 0 group by macid,parkid,userid;

--view_mac_ansnrs
select
view_snr_mac_park_user.macid,
view_snr_mac_park_user.parkid,
view_snr_mac_park_user.userid,
count(view_snr_mac_park_user.deviceid) as ansnrs
from
	view_snr_mac_park_user where  status > 0 group by macid,parkid,userid;

--view_summary_mac_park_user
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
