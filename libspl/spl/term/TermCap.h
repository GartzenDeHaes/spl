/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _termcap_h
#define _termcap_h

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef _WINDOWS
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/threading/Mutex.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>

#include <spl/term/acs.h>
#include <spl/io/File.h>
#include <spl/collection/Hashtable.h>
#include <spl/collection/Vector.h>

namespace spl
{

enum TemCapCode
{
 TCAP_ae = 0, TCAP_al = 1,
 TCAP_AL = 2, TCAP_am = 3,
 TCAP_as = 4, TCAP_bc = 5,
 TCAP_bl = 6, TCAP_bs = 7,
 TCAP_bt = 8, TCAP_bw = 9,
 TCAP_CC = 10, TCAP_cd = 11,
 TCAP_ce = 12, TCAP_ch = 13,
 TCAP_cl = 14, TCAP_cm = 15,
 TCAP_CM = 16, TCAP_co = 17,
 TCAP_cr = 18, TCAP_cs = 19,
 TCAP_cS = 20, TCAP_ct = 21,
 TCAP_cv = 22, TCAP_da = 23,
 TCAP_db = 24, TCAP_dB = 25,
 TCAP_dc = 26, TCAP_dC = 27,
 TCAP_DC = 28, TCAP_dF = 29,
 TCAP_dl = 30, TCAP_DL = 31,
 TCAP_dm = 32, TCAP_dN = 33,
 TCAP_do = 34, TCAP_DO = 35,
 TCAP_ds = 36, TCAP_dT = 37,
 TCAP_ec = 38, TCAP_ed = 39,
 TCAP_ei = 40, TCAP_eo = 41,
 TCAP_es = 42, TCAP_ff = 43,
 TCAP_fs = 44, TCAP_gn = 45,
 TCAP_hc = 46, TCAP_hd = 47,
 TCAP_ho = 48, TCAP_hs = 49,
 TCAP_hu = 50, TCAP_hz = 51,
 TCAP_i1 = 52, TCAP_i3 = 53,
 TCAP_ic = 54, TCAP_IC = 55,
 TCAP_if = 56, TCAP_im = 57,
 TCAP_in = 58, TCAP_ip = 59,
 TCAP_is = 60, TCAP_it = 61,
 TCAP_k0 = 62, TCAP_k1 = 63,
 TCAP_k2 = 64, TCAP_k3 = 65,
 TCAP_k4 = 66, TCAP_k5 = 67,
 TCAP_k6 = 68, TCAP_k7 = 69,
 TCAP_k8 = 70, TCAP_k9 = 71,
 TCAP_K1 = 72, TCAP_K2 = 73,
 TCAP_K3 = 74, TCAP_K4 = 75,
 TCAP_K5 = 76, TCAP_ka = 77,
 TCAP_kA = 78, TCAP_kb = 79,
 TCAP_kC = 80, TCAP_kd = 81,
 TCAP_kD = 82, TCAP_ke = 83,
 TCAP_kE = 84, TCAP_kF = 85,
 TCAP_kh = 86, TCAP_kH = 87,
 TCAP_kI = 88, TCAP_kl = 89,
 TCAP_kL = 90, TCAP_km = 91,
 TCAP_kM = 92, TCAP_kn = 93,
 TCAP_kN = 94, TCAP_ko = 95,
 TCAP_kP = 96, TCAP_kr = 97,
 TCAP_kR = 98, TCAP_ks = 99,
 TCAP_kS = 100, TCAP_kt = 101,
 TCAP_kT = 102, TCAP_ku = 103,
 TCAP_l0 = 104, TCAP_l1 = 105,
 TCAP_l2 = 106, TCAP_l3 = 107,
 TCAP_l4 = 108, TCAP_l5 = 109,
 TCAP_l6 = 110, TCAP_l7 = 111,
 TCAP_l8 = 112, TCAP_l9 = 113,
 TCAP_le = 114, TCAP_LE = 115,
 TCAP_li = 116, TCAP_ll = 117,
 TCAP_lm = 118, TCAP_LP = 119,
 TCAP_mb = 120, TCAP_md = 121,
 TCAP_me = 122, TCAP_mh = 123,
 TCAP_mi = 124, TCAP_mk = 125,
 TCAP_mm = 126, TCAP_mo = 127,
 TCAP_mp = 128, TCAP_mr = 129,
 TCAP_ms = 130, TCAP_nc = 131,
 TCAP_nd = 132, TCAP_NF = 133,
 TCAP_nl = 134, TCAP_ns = 135,
 TCAP_nw = 136, TCAP_os = 137,
 TCAP_pb = 138, TCAP_pc = 139,
 TCAP_pf = 140, TCAP_po = 141,
 TCAP_pO = 142, TCAP_ps = 143,
 TCAP_rc = 144, TCAP_RI = 145,
 TCAP_rp = 146, TCAP_rs = 147,
 TCAP_sa = 148, TCAP_sc = 149,
 TCAP_se = 150, TCAP_sf = 151,
 TCAP_SF = 152, TCAP_sg = 153, 
 TCAP_so = 154, TCAP_sr = 155,
 TCAP_SR = 156, TCAP_st = 157, 
 TCAP_ta = 158, TCAP_te = 159,
 TCAP_ti = 160, TCAP_ts = 161,
 TCAP_uc = 162, TCAP_ue = 163,
 TCAP_ug = 164, TCAP_ul = 165,
 TCAP_up = 166, TCAP_UP = 167,
 TCAP_us = 168, TCAP_vb = 169,
 TCAP_ve = 170, TCAP_vi = 171,
 TCAP_vs = 172, TCAP_wi = 173,
 TCAP_ws = 174, TCAP_xb = 175,
 TCAP_xn = 176, TCAP_xs = 177,
 TCAP_xt = 178
};

class TermCap : public IMemoryValidate
{
private:
	// copy constructor might make sense, but we'll forbid it for now
	inline TermCap(const TermCap& pmp) {}
	inline void operator =(const TermCap& pmp) {}

protected:
	Hashtable<String, String> m_caps;
	byte m_acsmap[128];
	bool m_isdos;
	bool m_hasacs;
	bool m_hasS0S1;
	volatile byte m_eos;

