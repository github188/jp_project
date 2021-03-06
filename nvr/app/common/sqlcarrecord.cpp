/***************************************************************************
 *   Copyright (C) 2015 by root   				   						   *
 *   ysgen0217@163.com   							   					   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "sqlcarrecord.h"
#include "../../library/util/log.h"
#include "../../library/util/system.h"
#include "../../corelib/database/databaseoracle/myoracle.h"
#include "../../corelib/database/databaseoracle/databaseoracle.h"

using namespace library;
using namespace corelib::database;
using namespace app;

extern std::map<std::string, ST_SYS_SETTING_INFO> g_cjmap;
extern std::map<std::string, ST_CAR_BRAND_INFO> g_cbmap;
extern std::map<std::string, ST_DEVICE_INFO> g_devinfomap;

////////////////////////////////////////////////////////////////////////////////
static void getdataitem(std::string &dst, char *src, size_t len)
{
	char str[MAX_BUFFER_SIZE_01k] = {0};
	size_t l = (len > sizeof(str) - 1) ? (sizeof(str) - 1) : len;

	dst.clear();
	if (src == NULL || len == 0)
	{
		dst += "";
		return;
	}
	::memcpy(str, src, l);
	dst += str;
}

////////////////////////////////////////////////////////////////////////////////
SqlCarRecord::SqlCarRecord(std::list<ST_CAR_RECORD_INFO> &dl) : m_dl(dl), jgsj(NULL), date(NULL), predate(NULL)
{
#if(defined(_ZH_PLATFORM_))
	specialblocks.insert("440400140000000040");
	specialblocks.insert("440400140000000045");
	specialblocks.insert("440400140000000049");
	specialblocks.insert("440400140000000036");
	specialblocks.insert("440400140000000038");
	specialblocks.insert("440400140000000039");
	specialblocks.insert("440400140000000030");
	specialblocks.insert("440400140000000028");
#endif
	
	initBindingVars();
}

SqlCarRecord::~SqlCarRecord()
{
	m_dl.clear();
	
	if (jgsj != NULL) OCI_DateFree(jgsj);
	jgsj = NULL;
	if (date == NULL) OCI_DateFree(date);
	date = NULL;
	if (predate == NULL) OCI_DateFree(predate);
	predate = NULL;
}

void SqlCarRecord::initBindingVars()
{
	xxbh.resize(100);
	kkbh.resize(50);
	cdbh.resize(50);
	hphm.resize(15);
	hpys.resize(1);
	cwhphm.resize(10);
	cwhpys.resize(1);
	hpyz.resize(1);
	sbcd.resize(20);
	qydm.resize(10);
	clbs.resize(10);
	tx1.resize(200);
	tx2.resize(200);
	tx3.resize(200);
	tx4.resize(200);
	tx5.resize(200);
	xszt.resize(4);
	clpp.resize(200);
	clwx.resize(3);
	csys.resize(5);
	cllx.resize(4);
	hpzl.resize(2);
	sbbh.resize(50);
	cj.resize(50);
	kkmc.resize(80);
	dwmc.resize(20);
	sbfx.resize(20);
	sbmc.resize(80);

	if (jgsj == NULL) jgsj = OCI_DateCreate(NULL);
	if (date == NULL) date = OCI_DateCreate(NULL);
	if (predate == NULL) predate = OCI_DateCreate(NULL);
}

void SqlCarRecord::Execute(Database *db)
{
	OCI_Statement* newRecordStmt, *headRecordStmt, *backRecordStmt, *fullRecordStmt;
	OCI_Statement* specialNewRecordStmt, *specialHeadRecordStmt, *specialBackRecordStmt;
	
	if (db == NULL) return ;

	uint64 tb = System::currentTimeMillis();

	DatatbaseOracle *mydb = static_cast<DatatbaseOracle*>(db);
	MyOracle* orcl = mydb->getDatabaseHandle();
	
	newRecordStmt = orcl->prepare(formatNewCarRecord());
	addBindingNewRecord(newRecordStmt);
	headRecordStmt = orcl->prepare(formatHeadCarRecord());
	addBindingHeadRecord(headRecordStmt);
	backRecordStmt = orcl->prepare(formatBackCarRecord());
	addBindingBackRecord(backRecordStmt);
	fullRecordStmt = orcl->prepare(formatFullCarRecord());
	addBindingFullRecord(fullRecordStmt);

	if (specialblocks.size() > 0) {
		specialNewRecordStmt = orcl->prepare(formatSpecialBlockNewCarRecord());
		addBindingSpecialBlockNewRecord(specialNewRecordStmt);
		specialHeadRecordStmt = orcl->prepare(formatSpecialBlockHeadCarRecord());
		addBindingSpecialBlockHeadRecord(specialHeadRecordStmt);
		specialBackRecordStmt = orcl->prepare(formatSpecialBlockBackCarRecord());
		addBindingSpecialBlockBackRecord(specialBackRecordStmt);
	}

	while (m_dl.size() > 0) {

		ST_CAR_RECORD_INFO carInfo = m_dl.front();
		m_dl.pop_front();

		update(carInfo.record);
		bool bRet = false;
		if (carInfo.action != INSERT) {
			if (strlen(tx1.c_str()) > 0 || strlen(tx2.c_str()) > 0) {
				bRet = orcl->execute(headRecordStmt);
				if (!bRet) Log(LOG_WARN, "update head, error pos: %d", OCI_GetSqlErrorPos(headRecordStmt));
				else Log(LOG_DEBUG, "update head %s success.", xxbh.c_str());
			} else if (strlen(tx3.c_str()) > 0 || strlen(tx4.c_str()) > 0) {
				bRet = orcl->execute(backRecordStmt);
				if (!bRet) Log(LOG_WARN, "update back, error pos: %d", OCI_GetSqlErrorPos(backRecordStmt));
				else Log(LOG_DEBUG, "update back %s success.", xxbh.c_str());
			} else if (strlen(tx5.c_str()) > 0) {
				bRet = orcl->execute(fullRecordStmt);
				if (!bRet) Log(LOG_WARN, "update full, error pos: %d", OCI_GetSqlErrorPos(fullRecordStmt));
				else Log(LOG_DEBUG, "update full %s success.", xxbh.c_str());
			} else {
				;
			}
		} else {
			bRet = orcl->execute(newRecordStmt);
			if (!bRet) Log(LOG_WARN, "insert new, error pos: %d", OCI_GetSqlErrorPos(newRecordStmt));
			else Log(LOG_DEBUG, "insert recd %s success.", xxbh.c_str());
		}

		if (specialblocks.size() > 0) {
			std::set<std::string>::iterator it = specialblocks.find(kkbh);
			if (it != specialblocks.end()) {
				if (carInfo.action != INSERT) {
					if (strlen(tx1.c_str()) > 0 || strlen(tx2.c_str()) > 0) {
						bRet = orcl->execute(specialBackRecordStmt);
						if (!bRet) Log(LOG_WARN, "special block update back, error pos: %d", OCI_GetSqlErrorPos(specialHeadRecordStmt));
					} else if (strlen(tx3.c_str()) > 0 || strlen(tx4.c_str()) > 0) {
						bRet = orcl->execute(specialHeadRecordStmt);
						if (!bRet) Log(LOG_WARN, "special block update head, error pos: %d", OCI_GetSqlErrorPos(specialBackRecordStmt));
					} else {
						;
					}
				} else {
					bRet = orcl->execute(specialNewRecordStmt);
					if (!bRet) Log(LOG_WARN, "special block insert recd, error pos: %d", OCI_GetSqlErrorPos(specialNewRecordStmt));
				}
			}
		}

		if (!bRet) {
			std::string errorRecord = toText(carInfo.record);
			Log(LOG_ERROR, "%s", errorRecord.c_str());
		}

		delete carInfo.record;
	}
	
//	bool bCommited = orcl->commit();

	OCI_StatementFree(newRecordStmt);
	OCI_StatementFree(headRecordStmt);
	OCI_StatementFree(backRecordStmt);
	OCI_StatementFree(fullRecordStmt);
	if (specialblocks.size() > 0) {
		OCI_StatementFree(specialNewRecordStmt);
		OCI_StatementFree(specialHeadRecordStmt);
		OCI_StatementFree(specialBackRecordStmt);
	}

	Log(LOG_DEBUG, "SqlCarRecord::Execute - execute car record cost[%llu] millsecs", System::currentTimeMillis() - tb);
}

std::string SqlCarRecord::formatNewCarRecord()
{
	std::string sql;
#if(defined(_ZH_PLATFORM_))
	sql = "INSERT INTO car_tab_temp(XXBH, KKBH, TPBH, JGSJ, CDBH, HPHM, HPYS, CWHPHM,CWHPYS, HPYZ, TXSL, TX1, TX2, TX3, TX4, CLSD, XSZT,CLPP, CLWX, CSYS, CLLX, HPZL, YLXXLX, YLXX, CLBJ,BYZD1, SBBH, UPLOADSTATUS, CLCD, CJ, KKMC, DWBH,DWMC, SBFX, SBCD, SBMC, WFBS, WFXWBH, GCSPLX, TX5,TX6, TX7, TX8, CLXS, SSDQ, CREATEDATE, QYDM, CLBS, KEYDATE, PREDATE, SUBDATE, FULLDATE) VALUES ";
	sql += "(:xxbh,:kkbh, NULL, :jgsj, :cdbh, :hphm, :hpys, :cwhphm, :cwhpys, :hpyz, :txsl, :tx1, :tx2, :tx3, :tx4, :clsd,:xszt,:clpp, :clwx, :csys, :cllx, :hpzl, NULL, NULL, NULL, NULL, :sbbh, NULL, :clcd, :cj, :kkmc, NULL, :dwmc, :sbfx, :sbcd, :sbmc, NULL, NULL, NULL, :tx5, NULL, NULL, NULL, :clxs, NULL, sysdate, :qydm, :clbs, :keydate, :predate, NULL, NULL)";
#else
	sql = "INSERT INTO car_tab_temp(XXBH, KKBH, TPBH, JGSJ, CDBH, HPHM, HPYS, CWHPHM,CWHPYS, HPYZ, TXSL, TX1, TX2, TX3, TX4, CLSD, XSZT,CLPP, CLWX, CSYS, CLLX, HPZL, YLXXLX, YLXX, CLBJ,BYZD1, SBBH, UPLOADSTATUS, CLCD, CJ, KKMC, DWBH,DWMC, SBFX, SBCD, SBMC, WFBS, WFXWBH, GCSPLX, TX5,TX6, TX7, TX8, CLXS, SSDQ, CREATEDATE, QYDM, CLBS, KEYDATE, PREDATE, SUBDATE, FULLDATE, CDLX) VALUES ";
	sql += "(:xxbh,:kkbh, NULL, :jgsj, :cdbh, :hphm, :hpys, :cwhphm, :cwhpys, :hpyz, :txsl, :tx1, :tx2, :tx3, :tx4, :clsd,:xszt,:clpp, :clwx, :csys, :cllx, :hpzl, NULL, NULL, NULL, NULL, :sbbh, NULL, :clcd, :cj, :kkmc, NULL, :dwmc, :sbfx, :sbcd, :sbmc, NULL, NULL, NULL, :tx5, NULL, NULL, NULL, :clxs, NULL, sysdate, :qydm, :clbs, :keydate, :predate, NULL, NULL, NULL)";
#endif

	return sql;
}

void SqlCarRecord::addBindingNewRecord(OCI_Statement *stmt)
{
	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":kkbh", (dtext*)(kkbh.c_str()), kkbh.size());
	OCI_BindDate(stmt, ":jgsj", jgsj);
	OCI_BindString(stmt, ":cdbh", (dtext*)(cdbh.c_str()), cdbh.size());
	OCI_BindString(stmt, ":hphm", (dtext*)(hphm.c_str()), hphm.size());
	OCI_BindString(stmt, ":hpys", (dtext*)(hpys.c_str()), hpys.size());
	OCI_BindString(stmt, ":cwhphm", (dtext*)(cwhphm.c_str()), 10);
	OCI_BindString(stmt, ":cwhpys", (dtext*)(cwhpys.c_str()), cwhpys.size());
	OCI_BindString(stmt, ":hpyz", (dtext*)(hpyz.c_str()), hpyz.size());
	OCI_BindInt(stmt, ":txsl", &txsl);
	OCI_BindString(stmt, ":tx1", (dtext*)(tx1.c_str()), tx1.size());
	OCI_BindString(stmt, ":tx2", (dtext*)(tx2.c_str()), tx2.size());
	OCI_BindString(stmt, ":tx3", (dtext*)(tx3.c_str()), tx3.size());
	OCI_BindString(stmt, ":tx4", (dtext*)(tx4.c_str()), tx4.size());
	OCI_BindFloat(stmt, ":clsd", &clsd);
	OCI_BindString(stmt, ":xszt", (dtext*)(xszt.c_str()), xszt.size());
	OCI_BindString(stmt, ":clpp", (dtext*)(clpp.c_str()), clpp.size());
	OCI_BindString(stmt, ":clwx", (dtext*)(clwx.c_str()), clwx.size());
	OCI_BindString(stmt, ":csys", (dtext*)(csys.c_str()), csys.size());
	OCI_BindString(stmt, ":cllx", (dtext*)(cllx.c_str()), cllx.size());
	OCI_BindString(stmt, ":hpzl", (dtext*)(hpzl.c_str()), hpzl.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindFloat(stmt, ":clcd", &clcd);
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":kkmc", (dtext*)(kkmc.c_str()), kkmc.size());
	OCI_BindString(stmt, ":dwmc", (dtext*)(dwmc.c_str()), dwmc.size());
	OCI_BindString(stmt, ":sbfx", (dtext*)(sbfx.c_str()), sbfx.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
	OCI_BindString(stmt, ":tx5", (dtext*)(tx5.c_str()), tx5.size());
	OCI_BindFloat(stmt, ":clxs", &clxs);
	OCI_BindString(stmt, ":qydm", (dtext*)(qydm.c_str()), qydm.size());
	OCI_BindString(stmt, ":clbs", (dtext*)(clbs.c_str()), clbs.size());
	OCI_BindDate(stmt, ":keydate", date);
	OCI_BindDate(stmt, ":predate", predate);
}

std::string SqlCarRecord::formatHeadCarRecord()
{
	std::string sql;

	sql = "update car_tab_temp set HPHM = :hphm, HPYS = :hpys, TX1 = :tx1, TX2 = :tx2, SBBH = :sbbh, CJ = :cj, SBCD = :sbcd, SBMC = :sbmc";
	sql += " where XXBH = :xxbh";

	return sql;
}

void SqlCarRecord::addBindingHeadRecord(OCI_Statement *stmt)
{
	if (stmt == NULL) return;
	
	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":hphm", (dtext*)(hphm.c_str()), xxbh.size());
	OCI_BindString(stmt, ":hpys", (dtext*)(hpys.c_str()), hpys.size());
	OCI_BindString(stmt, ":tx1", (dtext*)(tx1.c_str()), tx1.size());
	OCI_BindString(stmt, ":tx2", (dtext*)(tx2.c_str()), tx2.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
}

std::string SqlCarRecord::formatBackCarRecord()
{
	std::string sql;

	sql = "update car_tab_temp set CWHPHM = :cwhphm, CWHPYS = :cwhpys, TX3 = :tx3, TX4 = :tx4 , SBBH = :sbbh, CJ = :cj, SBCD = :sbcd, SBMC = :sbmc ";
	sql += " where XXBH = :xxbh";

	return sql;
}

void SqlCarRecord::addBindingBackRecord(OCI_Statement *stmt)
{
	if (stmt == NULL) return;

	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":cwhphm", (dtext*)(cwhphm.c_str()), 10);
	OCI_BindString(stmt, ":cwhpys", (dtext*)(cwhpys.c_str()), cwhpys.size());
	OCI_BindString(stmt, ":tx3", (dtext*)(tx3.c_str()), tx3.size());
	OCI_BindString(stmt, ":tx4", (dtext*)(tx4.c_str()), tx4.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
}

std::string SqlCarRecord::formatFullCarRecord()
{
	std::string sql;

	sql = "update car_tab_temp set TX5 = :tx5 ";
	sql += " where XXBH = :xxbh";

	return sql;
}

void SqlCarRecord::addBindingFullRecord(OCI_Statement *stmt)
{
	if (stmt == NULL) return;
	
	OCI_BindString(stmt, ":xxbh", (char*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":tx5", (char*)(tx5.c_str()), tx5.size());
}

std::string SqlCarRecord::formatSpecialBlockNewCarRecord()
{
	std::string sql;

#if(defined(_ZH_PLATFORM_))
	sql = "INSERT INTO car_tab_temp3(XXBH, KKBH, TPBH, JGSJ, CDBH, HPHM, HPYS, CWHPHM,CWHPYS, HPYZ, TXSL, TX1, TX2, TX3, TX4, CLSD, XSZT,CLPP, CLWX, CSYS, CLLX, HPZL, YLXXLX, YLXX, CLBJ,BYZD1, SBBH, UPLOADSTATUS, CLCD, CJ, KKMC, DWBH,DWMC, SBFX, SBCD, SBMC, WFBS, WFXWBH, GCSPLX, TX5,TX6, TX7, TX8, CLXS, SSDQ, CREATEDATE, QYDM, CLBS, KEYDATE, PREDATE, SUBDATE, FULLDATE) VALUES ";
	sql += "(:xxbh,:kkbh, NULL, :jgsj, :cdbh, :hphm, :hpys, :cwhphm, :cwhpys, :hpyz, :txsl, :tx3, :tx4, :tx1, :tx2, :clsd,:xszt,:clpp, :clwx, :csys, :cllx, :hpzl, NULL, NULL, NULL, NULL, :sbbh, NULL, :clcd, :cj, :kkmc, NULL, :dwmc, :sbfx, :sbcd, :sbmc, NULL, NULL, NULL, :tx5, NULL, NULL, NULL, :clxs, NULL, sysdate, :qydm, :clbs, :keydate, :predate, NULL, NULL)";
#else 
	sql = "INSERT INTO car_tab_temp3(XXBH, KKBH, TPBH, JGSJ, CDBH, HPHM, HPYS, CWHPHM,CWHPYS, HPYZ, TXSL, TX1, TX2, TX3, TX4, CLSD, XSZT,CLPP, CLWX, CSYS, CLLX, HPZL, YLXXLX, YLXX, CLBJ,BYZD1, SBBH, UPLOADSTATUS, CLCD, CJ, KKMC, DWBH,DWMC, SBFX, SBCD, SBMC, WFBS, WFXWBH, GCSPLX, TX5,TX6, TX7, TX8, CLXS, SSDQ, CREATEDATE, QYDM, CLBS, KEYDATE, PREDATE, SUBDATE, FULLDATE, CDLX) VALUES ";
	sql += "(:xxbh,:kkbh, NULL, :jgsj, :cdbh, :hphm, :hpys, :cwhphm, :cwhpys, :hpyz, :txsl, :tx1, :tx2, :tx3, :tx4, :clsd,:xszt,:clpp, :clwx, :csys, :cllx, :hpzl, NULL, NULL, NULL, NULL, :sbbh, NULL, :clcd, :cj, :kkmc, NULL, :dwmc, :sbfx, :sbcd, :sbmc, NULL, NULL, NULL, :tx5, NULL, NULL, NULL, :clxs, NULL, sysdate, :qydm, :clbs, :keydate, :predate, NULL, NULL, NULL)";
#endif
	return sql;
}

void SqlCarRecord::addBindingSpecialBlockNewRecord(OCI_Statement *stmt)
{
	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":kkbh", (dtext*)(kkbh.c_str()), kkbh.size());
	OCI_BindDate(stmt, ":jgsj", jgsj);
	OCI_BindString(stmt, ":cdbh", (dtext*)(cdbh.c_str()), cdbh.size());
	OCI_BindString(stmt, ":hphm", (dtext*)(hphm.c_str()), hphm.size());
	OCI_BindString(stmt, ":hpys", (dtext*)(hpys.c_str()), hpys.size());
	OCI_BindString(stmt, ":cwhphm", (dtext*)(cwhphm.c_str()), cwhphm.size());
	OCI_BindString(stmt, ":cwhpys", (dtext*)(cwhpys.c_str()), cwhpys.size());
	OCI_BindString(stmt, ":hpyz", (dtext*)(hpyz.c_str()), hpyz.size());
	OCI_BindInt(stmt, ":txsl", &txsl);
	OCI_BindString(stmt, ":tx1", (dtext*)(tx1.c_str()), tx1.size());
	OCI_BindString(stmt, ":tx2", (dtext*)(tx2.c_str()), tx2.size());
	OCI_BindString(stmt, ":tx3", (dtext*)(tx3.c_str()), tx3.size());
	OCI_BindString(stmt, ":tx4", (dtext*)(tx4.c_str()), tx4.size());
	OCI_BindFloat(stmt, ":clsd", &clsd);
	OCI_BindString(stmt, ":xszt", (dtext*)(xszt.c_str()), xszt.size());
	OCI_BindString(stmt, ":clpp", (dtext*)(clpp.c_str()), clpp.size());
	OCI_BindString(stmt, ":clwx", (dtext*)(clwx.c_str()), clwx.size());
	OCI_BindString(stmt, ":csys", (dtext*)(csys.c_str()), csys.size());
	OCI_BindString(stmt, ":cllx", (dtext*)(cllx.c_str()), cllx.size());
	OCI_BindString(stmt, ":hpzl", (dtext*)(hpzl.c_str()), hpzl.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindFloat(stmt, ":clcd", &clcd);
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":kkmc", (dtext*)(kkmc.c_str()), kkmc.size());
	OCI_BindString(stmt, ":dwmc", (dtext*)(dwmc.c_str()), dwmc.size());
	OCI_BindString(stmt, ":sbfx", (dtext*)(sbfx.c_str()), sbfx.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
	OCI_BindString(stmt, ":tx5", (dtext*)(tx5.c_str()), tx5.size());
	OCI_BindFloat(stmt, ":clxs", &clxs);
	OCI_BindString(stmt, ":qydm", (dtext*)(qydm.c_str()), qydm.size());
	OCI_BindString(stmt, ":clbs", (dtext*)(clbs.c_str()), clbs.size());
	OCI_BindDate(stmt, ":keydate", date);
	OCI_BindDate(stmt, ":predate", predate);
}

std::string SqlCarRecord::formatSpecialBlockHeadCarRecord()
{
	std::string sql;

	sql = "update car_tab_temp3 set HPHM = :hphm, HPYS = :hpys, TX1 = :tx3, TX2 = :tx4, SBBH = :sbbh, CJ = :cj, SBCD = :sbcd, SBMC = :sbmc";
	sql += " where XXBH = :xxbh";

	return sql;
}

void SqlCarRecord::addBindingSpecialBlockHeadRecord(OCI_Statement *stmt)
{
	if (stmt == NULL) return;
	
	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":hphm", (dtext*)(cwhphm.c_str()), xxbh.size());
	OCI_BindString(stmt, ":hpys", (dtext*)(cwhpys.c_str()), hpys.size());
	OCI_BindString(stmt, ":tx3", (dtext*)(tx3.c_str()), tx3.size());
	OCI_BindString(stmt, ":tx4", (dtext*)(tx4.c_str()), tx4.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
}

std::string SqlCarRecord::formatSpecialBlockBackCarRecord()
{
	std::string sql;

	sql = "update car_tab_temp3 set CWHPHM = :cwhphm, CWHPYS = :cwhpys, TX3 = :tx1, TX4 = :tx2 , SBBH = :sbbh, CJ = :cj, SBCD = :sbcd, SBMC = :sbmc ";
	sql += " where XXBH = :xxbh";

	return sql;
}

void SqlCarRecord::addBindingSpecialBlockBackRecord(OCI_Statement *stmt)
{
	if (stmt == NULL) return;

	OCI_BindString(stmt, ":xxbh", (dtext*)(xxbh.c_str()), xxbh.size());
	OCI_BindString(stmt, ":cwhphm", (dtext*)(hphm.c_str()), 10);
	OCI_BindString(stmt, ":cwhpys", (dtext*)(hpys.c_str()), cwhpys.size());
	OCI_BindString(stmt, ":tx1", (dtext*)(tx1.c_str()), tx1.size());
	OCI_BindString(stmt, ":tx2", (dtext*)(tx2.c_str()), tx2.size());
	OCI_BindString(stmt, ":sbbh", (dtext*)(sbbh.c_str()), sbbh.size());
	OCI_BindString(stmt, ":cj", (dtext*)(cj.c_str()), cj.size());
	OCI_BindString(stmt, ":sbcd", (dtext*)(sbcd.c_str()), sbcd.size());
	OCI_BindString(stmt, ":sbmc", (dtext*)(sbmc.c_str()), sbmc.size());
}

void SqlCarRecord::update(ST_CAR_DESCRIPTION_INFO *record)
{
	char in[MAX_BUFFER_SIZE_256] = {0}, out[MAX_BUFFER_SIZE_256] = {0};
	std::map<std::string, ST_SYS_SETTING_INFO>::iterator iter;
	std::map<std::string, ST_CAR_BRAND_INFO>::iterator ite;
	std::map<std::string, ST_DEVICE_INFO>::iterator it;
	char buf[MAX_BUFFER_SIZE_256] = {0};
	ST_SYS_SETTING_INFO stsyscjinfo;
	ST_DEVICE_INFO devinfo; 
	std::string t;
	
	if (record == NULL) return;

	memset(&stsyscjinfo, 0, sizeof(stsyscjinfo));
	memset(&devinfo, 0, sizeof(devinfo));
	memset(in, 0, sizeof(in)); memset(out, 0, sizeof(out));
	
	if ((strlen(record->urlInfo.url[0]) > 0 || strlen(record->urlInfo.url[1]) > 0))
		memcpy(in, record->plateFNo, sizeof(record->plateFNo));
	else if ((strlen(record->urlInfo.url[2]) > 0 || strlen(record->urlInfo.url[3]) > 0))
		memcpy(in, record->plateTNo, sizeof(record->plateTNo));
	else
		memcpy(in, record->plateFNo, sizeof(record->plateFNo));

	//transfer to gb2312
	memset(buf, 0, sizeof(buf));
	int32 iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		memcpy(buf, in, 3);
	} else {
		memcpy(buf, out, 3);
	}
	iter = g_cjmap.find(buf);
	if (iter != g_cjmap.end()) stsyscjinfo = iter->second;

	memset(buf, 0, sizeof(buf));
	strncpy(buf, record->devSn, sizeof(record->devSn));
	it = g_devinfomap.find(buf);
	if (it != g_devinfomap.end()) devinfo = it->second;

	//xxbh
	xxbh.clear();
	getdataitem(xxbh, record->blockSn, sizeof(record->blockSn));
	getdataitem(t, devinfo.direction, sizeof(devinfo.direction));
	xxbh += t;
	getdataitem(t, record->laneSn, sizeof(record->laneSn));
	xxbh += t;
	getdataitem(t, record->vehiSn, sizeof(record->vehiSn));
	xxbh += t;
//	Log(LOG_DEBUG, "SqlCarRecord::update - xxbh: %s ", xxbh.c_str());

	//kkbh
	getdataitem(kkbh, record->blockSn, sizeof(record->blockSn));

	//jgsj
	char *pcPos = strrchr(record->ptime, '.');
	if (pcPos && pcPos - record->ptime > 0)
		getdataitem(t, record->ptime, (pcPos - record->ptime));
	else
		t = "2000-01-01 00:00:00";

//	OCI_DateFromText(jgsj, MT(t.c_str()), MT("YYYY-MM-DD HH24:MI:SS"));
	OCI_DateFromText(jgsj, t.c_str(), "YYYY-MM-DD HH24:MI:SS");

	getdataitem(cdbh, record->laneSn, sizeof(record->laneSn));

	///! transfer @param 'hphm' to gb2312
	memset(in, 0, sizeof(in)); memset(out, 0, sizeof(out));
	memcpy(in, record->plateFNo, sizeof(record->plateFNo));
	iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		getdataitem(hphm, record->plateFNo, sizeof(record->plateFNo));
	} else {
		getdataitem(hphm, out, strlen(out));
	}
	getdataitem(hpys, record->plateFCol, sizeof(record->plateFCol));

	///! transfer @param 'hphm' to gb2312
	memset(in, 0, sizeof(in)); memset(out, 0, sizeof(out));
	memcpy(in, record->plateTNo, sizeof(record->plateTNo));
	iRet = System::u2g(in, sizeof(in), out, sizeof(out));
	if (iRet < 0 || strlen(out) <= 0) {
		getdataitem(cwhphm, record->plateTNo, sizeof(record->plateTNo));
	} else {
		getdataitem(cwhphm, out, strlen(out));
	}
	getdataitem(cwhpys, record->plateTCol, sizeof(record->plateTCol));
	getdataitem(hpyz, record->ftEquel, sizeof(record->ftEquel));
	txsl = record->urlInfo.urlNum;
	getdataitem(tx1, record->urlInfo.url[0], strlen(record->urlInfo.url[0]));
	getdataitem(tx2, record->urlInfo.url[1], strlen(record->urlInfo.url[1]));
	getdataitem(tx3, record->urlInfo.url[2], strlen(record->urlInfo.url[2]));
	getdataitem(tx4, record->urlInfo.url[3], strlen(record->urlInfo.url[3]));
	clsd = record->fSpeed;
	getdataitem(xszt, record->runState, sizeof(record->runState));
	getdataitem(clpp, record->vehiBrand, sizeof(record->vehiBrand));
	getdataitem(clwx, record->vehiShape, sizeof(record->vehiShape));
	getdataitem(csys, record->vehiCol, sizeof(record->vehiCol));
	getdataitem(cllx, record->vehiType, sizeof(record->vehiType));
	getdataitem(hpzl, record->plateType, sizeof(record->plateType));
	getdataitem(sbbh, record->devSn, sizeof(record->devSn));
	clcd = record->vehiLen;
	getdataitem(cj, stsyscjinfo.dspVal, strlen(stsyscjinfo.dspVal));
	getdataitem(kkmc, devinfo.kkmc, strlen(devinfo.kkmc));
	getdataitem(dwmc, devinfo.dwmc, strlen(devinfo.dwmc));
	getdataitem(sbfx, devinfo.direction, strlen(devinfo.direction));
	getdataitem(sbcd, devinfo.devLen, strlen(devinfo.devLen));
	getdataitem(sbmc, devinfo.devName, strlen(devinfo.devName));
	getdataitem(tx5, record->urlInfo.url[4], strlen(record->urlInfo.url[4]));
	clxs = record->fSpeedLim;
	getdataitem(qydm, devinfo.qydm, strlen(devinfo.qydm));
	getdataitem(clbs, record->vehiCode, sizeof(record->vehiCode));

	/* use keydate with time created */
	struct tm *ptm = NULL;
	char tmstr[32] = {0};
	time_t tsec = ::time(NULL);

	ptm = ::localtime(&tsec);
	sprintf(tmstr, "%04d-%02d-%02d %02d:%02d:%02d", (1900 + ptm->tm_year), 
		    (1 + ptm->tm_mon), ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	OCI_DateFromText(date, tmstr, "YYYY-MM-DD HH24:MI:SS");
	OCI_DateFromText(predate, record->arrivetime, "YYYY-MM-DD HH24:MI:SS");
}

std::string SqlCarRecord::toText(ST_CAR_DESCRIPTION_INFO *record)
{
	if (record == NULL) return std::string("");

	std::string text, t;

	text += xxbh; text += ",";
	text += kkbh; text += ",";
	
	getdataitem(t, record->devSn, sizeof(record->devSn));
	text += t; text += ",";
	
	getdataitem(t, record->ptime, sizeof(record->ptime));
	text += t; text += ",";
	
	text += cdbh; text += ",";
	text += hphm; text += ",";
	text += hpys; text += ",";
	text += cwhphm; text += ",";
	text += cwhpys; text += ",";
	text += hpyz; text += ",";
	text += tx1; text += ",";
	text += tx2; text += ",";
	text += tx3; text += ",";
	text += tx4; text += ",";
	text += xszt;

	return text;
}
