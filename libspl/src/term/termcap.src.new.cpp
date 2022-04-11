
const char *termcap_src_new = 
	"#### Specials\n" \
	"#\n" \
	"# Special \"terminals\".  These are used to label tty lines when you don't\n" \
	"# know what kind of terminal is on it.  The characteristics of an unknown\n" \
	"# terminal are the lowest common denominator - they look about like a ti 700.\n" \
	"# The last one, \"other\", is like unknown but it allows an escape from software\n" \
	"# that insists that a \"real\" unknown terminal is merely so far unspecified.\n" \
	"#\n" \
	"\n" \
	"dumb:\\\n" \
	"	:am:\\\n" \
	"	:co#80:\\\n" \
	"	:bl=^G:cr=^M:do=^J:sf=^J:\n" \
	"unknown:\\\n" \
	"	:gn:\\\n" \
	"	:tc=dumb:\n" \
	"other|none of the above, but not exactly unknown:\\\n" \
	"	:am:gn:\\\n" \
	"	:co#80:\\\n" \
	"	:cl=^M^J:do=^J:ho=^M:\n" \
	"\n" \
	"arpanet|bussiplexer|dialup|ethernet|network|net|patch|plugboard|switch|network switch or dialup:\\\n" \
	"	:tc=unknown:\n" \
	"lpr|printer|print|printing|line printer:\\\n" \
	"	:hc:os:\\\n" \
	"	:co#132:li#66:\\\n" \
	"	:bl=^G:cr=^M:do=^J:ff=^L:le=^H:sf=^J:\n" \
	"\n" \
	"#### ANSI terminals and terminal emulators\n" \
	"#\n" \
	"# See near the end of this file for details on ANSI conformance.\n" \
	"# Don't mess with these entries!  Lots of other entries depend on them!\n" \
	"#\n" \
	"# This section lists entries in a least-capable to most-capable order.\n" \
	"# if you're in doubt about what `ANSI' matches yours, try them in that\n" \
	"# order and back off from the first that breaks.\n" \
	"\n" \
	"# (ansi: changed \":pt:\" to \":it#8:\" -- esr)\n" \
	"ansi-mini|any ansi terminal with pessimistic assumptions:\\\n" \
	"	:am:bs:\\\n" \
	"	:co#80:it#8:li#24:\\\n" \
	"	:ce=\\E[K:cl=\\E[;H\\E[2J:cm=\\E[%i%d;%dH:do=\\E[B:\\\n" \
	"	:ho=\\E[H:le=\\E[D:nd=\\E[C:up=\\E[A:\n" \
	"\n" \
	"# Color controls corresponding to the ANSI.SYS de-facto standard\n" \
	"# (This is not a standalone entry)\n" \
	"ansi-pc-color:\\\n" \
	"	:Co#8:NC#3:pa#64:\\\n" \
	"	:AB=\\E[4%p1%dm:AF=\\E[3%p1%dm:\\\n" \
	"	:..Sb=\\E[4%?%p1%{1}%=%t4%e%p1%{3}%=%t6%e%p1%{4}%=%t1%e%p1%{6}%=%t3%e%p1%d%;m:\\\n" \
	"	:..Sf=\\E[3%?%p1%{1}%=%t4%e%p1%{3}%=%t6%e%p1%{4}%=%t1%e%p1%{6}%=%t3%e%p1%d%;m:\\\n" \
	"	:op=\\E[37;40m:\n" \
	"\n" \
	"# Procomm and some other ANSI emulations don't recognize all of the ANSI-\n" \
	"# standard capabilities.  This entry deletes cuu, cuf, cud, cub, and vpa/hpa\n" \
	"# capabilities, forcing curses to use repetitions of cuu1, cuf1, cud1 and cub1.\n" \
	"# Also deleted ich and ich1, as QModem up to 5.03 doesn't recognize these.\n" \
	"# Finally, we delete rep and ri, which seem to confuse many emulators.\n" \
	"# On the other hand, we can count on these programs doing rmacs/smacs/sgr.\n" \
	"# From: Eric S. Raymond <esr@snark.thyrsus.com> July 25 1995\n" \
	"pcansi-mono|ibm-pc terminal programs claiming to be ansi (mono mode):\\\n" \
	"	:am:bs:mi:ms:pt:\\\n" \
	"	:co#80:it#8:li#24:\\\n" \
	"	:ae=\\E[10m:al=\\E[L:as=\\E[12m:bl=^G:bt=\\E[Z:cd=\\E[J:\\\n" \
	"	:ce=\\E[K:cl=\\E[H\\E[J:cm=\\E[%i%d;%dH:cr=^M:ct=\\E[2g:\\\n" \
	"	:dc=\\E[P:dl=\\E[M:do=\\E[B:ho=\\E[H:kb=^H:kd=\\E[B:\\\n" \
	"	:kh=\\E[H:kl=\\E[D:kr=\\E[C:ku=\\E[A:le=\\E[D:mb=\\E[5m:\\\n" \
	"	:md=\\E[1m:me=\\E[0m:mk=\\E[9m:mr=\\E[7m:nd=\\E[C:\\\n" \
	"	:..sa=\\E[0;10%?%p1%t;7%;%?%p2%t;4%;%?%p3%t;7%;%?%p4%t;5%;%?%p6%t;1%;%?%p7%t;8%;%?%p8%t;11%;%?%p9%t;12%;m:\\\n" \
	"	:se=\\E[m:sf=^J:so=\\E[7m:st=\\EH:ta=^I:ue=\\E[m:up=\\E[A:\\\n" \
	"	:us=\\E[4m:\n" \
	"pcansi-mono25|ibm-pc terminal programs with 25 lines (mono mode):\\\n" \
	"	:li#25:\\\n" \
	"	:tc=pcansi-mono:\n" \
	"pcansi-mono33|ibm-pc terminal programs with 33 lines (mono mode):\\\n" \
	"	:li#33:\\\n" \
	"	:tc=pcansi-mono:\n" \
	"pcansi-mono43|ibm-pc terminal programs with 43 lines (mono mode):\\\n" \
	"	:li#43:\\\n" \
	"	:tc=pcansi-mono:\n" \
	"# The color versions.  All PC emulators do color...\n" \
	"pcansi|ibm-pc terminal programs claiming to be ansi:\\\n" \
	"	:tc=ansi-pc-color:tc=pcansi-mono:\n" \
	"pcansi-25|ansi25|ibm-pc terminal programs with 25 lines:\\\n" \
	"	:li#25:\\\n" \
	"	:tc=pcansi:\n" \
	"pcansi-33|ansi33|ibm-pc terminal programs with 33 lines:\\\n" \
	"	:li#33:\\\n" \
	"	:tc=pcansi:\n" \
	"pcansi-43|ansi43|ibm-pc terminal programs with 43 lines:\\\n" \
	"	:li#43:\\\n" \
	"	:tc=pcansi:\n" \
	"\n" \
	"# From: Eric S. Raymond <esr@snark.thyrsus.com> Feb 3 1995\n" \
	"# ansi-mono -- full X.364 with ANSI.SYS-compatible attributes, no color.\n" \
	"# Function-key mappings aren't in X3.64 but these are pretty standard.\n" \
	"# If you want pound signs rather than dollars, replace `B' with `A'\n" \
	"# in the s?ds capabilities.\n" \
	"ansi-mono|ANSI X3.64-1979 terminal with ANSI.SYS compatible attributes:\\\n" \
	"	:5i:\\\n" \
	"	:AL=\\E[%dL:DC=\\E[%dP:DL=\\E[%dM:DO=\\E[%dB:F1=\\E[W:\\\n" \
	"	:F2=\\E[X:IC=\\E[%d@:LE=\\E[%dD:RI=\\E[%dC:SF=\\E[%dS:\\\n" \
	"	:SR=\\E[%dT:UP=\\E[%dA:cb=\\E[1K:ch=\\E[%dG:ct=\\E[2g:\\\n" \
	"	:cv=\\E[%dd:ec=\\E[%dX:ei=:im=:k1=\\E[M:k2=\\E[N:k3=\\E[O:\\\n" \
	"	:k4=\\E[P:k5=\\E[Q:k6=\\E[R:k7=\\E[S:k8=\\E[T:k9=\\E[U:\\\n" \
	"	:k;=\\E[V:kB=\\E[Z:kI=\\E[L:kb=^H:kd=\\E[B:kl=\\E[D:\\\n" \
	"	:kr=\\E[C:ku=\\E[A:me=\\E[0;10m:nw=\\r\\E[S:pf=\\E[4i:\\\n" \
	"	:po=\\E[5i:..rp=%p1%c\\E[%p2%{1}%-%db:s0=\\E(B:s1=\\E)B:\\\n" \
	"	:s2=\\E*B:s3=\\E+B:\\\n" \
	"	:..sa=\\E[0;10%?%p1%t;7%;%?%p2%t;4%;%?%p3%t;7%;%?%p4%t;5%;%?%p6%t;1%;%?%p7%t;8%;%?%p8%t;11%;%?%p9%t;12%;m:\\\n" \
	"	:ta=\\E[I:tc=pcansi:\n" \
	"\n" \
	"# ansi -- this terminfo expresses the largest subset of X3.64 that will fit in\n" \
	"# standard terminfo.  Assumes ANSI.SYS-compatible attributes and color\n" \
	"# From: Eric S. Raymond <esr@snark.thyrsus.com> Feb 12 1995\n" \
	"ansi|ansi/pc-term compatible with color:\\\n" \
	"	:u6=\\E[%d;%dR:u7=\\E[6n:..u8=\\E[?%[;0123456789]c:\\\n" \
	"	:u9=\\E[c:tc=ansi-pc-color:tc=ansi-mono:\n" \
	"\n" \
	"#\n" \
	"# ANSI.SYS entries\n" \
	"#\n" \
	"# Cannot use :pt:, it does not work (why?).  :ho: seems required (why?). [gts]\n" \
	"# Caution: 4.3 BSD tset does not pass li#25 to stty rows except during login?\n" \
	"# :cl: clears attributes and sets wrap at margin before clearing the screen.\n" \
	"# (ansi.sys: removed obsolete \":ma=^Hh^Jj^Kk^Ll^^H:\" -- esr)\n" \
	"# From: greg small <gts@populi.berkeley.edu>\n" \
	"ansi.sys|ansisys|PC-DOS 3.1 ANSI.SYS:\\\n" \
	"	:am:bs:ms:\\\n" \
	"	:co#80:li#25:\\\n" \
	"	:ae=\\E[10:as=\\E[12:ce=\\E[K:cl=\\E[m\\E[7h\\E[2J:\\\n" \
	"	:cm=\\E[%i%d;%dH:ho=\\E[H:\\\n" \
	"	:is=U1 PC-DOS 3.1 ANSI.SYS 9-23-86\\n\\E[m\\E[7h:kd=^J:\\\n" \
	"	:kh=^^:kl=^H:kr=^L:ku=^K:md=\\E[1m:me=\\E[0;10m:\\\n" \
	"	:mr=\\E[7m:nd=\\E[C:se=\\E[m:so=\\E[1m:ue=\\E[m:up=\\E[A:\\\n" \
	"	:us=\\E[4m:\n" \
	"#\n" \
	"# Define IBM PC keypad keys for vi as per MS-Kermit while using ANSI.SYS.\n" \
	"# This should only be used when the terminal emulator cannot redefine the keys.\n" \
	"# Since redefining keys with ansi.sys also affects PC-DOS programs, the key\n" \
	"# definitions must be restored.  If the terminal emulator is quit while in vi\n" \
	"# or others using :ks:ke:, the keypad keys will not be defined as per PC-DOS.\n" \
	"# The PgUp and PgDn are prefixed with ESC so that tn3270 can be used on Unix\n" \
	"# (^U and ^D are already defined for tn3270).  The ESC is safe for vi but it\n" \
	"# does \"beep\".  ESC ESC i is used for Ins to avoid tn3270 ESC i for coltab.\n" \
	"# Left arrow is always BS, because PC-dos can tolerate this change.\n" \
	"# Caution: vi is limited to 256 string bytes, longer crashes or weirds out vi.\n" \
	"# Consequently the End keypad key could not be set (it is relatively safe and\n" \
	"# actually useful because it sends ^@ O, which beeps and opens a line above).\n" \
	"ansi.sysk|ansisysk|PC-DOS 3.1 ANSI.SYS with keypad redefined for vi:\\\n" \
	"	:is=U2 PC-DOS 3.1 ANSI.SYS with keypad redefined for vi 9-29-86\\n\\E[;75;8p:\\\n" \
	"	:ke=\\E[;71;0;71p\\E[;72;0;72p\\E[;73;0;73p\\E[;77;0;77p\\E[;80;0;80p\\E[;81;0;81p\\E[;82;0;82p\\E[;83;0;83p:\\\n" \
	"	:ks=\\E[;71;30p\\E[;72;11p\\E[;73;27;21p\\E[;77;12p\\E[;80;10p\\E[;81;27;4p\\E[;82;27;27;105p\\E[;83;127p:tc=ansi.sys:\n" \
	"#\n" \
	"# Adds ins/del line/character, hence vi reverse scrolls/inserts/deletes nicer.\n" \
	"nansi.sys|nansisys|PC-DOS Public Domain NANSI.SYS:\\\n" \
	"	:al=\\E[1L:dc=\\E[1P:dl=\\E[1M:ei=:ic=\\E[1@:im=:\\\n" \
	"	:is=U3 PC-DOS Public Domain NANSI.SYS 9-23-86\\n:tc=ansi.sys:\n" \
	"#\n" \
	"# See ansi.sysk and nansi.sys above.\n" \
	"nansi.sysk|nansisysk|PC-DOS Public Domain NANSI.SYS with keypad redefined for vi:\\\n" \
	"	:al=\\E[1L:dc=\\E[1P:dl=\\E[1M:ei=:ic=\\E[1@:im=:\\\n" \
	"	:is=U4 PC-DOS Public Domain NANSI.SYS with keypad redefined for vi 9-29-86\\n\\E[;75;8p:tc=ansi.sysk:\n" \
	"\n" \
	"cygwin|cygwin:\\\n" \
	"	:xn@:op=\\E[39;49m:Km=\\E[M:te=\\E[2J\\E[?47l\\E8:ti=\\E7\\E[?47h:tc=linux:cr=^J:\n" \
	"\n" \
	"#### ANSI console types\n" \
	"#\n" \
	"\n" \
	"# This entry is good for the 1.1.47 version of the Linux console driver.\n" \
	"#\n" \
	"# It assumes that you want A_PROTECT mapped to the alternate character set\n" \
	"# mode that permits IBM ROM characters to be displayed (this is the assumption\n" \
	"# used by ncurses version 1.9 and after, in order not to collide with the\n" \
	"# internationalization attribute values specified in the XSI Curses standard).\n" \
	"#\n" \
	"# We use \\E11m for rmacs rather than \\E12m so the acsc string can use the ROM\n" \
	"# graphics for control characters such as the diamond, up arrow and down-arrow.\n" \
	"# This trick could work with other Intel consoles like the att6386 and pc3.\n" \
	"#\n" \
	"# Note: there are numerous broken linux entries out there, which didn't screw\n" \
	"# up BSD termcap but hose ncurses's smarter cursor-movement optimization.\n" \
	"# One common pathology is an incorrect tab length of 4.  Also note that the \n" \
	"# hpa=\\E[%dG/vpa=\\E[%dd capabilities seem not to be reliable.  To reproduce\n" \
	"# the bug, re-introduce them and run worm -T 200 212 from the ncurses\n" \
	"# test suite, save the trace, then worm -N -T 200 212.  Observe that the first\n" \
	"# run fails to properly delete some worm segments, then diff the trace files.\n" \
	"#\n" \
	"# From: Eric S. Raymond <esr@snark.thyrsus.com> 23 July 1995\n" \
	"linux|linux console:\\\n" \
	"	:am:bs:eo:mi:ms:ut:xn:xo:\\\n" \
	"	:Co#8:co#80:it#8:li#25:pa#64:\\\n" \
	"	:&7=^Z:@7=\\E[4~:kh=\\E[1~:kH=\\E[4~:AB=\\E[4%dm:AF=\\E[3%dm:\\\n" \
	"	:AL=\\E[%dL:DC=\\E[%dP:DL=\\E[%dM:F1=\\E[23~:F2=\\E[24~:\\\n" \
	"	:F3=\\E[25~:F4=\\E[26~:F5=\\E[28~:F6=\\E[29~:F7=\\E[31~:\\\n" \
	"	:F8=\\E[32~:F9=\\E[33~:FA=\\E[34~:IC=\\E[%d@:K2=\\E[G:\\\n" \
	"	:S2=\\E[11m:S3=\\E[10m:Sb=\\E[%+(m:Sf=\\E[%+^^m:\\\n" \
	"	:ac=`\\004a\\261f\\370g\\361h\\260j\\331k\\277l\\332m\\300n\\305o~q\\304r\\362s_t\\303u\\264v\\301w\\302x\\263y\\371z\\372{\\373|\\374}\\375~\\376.\\031-\\030\\054\\021+^P0\\333:\\\n" \
	"	:ae=\\E[10m:al=\\E[L:as=\\E[11m:bl=^G:cd=\\E[J:ce=\\E[K:\\\n" \
	"	:cl=\\E[H\\E[J:cm=\\E[%i%d;%dH:cr=^J:cs=\\E[%i%d;%dr:\\\n" \
	"	:ct=\\E[3g:dc=\\E[P:dl=\\E[M:do=^J:ei=\\E[4l:ho=\\E[H:\\\n" \
	"	:ic=\\E[@:im=\\E[4h:k1=\\E[[A:k2=\\E[[B:k3=\\E[[C:\\\n" \
	"	:k4=\\E[[D:k5=\\E[[E:k6=\\E[17~:k7=\\E[18~:k8=\\E[19~:\\\n" \
	"	:k9=\\E[20~:k;=\\E[21~:kD=\\E[3~:kI=\\E[2~:kN=\\E[6~:\\\n" \
	"	:kP=\\E[5~:kb=^H:kd=\\E[B:kh=\\E[1~:kl=\\E[D:kr=\\E[C:\\\n" \
	"	:ku=\\E[A:le=^H:mb=\\E[5m:md=\\E[1m:me=\\E[0;10m:\\\n" \
	"	:mr=\\E[7m:nd=\\E[C:nw=^M^J:op=\\E[37;40m:r1=\\Ec:rc=\\E8:\\\n" \
	"	:sc=\\E7:se=\\E[m:sf=^J:so=\\E[7m:sr=\\EM:st=\\EH:ta=^I:\\\n" \
	"	:u6=\\E[%d;%dR:u7=\\E[6n:u8=\\E[?6c:u9=\\E[c:ue=\\E[24m:\\\n" \
	"	:up=\\E[A:us=\\E[4m:vb=\\E[?5h\\E[?5l:ve=\\E[?25h:\\\n" \
	"	:vi=\\E[?25l:\n" \
	"linux-mono|Linux console, no color:\\\n" \
	"	:Co@:pa@:\\\n" \
	"	:AB@:Sb@:Sf@:tc=linux:\n" \
	"\n" \
	"#	Reconstructed via infocmp from file: /usr/local/share/terminfo/r/rxvt\n" \
	"rxvt|rxvt terminal emulator (X Window System):\\\n" \
	"	:am:eo:km:mi:ms:xn:xo:\\\n" \
	"	:co#80:it#8:li#24:\\\n" \
	"	:AL=\\E[%dL:DC=\\E[%dP:DL=\\E[%dM:DO=\\E[%dB:IC=\\E[%d@:\\\n" \
	"	:K1=\\EOw:K2=\\EOu:K3=\\EOy:K4=\\EOq:K5=\\EOs:LE=\\E[%dD:\\\n" \
	"	:RI=\\E[%dC:UP=\\E[%dA:ae=^O:al=\\E[L:as=^N:bl=^G:cd=\\E[J:\\\n" \
	"	:ce=\\E[K:cl=\\E[H\\E[2J:cm=\\E[%i%d;%dH:cr=^M:\\\n" \
	"	:cs=\\E[%i%d;%dr:ct=\\E[3g:dc=\\E[P:dl=\\E[M:do=^J:ei=\\E[4l:\\\n" \
	"	:ho=\\E[H:i1=\\E[?47l\\E=\\E[?1l:ic=\\E[@:im=\\E[4h:\\\n" \
	"	:is=\\E[r\\E[m\\E[2J\\E[H\\E[?7h\\E[?1;3;4;6l\\E[4l:\\\n" \
	"	:k0=\\E[21~:k1=\\E[11~:k2=\\E[12~:k3=\\E[13~:k4=\\E[14~:\\\n" \
	"	:k5=\\E[15~:k6=\\E[17~:k7=\\E[18~:k8=\\E[19~:k9=\\E[20~:\\\n" \
	"	:kD=\\E[3~:kI=\\E[2~:kN=\\E[6~:kP=\\E[5~:kb=^H:kd=\\E[B:ke=\\E>:\\\n" \
	"	:kh=\\E[7~:kl=\\E[D:kr=\\E[C:ks=\\E=:ku=\\E[A:le=^H:mb=\\E[5m:\\\n" \
	"	:md=\\E[1m:me=\\E[m\\017:mr=\\E[7m:nd=\\E[C:rc=\\E8:sc=\\E7:\\\n" \
	"	:se=\\E[27m:sf=^J:so=\\E[7m:sr=\\EM:st=\\EH:ta=^I:\\\n" \
	"	:te=\\E[2J\\E[?47l\\E8:ti=\\E7\\E[?47h:ue=\\E[24m:up=\\E[A:\\\n" \
	"	:us=\\E[4m:vb=\\E[?5h\\E[?5l:ve=\\E[?25h:vi=\\E[?25l:\\\n" \
	"	:vs=\\E[?25h:\n" \
	"\n" \
	"rxvt-cygwin-native|rxvt terminal emulator (native MS Window System port) on cygwin:\\\n" \
	"	:ac=+\\257,\\256-^0\\333`\\004a\\261f\\370g\\361h\\260j\\331k\\277l\\332m\\300n\\305o~p\\304q\\304r\\304s_t\\303u\\264v\\301w\\302x\\263y\\363z\\362{\\343|\\330~\\376:\\\n" \
	"	:tc=rxvt:\n" \
	"\n" \
	"#	Reconstructed via infocmp from file: /usr/local/share/terminfo/v/vt100\n" \
	"vt100|vt100-am|dec vt100 (w/advanced video):\\\n" \
	"	:am:ms:xn:xo:\\\n" \
	"	:co#80:it#8:li#24:vt#3:\\\n" \
	"	:DO=\\E[%dB:K1=\\EOq:K2=\\EOr:K3=\\EOs:K4=\\EOp:K5=\\EOn:\\\n" \
	"	:LE=\\E[%dD:RI=\\E[%dC:UP=\\E[%dA:ae=^O:as=^N:bl=^G:cd=\\E[J:\\\n" \
	"	:ce=\\E[K:cl=\\E[H\\E[J:cm=\\E[%i%d;%dH:cr=^M:cs=\\E[%i%d;%dr:\\\n" \
	"	:ct=\\E[3g:do=^J:ho=\\E[H:k0=\\EOy:k1=\\EOP:k2=\\EOQ:k3=\\EOR:\\\n" \
	"	:k4=\\EOS:k5=\\EOt:k6=\\EOu:k7=\\EOv:k8=\\EOl:k9=\\EOw:kb=^H:\\\n" \
	"	:kd=\\EOB:ke=\\E[?1l\\E>:kl=\\EOD:kr=\\EOC:ks=\\E[?1h\\E=:\\\n" \
	"	:ku=\\EOA:le=^H:mb=\\E[5m:md=\\E[1m:me=\\E[m\\017:mr=\\E[7m:\\\n" \
	"	:nd=\\E[C:rc=\\E8:\\\n" \
	"	:..sa=\\E[0%?%p1%p6%|%t;1%;%?%p2%t;4%;%?%p1%p3%|%t;7%;%?%p4%t;5%;m%?%p9%t\\016%e\\017%;:\\\n" \
	"	:sc=\\E7:se=\\E[m:sf=^J:so=\\E[7m:sr=\\EM:st=\\EH:ta=^I:ue=\\E[m:\\\n" \
	"	:up=\\E[A:us=\\E[4m:\n" \
	"\n" \
	"#	Reconstructed via infocmp from file: /usr/share/terminfo/x/xterm-r6\n" \
	"xterm-r6|xterm|xterm X11R6 version:\\\n" \
	"	:am:km:mi:ms:xn:\\\n" \
	"	:co#80:it#8:li#24:\\\n" \
	"	:AL=\\E[%dL:DC=\\E[%dP:DL=\\E[%dM:DO=\\E[%dB:LE=\\E[%dD:\\\n" \
	"	:RI=\\E[%dC:UP=\\E[%dA:ae=^O:al=\\E[L:as=^N:bl=^G:cd=\\E[J:\\\n" \
	"	:ce=\\E[K:cl=\\E[H\\E[2J:cm=\\E[%i%d;%dH:cr=^M:\\\n" \
	"	:cs=\\E[%i%d;%dr:ct=\\E[3g:dc=\\E[P:dl=\\E[M:do=^J:ei=\\E[4l:\\\n" \
	"	:ho=\\E[H:im=\\E[4h:\\\n" \
	"	:is=\\E7\\E[r\\E[m\\E[?7h\\E[?1;3;4;6l\\E[4l\\E8\\E>:k1=\\EOP:\\\n" \
	"	:k2=\\EOQ:k3=\\EOR:k4=\\EOS:k5=\\E[15~:k6=\\E[17~:k7=\\E[18~:\\\n" \
	"	:k8=\\E[19~:k9=\\E[20~:kD=\\E[3~:kI=\\E[2~:kN=\\E[6~:kP=\\E[5~:\\\n" \
	"	:kb=^H:kd=\\EOB:ke=\\E[?1l\\E>:kh=\\E[1~:kl=\\EOD:kr=\\EOC:\\\n" \
	"	:ks=\\E[?1h\\E=:ku=\\EOA:le=^H:md=\\E[1m:me=\\E[m:mr=\\E[7m:\\\n" \
	"	:nd=\\E[C:rc=\\E8:sc=\\E7:se=\\E[m:sf=^J:so=\\E[7m:sr=\\EM:ta=^I:\\\n" \
	"	:te=\\E[2J\\E[?47l\\E8:ti=\\E7\\E[?47h:ue=\\E[m:up=\\E[A:\\\n" \
	"	:us=\\E[4m:\n" \
	"SC|screen|VT 100/ANSI X3.64 virtual terminal:\\\n" \
	"	:am:xn:ms:mi:G0:km:\\\n" \
	"	:DO=\\E[%dB:LE=\\E[%dD:RI=\\E[%dC:UP=\\E[%dA:bs:bt=\\E[Z:\\\n" \
	"	:cb=\\E[1K:cd=\\E[J:ce=\\E[K:cl=\\E[H\\E[J:cm=\\E[%i%d;%dH:ct=\\E[3g:\\\n" \
	"	:do=^J:nd=\\E[C:pt:rc=\\E8:rs=\\Ec:sc=\\E7:st=\\EH:up=\\EM:\\\n" \
	"	:le=^H:bl=^G:cr=^M:it#8:ho=\\E[H:nw=\\EE:ta=^I:is=\\E)0:\\\n" \
	"	:li#24:co#80:us=\\E[4m:ue=\\E[24m:so=\\E[3m:se=\\E[23m:\\\n" \
	"	:mb=\\E[5m:md=\\E[1m:mr=\\E[7m:me=\\E[m:sr=\\EM:al=\\E[L:\\\n" \
	"	:AL=\\E[%dL:dl=\\E[M:DL=\\E[%dM:cs=\\E[%i%d;%dr:dc=\\E[P:\\\n" \
	"	:DC=\\E[%dP:im=\\E[4h:ei=\\E[4l:IC=\\E[%d@:\\\n" \
	"	:ks=\\E[?1h\\E=:ke=\\E[?1l\\E>:vb=\\Eg:\\\n" \
	"	:ku=\\EOA:kd=\\EOB:kr=\\EOC:kl=\\EOD:\\\n" \
	"	:k1=\\EOP:k2=\\EOQ:k3=\\EOR:k4=\\EOS:k5=\\E[15~:k6=\\E[17~:\\\n" \
	"	:k7=\\E[18~:k8=\\E[19~:k9=\\E[20~:k;=\\E[21~:F1=\\E[23~:F2=\\E[24~:\\\n" \
	"	:kh=\\E[1~:kI=\\E[2~:kD=\\E[3~:kH=\\E[4~:@7=\\E[4~:kP=\\E[5~:\\\n" \
	"	:kN=\\E[6~:eA=\\E(B\\E)0:as=^N:ae=^O:ti=\\E[?1049h:te=\\E[?1049l:\\\n" \
	"	:vi=\\E[?25l:ve=\\E[34h\\E[?25h:vs=\\E[34l:\\\n" \
	"	:Co#8:pa#64:AF=\\E[3%dm:AB=\\E[4%dm:op=\\E[39;49m:AX:\\\n" \
	"	:ac=``aaffggjjkkllmmnnooppqqrrssttuuvvwwxxyyzz{{||}}~~..--++,,hhII00:\n" \
	"SB|screen-bce|VT 100/ANSI X3.64 virtual terminal with bce:\\\n" \
	"	:ut:tc=screen:\n" \
	"SH|screen-s|VT 100/ANSI X3.64 virtual terminal with hardstatus line:\\\n" \
	"	:ts=\\E_:fs=\\E\\\\:ds=\\E_\\E\\\\:tc=screen:\n" \
	""
;

