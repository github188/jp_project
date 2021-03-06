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
#ifndef __APP_PROTOCOL_CARDESCRIPTIONINFO_H__
#define __APP_PROTOCOL_CARDESCRIPTIONINFO_H__

#include <string>
#include <vector>

#include "../common/eprotocol.h"
#include "../../common/basetype.h"
#include "../../corelib/protocol/datastructure.h"

namespace app {

using corelib::protocol::DataStructure;
class CarDescriptionInfo : public DataStructure
{
	public:

		static const std::string ID_CAR_INFO;
		
	public:

		CarDescriptionInfo();

		~CarDescriptionInfo();

	public:

		virtual std::string getDataStructureType() const;

		virtual void copyDataStructure(const DataStructure *src);

		virtual bool equals(const DataStructure *value) const;

	public:

		virtual void setXxbh(const char *str, int32 len) {this->xxbh.append(trimSuffixSpace((char*)str, len));};
		virtual void setXxbh(const std::string &str);
		virtual std::string getXxbh()const;

		virtual void setKkbh(const char *str, int32 len) {this->kkbh.append(trimSuffixSpace((char*)str, len));};
		virtual void setKkbh(const std::string &str);
		virtual std::string getKkbh()const;

		virtual void setSbbh(const char *str, int32 len) {this->sbbh.append(trimSuffixSpace((char*)str, len));};
		virtual void setSbbh(const std::string &str);
		virtual std::string getSbbh()const;

		virtual void setJgsj(const char *str, int32 len) {this->jgsj.append(trimSuffixSpace((char*)str, len));};
		virtual void setJgsj(const std::string &str);
		virtual std::string getJgsj()const;

		virtual void setCdbh(const char *str, int32 len) {this->cb.append(trimSuffixSpace((char*)str, len));};
		virtual void setCdbh(const std::string &str);
		virtual std::string getCdbh()const;

		virtual void setHphm(const char *str, int32 len) {this->hphm.append(trimSuffixSpace((char*)str, len));};
		virtual void setHphm(const std::string &str);
		virtual std::string getHphm()const;

		virtual void setHpys(const char *str, int32 len) {this->hpys.append(trimSuffixSpace((char*)str, len));};
		virtual void setHpys(const std::string &str);
		virtual std::string getHpys()const;

		virtual void setCwhphm(const char *str, int32 len) {this->cwhphm.append(trimSuffixSpace((char*)str, len));};
		virtual void setCwhphm(const std::string &str);
		virtual std::string getCwhphm()const;

		virtual void setCwhpys(const char *str, int32 len) {this->cwhpys.append(trimSuffixSpace((char*)str, len));};
		virtual void setCwhpys(const std::string &str);
		virtual std::string getCwhpys()const;

		virtual void setHpyz(const char *str, int32 len) {this->hpyz.append(trimSuffixSpace((char*)str, len));};
		virtual void setHpyz(const std::string &str);
		virtual std::string getHpyz()const;

		virtual void setClsd(float speed);
		virtual float getClsd()const;

		virtual void setClxs(float limitSpeed);
		virtual float getClxs()const;

		virtual void setClcd(int32 clcd);
		virtual int32 getClcd()const;

		virtual void setXszt(const char *str, int32 len) {this->xszt.append(trimSuffixSpace((char*)str, len));};
		virtual void setXszt(const std::string &str);
		virtual std::string getXszt()const;

		virtual void setClpp(const char *str, int32 len) {this->clpp.append(trimSuffixSpace((char*)str, len));};
		virtual void setClpp(const std::string &str);
		virtual std::string getClpp()const;

		virtual void setClwx(const char *str, int32 len) {this->clwx.append(trimSuffixSpace((char*)str, len));};
		virtual void setClwx(const std::string &str);
		virtual std::string getClwx()const;

		virtual void setCsys(const char *str, int32 len) {this->csys.append(trimSuffixSpace((char*)str, len));};
		virtual void setCsys(const std::string &str);
		virtual std::string getCsys()const;

		virtual void setCb(const char *str, int32 len) {this->cb.append(trimSuffixSpace((char*)str, len));};
		virtual void setCb(const std::string &str);
		virtual std::string getCb()const;

		virtual void setCllx(const char *str, int32 len) {this->cllx.append(trimSuffixSpace((char*)str, len));};
		virtual void setCllx(const std::string &str);
		virtual std::string getCllx()const;

		virtual void setHpzl(const char *str, int32 len) {this->hpzl.append(trimSuffixSpace((char*)str, len));};
		virtual void setHpzl(const std::string &str);
		virtual std::string getHpzl()const;

		virtual void setSsqy(const char *str, int32 len) {this->ssqy.append(trimSuffixSpace((char*)str, len));};
		virtual void setSsqy(const std::string &str);
		virtual std::string getSsqy()const;

		virtual void setTxsl(int32 num);
		virtual int32 getTxsl()const;

		virtual void setPicture(ST_PICTURE_INFO *picture);
		virtual void getPicture(int32 index, uint8 *data, uint32 &len);
		virtual int32 getPictureNumber() const;

		void getCarInfo(ST_CAR_DESCRIPTION_INFO &carInfo);
		void getPictures(ST_PICTURES_INFO &pictures);

	private:

		std::string trimSuffixSpace(char *data, int32 size);
		std::string trimSuffixSpace(const std::string &data);

	private:
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

		std::vector<ST_PICTURE_INFO *> pictures;
};

}

#endif /* end of file */