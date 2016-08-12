/***************************************************************************
 *   Copyright (C) 2015 by root   				   						   *
 *   ysgen0217@163.com   							   				  	   *
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
#ifndef __APP_PROTOCOL_CARBYPASSDESCINFO_H__
#define __APP_PROTOCOL_CARBYPASSDESCINFO_H__

#include <string>
#include <vector>

#include "../common/eprotocol.h"
#include "../../common/basetype.h"
#include "../../corelib/protocol/datastructure.h"

namespace app {

using corelib::protocol::DataStructure;
class CarBypassDescInfo : public DataStructure
{
	public:

		static const std::string ID_CARBYPASS_INFO;

	public:

		CarBypassDescInfo();
		~CarBypassDescInfo();

	public:

		virtual std::string getDataStructureType() const;
		virtual void copyDataStructure(const DataStructure *src);
		virtual bool equals(const DataStructure *value) const;

	public:

		virtual void setAll(ST_CAR_DESCRIPTION_INFO *carInfo);
		virtual ST_CAR_DESCRIPTION_INFO getAll()const;

		virtual void setXxbh(const char *str, int32 len) {this->xxbh.append(str, len);};
		virtual void setXxbh(const std::string &str) {this->xxbh.append(str);};
		virtual std::string getXxbh()const {return this->xxbh;};

		virtual void setKkbh(const char *str, int32 len) {this->kkbh.append(str, len);};
		virtual void setKkbh(const std::string &str) {this->kkbh.append(str);};
		virtual std::string getKkbh()const {return this->kkbh;};

		virtual void setSbbh(const char *str, int32 len) {this->sbbh.append(str, len);};
		virtual void setSbbh(const std::string &str) {this->sbbh.append(str);};
		virtual std::string getSbbh()const {return this->sbbh;};

		virtual void setJgsj(const char *str, int32 len) {this->jgsj.append(str, len);};
		virtual void setJgsj(const std::string &str) {this->jgsj.append(str);};
		virtual std::string getJgsj()const {return this->jgsj;};

		virtual void setCdbh(const char *str, int32 len) {this->cb.append(str, len);};
		virtual void setCdbh(const std::string &str) {this->cb.append(str);};
		virtual std::string getCdbh()const {return this->cb;};

		virtual void setHphm(const char *str, int32 len) {this->hphm.append(str, len);};
		virtual void setHphm(const std::string &str) {this->hphm.append(str);};
		virtual std::string getHphm()const {return this->hphm;};

		virtual void setHpys(const char *str, int32 len) {this->hpys.append(str, len);};
		virtual void setHpys(const std::string &str) {this->hpys.append(str);};
		virtual std::string getHpys()const {return this->hpys;};

		virtual void setCwhphm(const char *str, int32 len) {this->cwhphm.append(str, len);};
		virtual void setCwhphm(const std::string &str) {this->cwhphm.append(str);};
		virtual std::string getCwhphm()const {return this->cwhphm;};

		virtual void setCwhpys(const char *str, int32 len) {this->cwhpys.append(str, len);};
		virtual void setCwhpys(const std::string &str) {this->cwhpys.append(str);};
		virtual std::string getCwhpys()const {return this->cwhpys;};

		virtual void setHpyz(const char *str, int32 len) {this->hpyz.append(str, len);};
		virtual void setHpyz(const std::string &str) {this->hpyz.append(str);};
		virtual std::string getHpyz()const {return this->hpyz;};

		virtual void setClsd(float speed) {this->clsd = speed;};
		virtual float getClsd()const {return this->clsd;};

		virtual void setClxs(float limitSpeed) {this->clxs = limitSpeed;};
		virtual float getClxs()const {return this->clxs;};

		virtual void setClcd(int32 clcd) {this->cscd = clcd;};
		virtual int32 getClcd()const {return this->cscd;};

		virtual void setXszt(const char *str, int32 len) {this->xszt.append(str, len);};
		virtual void setXszt(const std::string &str) {this->xszt.append(str);};
		virtual std::string getXszt()const {return this->xszt;};

		virtual void setClpp(const char *str, int32 len) {this->clpp.append(str, len);};
		virtual void setClpp(const std::string &str) {this->clpp.append(str);};
		virtual std::string getClpp()const {return this->clpp;};

		virtual void setClwx(const char *str, int32 len) {this->clwx.append(str, len);};
		virtual void setClwx(const std::string &str) {this->clwx.append(str);};
		virtual std::string getClwx()const {return this->clwx;};

		virtual void setCsys(const char *str, int32 len) {this->csys.append(str, len);};
		virtual void setCsys(const std::string &str) {this->csys.append(str);};
		virtual std::string getCsys()const {return this->csys;};

		virtual void setCb(const char *str, int32 len) {this->cb.append(str, len);};
		virtual void setCb(const std::string &str) {this->cb.append(str);};
		virtual std::string getCb()const {return this->cb;};

		virtual void setCllx(const char *str, int32 len) {this->cllx.append(str, len);};
		virtual void setCllx(const std::string &str) {this->cllx.append(str);};
		virtual std::string getCllx()const {return this->cllx;};

		virtual void setHpzl(const char *str, int32 len) {this->hpzl.append(str, len);};
		virtual void setHpzl(const std::string &str) {this->hpzl.append(str);};
		virtual std::string getHpzl()const {return this->hpzl;};

		virtual void setSsqy(const char *str, int32 len) {this->ssqy.append(str, len);};
		virtual void setSsqy(const std::string &str) {this->ssqy.append(str);};
		virtual std::string getSsqy()const {return this->ssqy;};

		virtual void setTxsl(int32 num) {this->txsl = num;};
		virtual int32 getTxsl()const {return this->txsl;};

		virtual void setUrls(ST_PICTURE_URL_INFO u) {this->urls = u;};
		virtual ST_PICTURE_URL_INFO getUrls()const {return this->urls;};

	public:

		std::string xxbh;
		std::string kkbh;
		std::string sbbh;
		std::string jgsj;
		std::string cdbh;
		std::string hphm;
		std::string hpys;
		std::string cwhphm;
		std::string cwhpys;
		std::string hpyz;
		float		clsd;
		float		clxs;
		int32		cscd;
		std::string xszt;
		std::string clpp;
		std::string clwx;
		std::string csys;
		std::string cb;
		std::string cllx;
		std::string hpzl;
		std::string ssqy;
		int32		txsl;
		ST_PICTURE_URL_INFO urls;
};

}

#endif /* end of file */