	Mutex m_capex;
	String m_termname;

	static const char *m_capCodes[];
	static const char *m_capDefs[];

public:
	TermCap();
	virtual ~TermCap();

	inline byte ACS( int ch )
	{
		ASSERT(256 > ch);
		return m_acsmap[ch];
	}

	inline bool IsDOS() { return m_isdos; }
	inline bool HasACS() { return m_hasacs; }
	inline void DisableACS() { m_hasacs = false; }
	inline bool HasS0S1() { return m_hasS0S1; }
	inline void DisableS0S1() { m_hasS0S1 = false; }

	inline const String& GetTermName() { return m_termname; }
	void SetTerm(const String termName);

	int GetCapNum(TemCapCode code);
	bool GetCapFlag(TemCapCode code);
	bool GetCapString(TemCapCode code, char *buf, int buflen);

	bool GetCapString(TemCapCode code, StringBuffer *sb);
	bool FormatParams(const char *cmdstr, char *buf, int buflen, int param1);
	bool FormatGoto(const char *cmstr, char *buf, int buflen, int x, int y);
	bool FormatGoto(const char *cmstr, StringBuffer *sb, int x, int y);
	bool PrintParams(const char *cmdstr, char *buf, int buflen, int param1, int param2);
	bool PrintParams(const char *cmdstr, char *buf, int buflen, int param1, int param2, int param3, int param4);

	static bool HasTerminalDef( const String& termName );
	inline static bool HasTerminalDef( String &termName ) { return HasTerminalDef(termName.GetChars()); }

	static inline const char *GetCapDesc(TemCapCode code) { return m_capDefs[(int)code]; }
	static inline const char *GetCapTxt(TemCapCode code) { return m_capCodes[(int)code]; }

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

}

#endif
