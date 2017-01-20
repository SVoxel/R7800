#########################################################################
# Makefile.in for Samba - rewritten for autoconf support
# Copyright Andrew Tridgell 1992-1998
# Copyright (C) 2001 by Martin Pool <mbp@samba.org>
# Copyright Andrew Bartlett 2002
# Copyright (C) 2003 Jim McDonough <jmcd@us.ibm.com>
# Copyright (C) 2002-2003 Jelmer Vernooij <jelmer@samba.org>
# Copyright (C) 2006 Lars Mueller <lars@samba.org>
###########################################################################

prefix=/usr
exec_prefix=/usr

selftest_prefix=./
smbtorture4_path=

LIBS= -lcrypt -lresolv -ldl
CC=gcc
SHLD=${CC} ${CFLAGS}
CFLAGS= -O -D_SAMBA_BUILD_
CPPFLAGS=-DHAVE_CONFIG_H 
EXEEXT=
LDFLAGS=
AR=ar
LDSHFLAGS=-shared -Wl,-Bsymbolic 
WINBIND_NSS_LDSHFLAGS=-shared -Wl,-Bsymbolic 
AWK=gawk
DYNEXP=-Wl,--export-dynamic
PYTHON=
PERL=/usr/bin/perl

TERMLDFLAGS=
TERMLIBS=
PRINT_LIBS=
AUTH_LIBS=-lcrypt 
ACL_LIBS=
PASSDB_LIBS= 
IDMAP_LIBS=
KRB5LIBS=
LDAP_LIBS=
NSCD_LIBS=

INSTALLCMD=/usr/bin/install -c
INSTALLLIBCMD_SH=$(INSTALLCMD)
INSTALLLIBCMD_A=:
UNINSTALLLIBCMD_SH=rm -f
UNINSTALLLIBCMD_A=:


srcdir=${shell pwd}
builddir=${shell pwd}
SHELL=/bin/sh
DESTDIR=/

# XXX: Perhaps this should be /bin/bash instead -- apparently autoconf
# will search for a POSIX-compliant shell, and that might not be
# /bin/sh on some platforms.  I guess it's not a big problem -- mbp

# See the autoconf manual "Installation Directory Variables" for a
# discussion of the subtle use of these variables.

BASEDIR= /usr
BINDIR = /usr/bin
# sbindir is mapped to bindir when compiling SAMBA in 2.0.x compatibility mode.
SBINDIR = /usr/sbin
ROOTSBINDIR = ${SBINDIR}
LIBDIR = ${exec_prefix}/lib
PAMMODULESDIR = ${LIBDIR}/security
VFSLIBDIR = $(LIBDIR)/vfs
PDBLIBDIR = $(LIBDIR)/pdb
RPCLIBDIR = $(LIBDIR)/rpc
IDMAPLIBDIR = $(LIBDIR)/idmap
CHARSETLIBDIR = $(LIBDIR)/charset
AUTHLIBDIR = $(LIBDIR)/auth
CONFIGLIBDIR = $(LIBDIR)/config
CONFIGDIR = ${LIBDIR}
VARDIR = /var
MANDIR = /usr/man
DATADIR = /usr/share

# The permissions to give the executables
INSTALLPERMS = 0755

# set these to where to find various files
# These can be overridden by command line switches (see smbd(8))
# or in smb.conf (see smb.conf(5))
LOGFILEBASE = /var/log/samba
CONFIGFILE = $(CONFIGDIR)/smb.conf
LMHOSTSFILE = $(CONFIGDIR)/lmhosts

# This is where smbpasswd et al go
PRIVATEDIR = ${prefix}/private

SMB_PASSWD_FILE = $(PRIVATEDIR)/smbpasswd
PRIVATE_DIR = $(PRIVATEDIR)

# This is where SWAT images and help files go
SWATDIR = ${prefix}/swat

# the directory where lock files go
LOCKDIR = ${VARDIR}/locks

# the directory where pid files go
PIDDIR = ${VARDIR}/locks

LIBSMBCLIENT=bin/libsmbclient.a bin/libsmbclient.so
LIBSMBCLIENT_MAJOR=0
LIBSMBCLIENT_MINOR=1

LIBMSRPC=bin/libmsrpc.a bin/libmsrpc.so
LIBMSRPC_MAJOR=0
LIBMSRPC_MINOR=1

LIBSMBSHAREMODES=bin/libsmbsharemodes.a bin/libsmbsharemodes.so
LIBSMBSHAREMODES_MAJOR=0
LIBSMBSHAREMODES_MINOR=2

FLAGS1 = $(CFLAGS) -I$(srcdir)/popt -I$(srcdir)/iniparser/src -Iinclude -I$(srcdir)/include -I$(srcdir)/tdb -I$(srcdir)/smbwrapper -I. $(CPPFLAGS) -I$(srcdir) -D_SAMBA_BUILD_
FLAGS2 =
FLAGS3 = 
FLAGS4 = 
FLAGS5 = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4)
FLAGS  = $(ISA) $(FLAGS5) 

PASSWD_FLAGS = -DSMB_PASSWD_FILE=\"$(SMB_PASSWD_FILE)\" -DPRIVATE_DIR=\"$(PRIVATE_DIR)\"
PATH_FLAGS1 = -DCONFIGFILE=\"$(CONFIGFILE)\"  -DSBINDIR=\"$(SBINDIR)\"
PATH_FLAGS2 = $(PATH_FLAGS1) -DBINDIR=\"$(BINDIR)\" -DDRIVERFILE=\"$(DRIVERFILE)\" 
PATH_FLAGS3 = $(PATH_FLAGS2) -DLMHOSTSFILE=\"$(LMHOSTSFILE)\" 
PATH_FLAGS4 = $(PATH_FLAGS3) -DSWATDIR=\"$(SWATDIR)\"  -DLOCKDIR=\"$(LOCKDIR)\" -DPIDDIR=\"$(PIDDIR)\"
PATH_FLAGS5 = $(PATH_FLAGS4) -DLIBDIR=\"$(LIBDIR)\" \
	      -DLOGFILEBASE=\"$(LOGFILEBASE)\" -DSHLIBEXT=\"so\"
PATH_FLAGS6 = $(PATH_FLAGS5) -DCONFIGDIR=\"$(CONFIGDIR)\"
PATH_FLAGS = $(PATH_FLAGS6) $(PASSWD_FLAGS)

# Note that all executable programs now provide for an optional executable suffix.

SBIN_PROGS = bin/smbd bin/nmbd bin/swat  bin/winbindd$(EXEEXT)

ROOT_SBIN_PROGS = bin/mount.cifs bin/umount.cifs

BIN_PROGS1 = bin/smbclient bin/net bin/smbspool \
	bin/testparm bin/smbstatus bin/smbget
BIN_PROGS2 = bin/smbcontrol bin/smbtree bin/tdbbackup \
	bin/nmblookup bin/pdbedit bin/tdbdump \
	bin/tdbtool
BIN_PROGS3 = bin/smbpasswd bin/rpcclient bin/smbcacls \
	bin/profiles bin/ntlm_auth \
	bin/smbcquotas bin/eventlogadm 

TORTURE_PROGS = bin/smbtorture bin/msgtest \
	bin/masktest bin/locktest \
	bin/locktest2 bin/nsstest bin/vfstest

BIN_PROGS = $(BIN_PROGS1) $(BIN_PROGS2) $(BIN_PROGS3)  bin/wbinfo$(EXEEXT)

EVERYTHING_PROGS = bin/debug2html bin/smbfilter bin/talloctort \
	bin/log2pcap

SHLIBS =  nsswitch/libnss_winbind.so nsswitch/libnss_wins.so libsmbclient libsmbsharemodes libmsrpc

PAM_MODULES = 

SCRIPTS = $(srcdir)/script/smbtar $(builddir)/script/findsmb

VFS_MODULES =  bin/recycle.so bin/audit.so bin/extd_audit.so bin/full_audit.so bin/netatalk.so bin/fake_perms.so bin/default_quota.so bin/readonly.so bin/cap.so bin/expand_msdfs.so bin/shadow_copy.so
PDB_MODULES = 
RPC_MODULES = 
IDMAP_MODULES = 
CHARSET_MODULES =  bin/CP850.so bin/CP437.so
AUTH_MODULES =  bin/script.so
MODULES = $(VFS_MODULES) $(PDB_MODULES) $(RPC_MODULES) $(IDMAP_MODULES) $(CHARSET_MODULES) $(AUTH_MODULES)

######################################################################
# object file lists
######################################################################

TDBBASE_OBJ = tdb/tdb.o tdb/spinlock.o

TDB_OBJ = $(TDBBASE_OBJ) tdb/tdbutil.o tdb/tdbback.o

SMBLDAP_OBJ =  

VERSION_OBJ = lib/version.o

DOSERR_OBJ = libsmb/doserr.o

SNPRINTF_OBJ = lib/snprintf.o

WBCOMMON_OBJ = nsswitch/wb_common.o

AFS_OBJ = lib/afs.o

AFS_SETTOKEN_OBJ = lib/afs_settoken.o

SERVER_MUTEX_OBJ = lib/server_mutex.o

ERRORMAP_OBJ = libsmb/errormap.o

PASSCHANGE_OBJ = libsmb/passchange.o

SOCKET_WRAPPER_OBJ = lib/socket_wrapper.o

RPC_PARSE_OBJ0 = rpc_parse/parse_prs.o rpc_parse/parse_misc.o

LIB_OBJ = $(VERSION_OBJ) lib/charcnv.o lib/debug.o lib/fault.o \
          lib/getsmbpass.o lib/interface.o lib/md4.o \
          lib/interfaces.o lib/pidfile.o lib/replace.o lib/replace1.o lib/repdir.o lib/timegm.o \
          lib/signal.o lib/system.o lib/sendfile.o lib/time.o \
	  lib/ufc.o lib/genrand.o lib/username.o \
	  lib/util_pw.o lib/access.o lib/smbrun.o \
	  lib/bitmap.o lib/crc32.o $(SNPRINTF_OBJ) lib/dprintf.o \
	  lib/xfile.o lib/wins_srv.o \
	  lib/util_str.o lib/clobber.o lib/util_sid.o lib/util_uuid.o \
	  lib/util_unistr.o lib/util_file.o lib/data_blob.o \
	  lib/util.o lib/util_sock.o lib/sock_exec.o lib/util_sec.o \
	  lib/talloc.o lib/substitute.o lib/fsusage.o \
	  lib/ms_fnmatch.o lib/select.o lib/messages.o \
	  lib/tallocmsg.o lib/dmallocmsg.o libsmb/smb_signing.o \
	  lib/md5.o lib/hmacmd5.o lib/arc4.o lib/iconv.o \
	  nsswitch/wb_client.o $(WBCOMMON_OBJ) \
	  lib/pam_errors.o intl/lang_tdb.o \
	  lib/adt_tree.o lib/gencache.o $(TDB_OBJ) \
	  lib/module.o lib/events.o lib/ldap_escape.o  \
	  lib/secdesc.o lib/util_seaccess.o lib/secace.o lib/secacl.o  \
	  libads/krb5_errs.o lib/system_smbd.o lib/audit.o

LIB_DUMMY_OBJ = lib/dummysmbd.o lib/dummyroot.o
LIB_NONSMBD_OBJ = $(LIB_OBJ) $(LIB_DUMMY_OBJ)

READLINE_OBJ = lib/readline.o

# Also depends on  $(SECRETS_OBJ) $(LIBSAMBA_OBJ)
# Be sure to include them into your application
POPT_LIB_OBJ = lib/popt_common.o 

PARAM_OBJ = dynconfig.o param/loadparm.o param/params.o lib/sharesec.o

KRBCLIENT_OBJ = libads/kerberos.o libads/ads_status.o 

LIBADS_OBJ = libads/ldap.o libads/ldap_printer.o libads/sasl.o \
	     libads/krb5_setpw.o libads/ldap_user.o \
	     libads/ads_struct.o libads/kerberos_keytab.o \
             libads/disp_sec.o libads/ads_utils.o libads/ldap_utils.o \
	     libads/authdata.o libads/cldap.o

LIBADS_SERVER_OBJ = libads/util.o libads/kerberos_verify.o \
		    libads/ldap_schema.o sam/nss_info.o

SECRETS_OBJ = passdb/secrets.o passdb/machine_sid.o 

LIBNMB_OBJ = libsmb/unexpected.o libsmb/namecache.o libsmb/nmblib.o \
	     libsmb/namequery.o libsmb/conncache.o libads/dns.o

LIBSAMBA_OBJ = libsmb/nterr.o libsmb/dcerpc_err.o libsmb/smbdes.o \
	       libsmb/smbencrypt.o libsmb/ntlm_check.o \
	       libsmb/ntlmssp.o libsmb/ntlmssp_parse.o libsmb/ntlmssp_sign.o

LIBSMB_OBJ = libsmb/clientgen.o libsmb/cliconnect.o libsmb/clifile.o \
	     libsmb/clikrb5.o libsmb/clispnego.o libsmb/asn1.o \
	     libsmb/clirap.o libsmb/clierror.o libsmb/climessage.o \
	     libsmb/clireadwrite.o libsmb/clilist.o libsmb/cliprint.o \
	     libsmb/clitrans.o libsmb/clisecdesc.o libsmb/clidgram.o \
	     libsmb/clistr.o libsmb/cliquota.o libsmb/clifsinfo.o libsmb/clidfs.o \
             libsmb/smberr.o libsmb/credentials.o libsmb/pwd_cache.o \
	     libsmb/clioplock.o $(ERRORMAP_OBJ) libsmb/clirap2.o \
	     $(DOSERR_OBJ) \
	     $(RPC_PARSE_OBJ1) $(LIBSAMBA_OBJ) $(LIBNMB_OBJ)

LIBMSRPC_OBJ = rpc_client/cli_lsarpc.o rpc_client/cli_samr.o \
	       rpc_client/cli_netlogon.o rpc_client/cli_srvsvc.o \
	       rpc_client/cli_wkssvc.o rpc_client/cli_dfs.o \
	       rpc_client/cli_reg.o $(RPC_CLIENT_OBJ) \
	       rpc_client/cli_spoolss.o rpc_client/cli_spoolss_notify.o  \
	       rpc_client/cli_ds.o rpc_client/cli_echo.o \
	       rpc_client/cli_shutdown.o rpc_client/cli_svcctl.o 

REGOBJS_OBJ = registry/reg_objects.o

REGISTRY_OBJ = registry/reg_frontend.o registry/reg_cachehook.o registry/reg_printing.o \
               registry/reg_db.o registry/reg_eventlog.o registry/reg_shares.o \
               registry/reg_util.o registry/reg_dynamic.o registry/reg_perfcount.o

RPC_LSA_OBJ = rpc_server/srv_lsa.o rpc_server/srv_lsa_nt.o

RPC_NETLOG_OBJ = rpc_server/srv_netlog.o rpc_server/srv_netlog_nt.o

RPC_SAMR_OBJ = rpc_server/srv_samr.o rpc_server/srv_samr_nt.o \
               rpc_server/srv_samr_util.o

REGFIO_OBJ = registry/regfio.o

RPC_REG_OBJ =  rpc_server/srv_reg.o rpc_server/srv_reg_nt.o $(REGFIO_OBJ)

RPC_LSA_DS_OBJ =  rpc_server/srv_lsa_ds.o rpc_server/srv_lsa_ds_nt.o

RPC_SVC_OBJ = rpc_server/srv_srvsvc.o rpc_server/srv_srvsvc_nt.o

RPC_WKS_OBJ =  rpc_server/srv_wkssvc.o rpc_server/srv_wkssvc_nt.o

RPC_SVCCTL_OBJ =  rpc_server/srv_svcctl.o rpc_server/srv_svcctl_nt.o \
                  services/svc_spoolss.o services/svc_rcinit.o services/services_db.o \
                  services/svc_netlogon.o services/svc_winreg.o \
                  services/svc_wins.o

RPC_NTSVCS_OBJ = rpc_server/srv_ntsvcs.o rpc_server/srv_ntsvcs_nt.o

RPC_DFS_OBJ =  rpc_server/srv_dfs.o rpc_server/srv_dfs_nt.o

RPC_SPOOLSS_OBJ = rpc_server/srv_spoolss.o rpc_server/srv_spoolss_nt.o 

RPC_EVENTLOG_OBJ = rpc_server/srv_eventlog.o rpc_server/srv_eventlog_nt.o rpc_server/srv_eventlog_lib.o

RPC_PIPE_OBJ = rpc_server/srv_pipe_hnd.o \
               rpc_server/srv_pipe.o rpc_server/srv_lsa_hnd.o

RPC_ECHO_OBJ = rpc_server/srv_echo.o rpc_server/srv_echo_nt.o

RPC_SERVER_OBJ =  $(RPC_LSA_OBJ) $(RPC_REG_OBJ) $(RPC_LSA_DS_OBJ) $(RPC_WKS_OBJ) $(RPC_SVCCTL_OBJ) $(RPC_NTSVCS_OBJ) $(RPC_NETLOG_OBJ) $(RPC_DFS_OBJ) $(RPC_SVC_OBJ) $(RPC_SPOOLSS_OBJ) $(RPC_EVENTLOG_OBJ) $(RPC_SAMR_OBJ) $(RPC_PIPE_OBJ)

# this includes only the low level parse code, not stuff
# that requires knowledge of security contexts
RPC_PARSE_OBJ1 = $(RPC_PARSE_OBJ0) rpc_parse/parse_sec.o

RPC_PARSE_OBJ = rpc_parse/parse_lsa.o rpc_parse/parse_net.o \
                rpc_parse/parse_reg.o rpc_parse/parse_rpc.o \
                rpc_parse/parse_samr.o rpc_parse/parse_srv.o \
                rpc_parse/parse_wks.o rpc_parse/parse_ds.o \
		rpc_parse/parse_spoolss.o rpc_parse/parse_dfs.o \
		rpc_parse/parse_echo.o rpc_parse/parse_shutdown.o \
		rpc_parse/parse_svcctl.o \
		rpc_parse/parse_eventlog.o rpc_parse/parse_buffer.o \
                rpc_parse/parse_ntsvcs.o $(REGOBJS_OBJ)

RPC_CLIENT_OBJ = rpc_client/cli_pipe.o

LOCKING_OBJ = locking/locking.o locking/brlock.o locking/posix.o

PASSDB_GET_SET_OBJ = passdb/pdb_get_set.o

PASSDB_OBJ = $(PASSDB_GET_SET_OBJ) passdb/passdb.o passdb/pdb_interface.o \
		passdb/util_wellknown.o passdb/util_builtin.o passdb/pdb_compat.o \
		passdb/util_unixsids.o passdb/lookup_sid.o \
		passdb/login_cache.o  passdb/pdb_smbpasswd.o passdb/pdb_tdb.o \
		lib/account_pol.o lib/privileges.o lib/util_nscd.o

DEVEL_HELP_WEIRD_OBJ = modules/weird.o
CP850_OBJ = modules/CP850.o
CP437_OBJ = modules/CP437.o
CHARSET_MACOSXFS_OBJ = modules/charset_macosxfs.o

GROUPDB_OBJ = groupdb/mapping.o

PROFILE_OBJ = profile/profile.o
PROFILES_OBJ = utils/profiles.o \
               $(REGFIO_OBJ) $(REGOBJS_OBJ) $(ERRORMAP_OBJ) \
	       $(RPC_PARSE_OBJ1) $(PARAM_OBJ) $(LIBSAMBA_OBJ) \
               $(DOSERR_OBJ) $(LIB_OBJ) $(LIB_DUMMY_OBJ) \
               $(POPT_LIB_OBJ) $(SECRETS_OBJ)

OPLOCK_OBJ = smbd/oplock.o smbd/oplock_irix.o smbd/oplock_linux.o

NOTIFY_OBJ = smbd/notify.o smbd/notify_hash.o smbd/notify_kernel.o smbd/notify_fam.o

VFS_AUDIT_OBJ = modules/vfs_audit.o
VFS_EXTD_AUDIT_OBJ = modules/vfs_extd_audit.o
VFS_FULL_AUDIT_OBJ = modules/vfs_full_audit.o
VFS_FAKE_PERMS_OBJ = modules/vfs_fake_perms.o
VFS_RECYCLE_OBJ = modules/vfs_recycle.o
VFS_NETATALK_OBJ = modules/vfs_netatalk.o
VFS_DEFAULT_QUOTA_OBJ = modules/vfs_default_quota.o
VFS_READONLY_OBJ = modules/vfs_readonly.o modules/getdate.o
VFS_CAP_OBJ = modules/vfs_cap.o
VFS_EXPAND_MSDFS_OBJ = modules/vfs_expand_msdfs.o
VFS_SHADOW_COPY_OBJ = modules/vfs_shadow_copy.o
VFS_AFSACL_OBJ = modules/vfs_afsacl.o
VFS_CATIA_OBJ = modules/vfs_catia.o

PLAINTEXT_AUTH_OBJ = auth/pampass.o auth/pass_check.o

SLCACHE_OBJ = libsmb/samlogon_cache.o

DCUTIL_OBJ  = libsmb/namequery_dc.o libsmb/trustdom_cache.o libsmb/trusts_util.o

AUTH_BUILTIN_OBJ = auth/auth_builtin.o
AUTH_DOMAIN_OBJ = auth/auth_domain.o
AUTH_SAM_OBJ = auth/auth_sam.o
AUTH_SERVER_OBJ = auth/auth_server.o
AUTH_UNIX_OBJ = auth/auth_unix.o
AUTH_WINBIND_OBJ = auth/auth_winbind.o
AUTH_SCRIPT_OBJ = auth/auth_script.o

AUTH_OBJ = auth/auth.o  $(AUTH_SAM_OBJ) $(AUTH_UNIX_OBJ) $(AUTH_WINBIND_OBJ) $(AUTH_SERVER_OBJ) $(AUTH_DOMAIN_OBJ) $(AUTH_BUILTIN_OBJ) auth/auth_util.o auth/auth_compat.o \
	   auth/auth_ntlmssp.o \
	   $(PLAINTEXT_AUTH_OBJ) $(SLCACHE_OBJ) $(DCUTIL_OBJ)

MANGLE_OBJ = smbd/mangle.o smbd/mangle_hash.o smbd/mangle_map.o smbd/mangle_hash2.o

SMBD_OBJ_MAIN = smbd/server.o

BUILDOPT_OBJ = smbd/build_options.o

SMBD_OBJ_SRV = smbd/files.o smbd/chgpasswd.o smbd/connection.o \
	       smbd/utmp.o smbd/session.o smbd/map_username.o \
               smbd/dfree.o smbd/dir.o smbd/password.o smbd/conn.o \
	       smbd/share_access.o smbd/fileio.o \
               smbd/ipc.o smbd/lanman.o smbd/negprot.o \
               smbd/message.o smbd/nttrans.o smbd/pipes.o \
               smbd/reply.o smbd/sesssetup.o smbd/trans2.o smbd/uid.o \
	       smbd/dosmode.o smbd/filename.o smbd/open.o smbd/close.o \
	       smbd/blocking.o smbd/sec_ctx.o smbd/srvstr.o \
	       smbd/vfs.o smbd/vfs-wrap.o smbd/statcache.o \
               smbd/posix_acls.o lib/sysacls.o $(SERVER_MUTEX_OBJ) \
	       smbd/process.o smbd/service.o smbd/error.o \
	       printing/printfsp.o lib/sysquotas.o lib/sysquotas_linux.o \
	       lib/sysquotas_xfs.o lib/sysquotas_4A.o \
	       smbd/change_trust_pw.o smbd/fake_file.o \
	       smbd/quotas.o smbd/ntquotas.o $(AFS_OBJ) smbd/msdfs.o \
	       $(AFS_SETTOKEN_OBJ) smbd/aio.o smbd/statvfs.o \
	       smbd/dmapi.o $(MANGLE_OBJ) 

SMBD_OBJ_BASE = $(PARAM_OBJ) $(SMBD_OBJ_SRV) $(LIBSMB_OBJ) \
		$(RPC_SERVER_OBJ) $(RPC_PARSE_OBJ) $(SECRETS_OBJ) \
		$(LOCKING_OBJ) $(PASSDB_OBJ) $(PRINTING_OBJ) $(PROFILE_OBJ) \
		$(LIB_OBJ) $(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) \
		$(NOTIFY_OBJ) $(GROUPDB_OBJ) $(AUTH_OBJ) \
		$(LIBMSRPC_OBJ) \
		$(LIBADS_OBJ) $(KRBCLIENT_OBJ) $(LIBADS_SERVER_OBJ) \
		$(REGISTRY_OBJ) $(POPT_LIB_OBJ) \
		$(BUILDOPT_OBJ) $(SMBLDAP_OBJ)

PRINTING_OBJ = printing/pcap.o printing/print_svid.o printing/print_aix.o \
               printing/print_cups.o printing/print_generic.o \
               printing/lpq_parse.o printing/load.o \
               printing/print_iprint.o

PRINTBASE_OBJ = printing/notify.o printing/printing_db.o
PRINTBACKEND_OBJ = printing/printing.o printing/nt_printing.o $(PRINTBASE_OBJ) 

SMBD_OBJ = $(SMBD_OBJ_BASE) $(SMBD_OBJ_MAIN)
NMBD_OBJ1 = nmbd/asyncdns.o nmbd/nmbd.o nmbd/nmbd_become_dmb.o \
            nmbd/nmbd_become_lmb.o nmbd/nmbd_browserdb.o \
            nmbd/nmbd_browsesync.o nmbd/nmbd_elections.o \
            nmbd/nmbd_incomingdgrams.o nmbd/nmbd_incomingrequests.o \
            nmbd/nmbd_lmhosts.o nmbd/nmbd_logonnames.o nmbd/nmbd_mynames.o \
            nmbd/nmbd_namelistdb.o nmbd/nmbd_namequery.o \
            nmbd/nmbd_nameregister.o nmbd/nmbd_namerelease.o \
            nmbd/nmbd_nodestatus.o nmbd/nmbd_packets.o \
            nmbd/nmbd_processlogon.o nmbd/nmbd_responserecordsdb.o \
            nmbd/nmbd_sendannounce.o nmbd/nmbd_serverlistdb.o \
            nmbd/nmbd_subnetdb.o nmbd/nmbd_winsproxy.o nmbd/nmbd_winsserver.o \
            nmbd/nmbd_workgroupdb.o nmbd/nmbd_synclists.o

NMBD_OBJ = $(NMBD_OBJ1) $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
           $(PROFILE_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ)

SWAT_OBJ1 = web/cgi.o web/diagnose.o web/startstop.o web/statuspage.o \
           web/swat.o web/neg_lang.o 

SWAT_OBJ = $(SWAT_OBJ1) $(PARAM_OBJ) $(PRINTING_OBJ) $(LIBSMB_OBJ) \
	   $(LOCKING_OBJ) $(PASSDB_OBJ) $(SECRETS_OBJ) $(KRBCLIENT_OBJ) \
	   $(LIB_NONSMBD_OBJ) $(GROUPDB_OBJ) $(PLAINTEXT_AUTH_OBJ) \
	   $(POPT_LIB_OBJ) $(SMBLDAP_OBJ) $(RPC_PARSE_OBJ) $(LIBMSRPC_OBJ) \
           $(PASSCHANGE_OBJ)

SMBSH_OBJ = smbwrapper/smbsh.o smbwrapper/shared.o \
            $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(RPC_PARSE_OBJ1) $(LIBSAMBA_OBJ) \
	    $(DOSERR_OBJ) $(SECRETS_OBJ)

STATUS_OBJ = utils/status.o $(LOCKING_OBJ) $(PARAM_OBJ) \
             $(PROFILE_OBJ) $(LIB_NONSMBD_OBJ) $(POPT_LIB_OBJ) \
	     $(SECRETS_OBJ) $(LIBSAMBA_OBJ) $(ERRORMAP_OBJ) $(RPC_PARSE_OBJ1) \
             $(DOSERR_OBJ)
             

SMBCONTROL_OBJ = utils/smbcontrol.o $(LOCKING_OBJ) $(PARAM_OBJ) \
	$(PROFILE_OBJ) $(LIB_NONSMBD_OBJ) $(POPT_LIB_OBJ) \
	$(SECRETS_OBJ) $(LIBSAMBA_OBJ) $(RPC_PARSE_OBJ1) $(DOSERR_OBJ) \
	$(PRINTBASE_OBJ) $(ERRORMAP_OBJ)

SMBTREE_OBJ = utils/smbtree.o $(PARAM_OBJ) \
             $(PROFILE_OBJ) $(LIB_NONSMBD_OBJ) $(LIBSMB_OBJ) \
	     $(KRBCLIENT_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) \
             rpc_client/cli_srvsvc.o rpc_parse/parse_srv.o  \
             rpc_client/cli_pipe.o rpc_parse/parse_rpc.o \
             rpc_client/cli_netlogon.o rpc_parse/parse_net.o

TESTPARM_OBJ = utils/testparm.o \
               $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(POPT_LIB_OBJ) \
	       $(SECRETS_OBJ) $(LIBSAMBA_OBJ) $(RPC_PARSE_OBJ1) $(DOSERR_OBJ)

SMBPASSWD_OBJ = utils/owrt_smbpasswd.o utils/passwd_util.o $(PASSCHANGE_OBJ) \
		$(PARAM_OBJ) $(SECRETS_OBJ) $(LIBSMB_OBJ) $(PASSDB_OBJ) \
		$(GROUPDB_OBJ) $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) \
		$(POPT_LIB_OBJ) $(SMBLDAP_OBJ) $(RPC_PARSE_OBJ) $(LIBMSRPC_OBJ)

PDBEDIT_OBJ = utils/pdbedit.o utils/passwd_util.o $(PARAM_OBJ) $(PASSDB_OBJ) \
		$(LIBSAMBA_OBJ) $(LIB_NONSMBD_OBJ) $(GROUPDB_OBJ) \
		$(SECRETS_OBJ) $(POPT_LIB_OBJ) $(SMBLDAP_OBJ) libsmb/asn1.o \
		$(RPC_PARSE_OBJ1) $(DOSERR_OBJ)

SMBGET_OBJ = utils/smbget.o $(POPT_LIB_OBJ) $(LIBSMBCLIENT_OBJ)

RPCCLIENT_OBJ1 = rpcclient/rpcclient.o rpcclient/cmd_lsarpc.o \
	         rpcclient/cmd_samr.o rpcclient/cmd_spoolss.o \
		 rpcclient/cmd_netlogon.o rpcclient/cmd_srvsvc.o \
		 rpcclient/cmd_dfs.o rpcclient/cmd_reg.o \
		 rpcclient/cmd_ds.o rpcclient/cmd_echo.o \
		 rpcclient/cmd_shutdown.o rpcclient/cmd_test.o \
		 lib/display_sec.o

RPCCLIENT_OBJ = $(RPCCLIENT_OBJ1) \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(LIB_NONSMBD_OBJ) \
             $(RPC_PARSE_OBJ) $(PASSDB_OBJ) $(LIBMSRPC_OBJ) \
             $(READLINE_OBJ) $(GROUPDB_OBJ) $(KRBCLIENT_OBJ) \
	     $(LIBADS_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ) \
	     $(SMBLDAP_OBJ) $(DCUTIL_OBJ)

PAM_WINBIND_OBJ = nsswitch/pam_winbind.o $(WBCOMMON_OBJ) \
		  lib/replace1.o $(SNPRINTF_OBJ) $(INIPARSER_OBJ)

SMBW_OBJ1 = smbwrapper/smbw.o \
		smbwrapper/smbw_dir.o smbwrapper/smbw_stat.o \
		smbwrapper/realcalls.o smbwrapper/shared.o \
		smbwrapper/smbw_cache.o

SMBW_OBJ = $(SMBW_OBJ1) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(PARAM_OBJ) \
                $(LIB_NONSMBD_OBJ)

#SMBWRAPPER_OBJ1 = smbwrapper/wrapped.o

SMBWRAPPER_OBJ = $(SMBW_OBJ) $(SMBWRAPPER_OBJ1)

LIBSMBCLIENT_OBJ = libsmb/libsmbclient.o libsmb/libsmb_compat.o \
		   libsmb/libsmb_cache.o \
		   $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) \
	  	   $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
		   $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) \
		   $(SECRETS_OBJ) $(PASSDB_OBJ) $(SMBLDAP_OBJ) $(GROUPDB_OBJ)

CAC_OBJ = $(LIBSMBCLIENT_OBJ) \
			 libmsrpc/libmsrpc.o libmsrpc/libmsrpc_internal.o \
			 libmsrpc/cac_lsarpc.o libmsrpc/cac_winreg.o libmsrpc/cac_samr.o \
			 libmsrpc/cac_svcctl.o

LIBSMBSHAREMODES_OBJ = libsmb/smb_share_modes.o tdb/tdb.o tdb/spinlock.o

# This shared library is intended for linking with unit test programs
# to test Samba internals.  It's called libbigballofmud.so to
# discourage casual usage.

LIBBIGBALLOFMUD_MAJOR = 0

LIBBIGBALLOFMUD_OBJ = $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ) \
	$(LIBSMB_OBJ) $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_OBJ) \
	$(GROUPDB_OBJ) $(KRBCLIENT_OBJ) $(SMBLDAP_OBJ)

LIBBIGBALLOFMUD_PICOBJS = $(LIBBIGBALLOFMUD_OBJ:.o=.po)

CLIENT_OBJ1 = client/client.o client/clitar.o rpc_client/cli_srvsvc.o \
	rpc_parse/parse_srv.o rpc_client/cli_pipe.o rpc_parse/parse_rpc.o \
	rpc_client/cli_netlogon.o rpc_parse/parse_net.o

CLIENT_OBJ = $(CLIENT_OBJ1) $(PARAM_OBJ) $(LIBSMB_OBJ) \
	     $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) \
             $(READLINE_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) \
	     lib/display_sec.o

TOOL_OBJ = client/smbctool.o client/clitar.o $(PARAM_OBJ) $(LIBSMB_OBJ) \
	     $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) \
             $(READLINE_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ)

NET_OBJ1 = utils/net.o utils/net_ads.o utils/net_domain.o utils/net_help.o \
	   utils/net_rap.o utils/net_rpc.o utils/net_rpc_samsync.o \
	   utils/net_rpc_join.o utils/net_time.o utils/net_lookup.o \
	   utils/net_cache.o utils/net_groupmap.o utils/net_idmap.o \
	   utils/net_status.o utils/net_rpc_printer.o utils/net_rpc_rights.o \
	   utils/net_rpc_service.o utils/net_rpc_registry.o utils/net_usershare.o \
	   utils/netlookup.o utils/net_sam.o utils/net_rpc_shell.o \
	   utils/net_util.o utils/net_rpc_sh_acct.o utils/net_rpc_audit.o \
	   utils/passwd_util.o

NET_OBJ = $(NET_OBJ1) $(PARAM_OBJ) $(SECRETS_OBJ) $(LIBSMB_OBJ) \
	  $(RPC_PARSE_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
	  $(KRBCLIENT_OBJ) $(LIB_NONSMBD_OBJ) \
	  $(LIBMSRPC_OBJ) $(IDMAP_OBJ) \
	  $(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) $(POPT_LIB_OBJ) \
	  $(SMBLDAP_OBJ) $(DCUTIL_OBJ) $(SERVER_MUTEX_OBJ) \
	  $(AFS_OBJ) $(AFS_SETTOKEN_OBJ) $(REGFIO_OBJ) $(READLINE_OBJ) \
	  lib/display_sec.o

CUPS_OBJ = client/smbspool.o $(PARAM_OBJ) $(LIBSMB_OBJ) \
	  $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) $(SECRETS_OBJ)

MOUNT_OBJ = client/smbmount.o \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

MNT_OBJ = client/smbmnt.o lib/replace.o lib/timegm.o $(VERSION_OBJ) $(SNPRINTF_OBJ) 

UMOUNT_OBJ = client/smbumount.o  

CIFS_MOUNT_OBJ = client/mount.cifs.o

CIFS_UMOUNT_OBJ = client/umount.cifs.o

NMBLOOKUP_OBJ = utils/nmblookup.o $(PARAM_OBJ) $(LIBNMB_OBJ) $(RPC_PARSE_OBJ1) $(DOSERR_OBJ) \
               $(LIB_NONSMBD_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) $(LIBSAMBA_OBJ) 

SMBTORTURE_OBJ1 = torture/torture.o torture/nbio.o torture/scanner.o torture/utable.o \
		torture/denytest.o torture/mangle_test.o 

SMBTORTURE_OBJ = $(SMBTORTURE_OBJ1) $(PARAM_OBJ) \
	$(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

MASKTEST_OBJ = torture/masktest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

MSGTEST_OBJ = torture/msgtest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

LOCKTEST_OBJ = torture/locktest.o $(PARAM_OBJ) $(LOCKING_OBJ) $(KRBCLIENT_OBJ) \
               $(LIBSMB_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)  

NSSTEST_OBJ = torture/nsstest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

VFSTEST_OBJ = torture/cmd_vfs.o torture/vfstest.o $(SMBD_OBJ_BASE) $(READLINE_OBJ)

SMBICONV_OBJ = $(PARAM_OBJ) torture/smbiconv.o $(LIB_NONSMBD_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) $(LIBSAMBA_OBJ)

LOG2PCAP_OBJ = utils/log2pcaphex.o

LOCKTEST2_OBJ = torture/locktest2.o $(PARAM_OBJ) $(LOCKING_OBJ) $(LIBSMB_OBJ) \
		$(KRBCLIENT_OBJ) $(LIB_NONSMBD_OBJ) $(SECRETS_OBJ)

SMBCACLS_OBJ = utils/smbcacls.o $(PARAM_OBJ) $(LIBSMB_OBJ) \
			   $(KRBCLIENT_OBJ) $(LIB_NONSMBD_OBJ) $(RPC_PARSE_OBJ) \
			   $(PASSDB_OBJ) $(GROUPDB_OBJ) $(LIBMSRPC_OBJ) $(SECRETS_OBJ) \
			   $(POPT_LIB_OBJ) $(DCUTIL_OBJ) $(LIBADS_OBJ) $(SMBLDAP_OBJ)

SMBCQUOTAS_OBJ = utils/smbcquotas.o $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
		$(PARAM_OBJ) \
		$(LIB_NONSMBD_OBJ) $(RPC_PARSE_OBJ) \
		$(LIBMSRPC_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ) \
		$(PASSDB_OBJ) $(SMBLDAP_OBJ) $(GROUPDB_OBJ)

EVTLOGADM_OBJ0	= utils/eventlogadm.o 

EVTLOGADM_OBJ	= $(EVTLOGADM_OBJ0) $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(REGOBJS_OBJ) \
		$(ERRORMAP_OBJ) $(RPC_PARSE_OBJ1) $(LIBSAMBA_OBJ) $(DOSERR_OBJ) \
		$(SECRETS_OBJ) \
	        registry/reg_eventlog.o rpc_server/srv_eventlog_lib.o registry/reg_util.o \
		registry/reg_db.o 

TALLOCTORT_OBJ = lib/talloctort.o $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) \
                 $(RPC_PARSE_OBJ1) $(DOSERR_OBJ) $(LIBSAMBA_OBJ) $(SECRETS_OBJ)

RPCTORTURE_OBJ = torture/rpctorture.o \
             rpcclient/display.o \
             rpcclient/cmd_lsarpc.o \
             rpcclient/cmd_wkssvc.o \
             rpcclient/cmd_samr.o \
             rpcclient/cmd_srvsvc.o \
             rpcclient/cmd_netlogon.o \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) \
             $(RPC_CLIENT_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_GET_SET_OBJ) $(SECRETS_OBJ)

DEBUG2HTML_OBJ = utils/debug2html.o utils/debugparse.o

SMBFILTER_OBJ = utils/smbfilter.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(SECRETS_OBJ) \
                 $(LIB_NONSMBD_OBJ) $(KRBCLIENT_OBJ) 

PROTO_OBJ = $(SMBD_OBJ_MAIN) \
	    $(SMBD_OBJ_SRV) $(NMBD_OBJ1) $(SWAT_OBJ1) $(LIB_OBJ) $(LIBSMB_OBJ) \
	    $(SMBTORTURE_OBJ1) $(RPCCLIENT_OBJ1) \
	    $(LIBMSRPC_OBJ) $(SMBW_OBJ1) $(SMBWRAPPER_OBJ1) \
	    $(RPC_PIPE_OBJ) $(RPC_PARSE_OBJ) $(KRBCLIENT_OBJ) \
	    $(AUTH_OBJ) $(PARAM_OBJ) $(LOCKING_OBJ) $(SECRETS_OBJ) \
	    $(PRINTING_OBJ) $(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) $(NOTIFY_OBJ) \
	    $(PASSDB_OBJ) $(GROUPDB_OBJ) \
	    $(READLINE_OBJ) $(PROFILE_OBJ) $(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) \
	    $(AUTH_SAM_OBJ) $(REGISTRY_OBJ) $(POPT_LIB_OBJ) \
	    $(RPC_LSA_OBJ) $(RPC_NETLOG_OBJ) $(RPC_SAMR_OBJ) $(RPC_REG_OBJ) $(RPC_LSA_DS_OBJ) \
	    $(RPC_SVC_OBJ) $(RPC_WKS_OBJ) $(RPC_DFS_OBJ) $(RPC_SPOOLSS_OBJ) \
	    $(RPC_ECHO_OBJ) $(RPC_SVCCTL_OBJ) $(RPC_EVENTLOG_OBJ) $(SMBLDAP_OBJ) \
            $(IDMAP_OBJ) libsmb/spnego.o $(PASSCHANGE_OBJ) \
            $(RPC_NTSVCS_OBJ) utils/passwd_util.o

WINBIND_WINS_NSS_OBJ = nsswitch/wins.o $(PARAM_OBJ) \
	$(LIBSMB_OBJ) $(LIB_NONSMBD_OBJ) $(NSSWINS_OBJ) $(KRBCLIENT_OBJ)

WINBIND_WINS_NSS_PICOBJS = $(WINBIND_WINS_NSS_OBJ:.o=.po)

PICOBJS = $(SMBWRAPPER_OBJ:.o=.po)
LIBSMBCLIENT_PICOBJS = $(LIBSMBCLIENT_OBJ:.o=.po)
LIBSMBSHAREMODES_PICOBJS = $(LIBSMBSHAREMODES_OBJ:.o=.po)
CAC_PICOBJS = $(CAC_OBJ:.o=.po)


PAM_SMBPASS_OBJ_0 = pam_smbpass/pam_smb_auth.o pam_smbpass/pam_smb_passwd.o \
		pam_smbpass/pam_smb_acct.o pam_smbpass/support.o \
		$(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
		$(SECRETS_OBJ) $(SMBLDAP_OBJ) $(LIBSAMBA_OBJ) \
		$(RPC_PARSE_OBJ1) $(DOSERR_OBJ)

PAM_SMBPASS_PICOOBJ = $(PAM_SMBPASS_OBJ_0:.o=.po)

IDMAP_OBJ     = sam/idmap.o sam/idmap_util.o  sam/idmap_tdb.o

WINBINDD_OBJ1 = \
		nsswitch/winbindd.o       \
		nsswitch/winbindd_user.o  \
		nsswitch/winbindd_group.o \
		nsswitch/winbindd_util.o  \
		nsswitch/winbindd_cache.o \
		nsswitch/winbindd_pam.o   \
		nsswitch/winbindd_sid.o   \
		nsswitch/winbindd_misc.o  \
		nsswitch/winbindd_cm.o \
		nsswitch/winbindd_wins.o \
		nsswitch/winbindd_rpc.o \
		nsswitch/winbindd_reconnect.o \
		nsswitch/winbindd_ads.o \
		nsswitch/winbindd_passdb.o \
		nsswitch/winbindd_dual.o \
		nsswitch/winbindd_async.o \
		nsswitch/winbindd_creds.o \
		nsswitch/winbindd_cred_cache.o 

WINBINDD_OBJ = \
		$(WINBINDD_OBJ1) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
		$(PARAM_OBJ) $(LIB_NONSMBD_OBJ) \
		$(LIBSMB_OBJ) $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) \
		$(PROFILE_OBJ) $(SLCACHE_OBJ) $(SMBLDAP_OBJ) \
		$(SECRETS_OBJ) $(LIBADS_OBJ) $(KRBCLIENT_OBJ) $(POPT_LIB_OBJ) \
		$(DCUTIL_OBJ) $(IDMAP_OBJ) \
		$(AFS_OBJ) $(AFS_SETTOKEN_OBJ) \
		$(LIBADS_SERVER_OBJ) $(SERVER_MUTEX_OBJ)

WBINFO_OBJ = nsswitch/wbinfo.o $(LIBSAMBA_OBJ) $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) \
		$(SECRETS_OBJ) $(POPT_LIB_OBJ) $(AFS_SETTOKEN_OBJ) $(RPC_PARSE_OBJ1) $(DOSERR_OBJ)

WINBIND_NSS_OBJ = $(WBCOMMON_OBJ) lib/replace1.o nsswitch/winbind_nss_linux.o

WINBIND_NSS_PICOBJS = $(WINBIND_NSS_OBJ:.o=.po) lib/snprintf.po

POPT_OBJS=popt/findme.o popt/popt.o popt/poptconfig.o \
          popt/popthelp.o popt/poptparse.o

INIPARSER_OBJ = iniparser/src/iniparser.o iniparser/src/dictionary.o \
		iniparser/src/strlib.o

TDBBACKUP_OBJ = tdb/tdbbackup.o tdb/tdbback.o $(SNPRINTF_OBJ) $(TDBBASE_OBJ)

TDBTOOL_OBJ = tdb/tdbtool.o $(TDBBASE_OBJ) $(SNPRINTF_OBJ)

TDBDUMP_OBJ = tdb/tdbdump.o $(TDBBASE_OBJ) $(SNPRINTF_OBJ)

NTLM_AUTH_OBJ1 = utils/ntlm_auth.o utils/ntlm_auth_diagnostics.o 

NTLM_AUTH_OBJ = ${NTLM_AUTH_OBJ1} $(LIBSAMBA_OBJ) $(POPT_LIB_OBJ) \
		libsmb/asn1.o libsmb/spnego.o libsmb/clikrb5.o libads/kerberos.o \
		libads/kerberos_verify.o $(SECRETS_OBJ) $(SERVER_MUTEX_OBJ) \
		libads/authdata.o $(RPC_PARSE_OBJ1) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
		$(SMBLDAP_OBJ) $(DOSERR_OBJ) rpc_parse/parse_net.o $(LIBNMB_OBJ)

######################################################################
# now the rules...
######################################################################
all : SHOWFLAGS proto_exists $(SBIN_PROGS) $(BIN_PROGS) $(ROOT_SBIN_PROGS) \
	$(SHLIBS) $(MODULES) $(PAM_MODULES)  bin/smbsh$(EXEEXT) bin/smbwrapper.so

pam_modules : $(PAM_MODULES)

pam_smbpass : SHOWFLAGS proto_exists bin/pam_smbpass.so

pam_winbind : SHOWFLAGS proto_exists bin/pam_winbind.so

smbwrapper : SHOWFLAGS bin/smbsh$(EXEEXT) bin/smbwrapper.so

torture : SHOWFLAGS $(TORTURE_PROGS)

smbtorture : SHOWFLAGS bin/smbtorture

masktest : SHOWFLAGS bin/masktest

msgtest : SHOWFLAGS bin/msgtest

locktest : SHOWFLAGS bin/locktest

smbcacls : SHOWFLAGS bin/smbcacls

smbcquotas : SHOWFLAGS bin/smbcquotas

smbctool: SHOWFLAGS bin/smbctool

locktest2 : SHOWFLAGS bin/locktest2

rpctorture : SHOWFLAGS bin/rpctorture

debug2html : SHOWFLAGS bin/debug2html

smbfilter : SHOWFLAGS bin/smbfilter

eventlogadm: SHOWFLAGS bin/eventlogadm

talloctort : SHOWFLAGS bin/talloctort

timelimit : SHOWFLAGS bin/timelimit

nsswitch : SHOWFLAGS bin/winbindd bin/wbinfo nsswitch/libnss_winbind.so \
	nsswitch/libnss_wins.so bin/pam_winbind.so

wins : SHOWFLAGS nsswitch/libnss_wins.so

modules: SHOWFLAGS proto_exists $(MODULES)

cac: SHOWFLAGS $(LIBMSRPC)

everything: all libsmbclient debug2html smbfilter talloctort modules torture \
	$(EVERYTHING_PROGS)

.SUFFIXES:
.SUFFIXES: .c .o .po .lo

SHOWFLAGS:
	@echo "Using FLAGS = $(FLAGS)"
	@echo "      LIBS = $(LIBS)"
	@echo "      LDSHFLAGS = $(LDSHFLAGS)"
	@echo "      LDFLAGS = $(LDFLAGS)"
	@echo "      PIE_CFLAGS = -fPIE"
	@echo "      PIE_LDFLAGS = -pie"

MAKEDIR = || exec false; \
	  if test -d "$$dir"; then :; else \
	  echo mkdir "$$dir"; \
	  mkdir -p "$$dir" >/dev/null 2>&1 || \
	  test -d "$$dir" || \
	  mkdir "$$dir" || \
	  exec false; fi || exec false

.c.o:
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	 dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@if test -n "$(CC_CHECKER)"; then \
	  echo "Checking  $*.c with '$(CC_CHECKER)'";\
	  $(CC_CHECKER) -I. -I$(srcdir) $(FLAGS) -fPIE -c $< -o $@;\
	 fi
	@echo Compiling $*.c
	@$(CC) -I. -I$(srcdir) $(FLAGS) -fPIE -c $< -o $@ 
#	-mv `echo $@ | sed 's%^.*/%%g'` $@

# this adds support for precompiled headers. To use it, install a snapshot
# of gcc-3.4 and run 'make pch' before you do the main build. 
pch: proto_exists
	rm -f $(builddir)/include/includes.h.gch
	$(CC) -I. -I$(srcdir) $(FLAGS) -fPIE -c $(srcdir)/include/includes.h -o $(builddir)/include/includes.h.gch

# These dependencies are only approximately correct: we want to make
# sure Samba's paths are updated if ./configure is re-run.  Really it
# would be nice if "make prefix=/opt/samba all" also rebuilt things,
# but since we also require "make install prefix=/opt/samba" *not* to
# rebuild it's a bit hard.

dynconfig.o: dynconfig.c Makefile
	@echo Compiling $*.c
	@$(CC) $(FLAGS) $(PATH_FLAGS) -fPIE -c $(srcdir)/dynconfig.c -o $@ 

dynconfig.po: dynconfig.c Makefile
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIC -c $(srcdir)/dynconfig.c -o $@
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

lib/pidfile.o: lib/pidfile.c
	@echo Compiling $*.c
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIE -c $(srcdir)/lib/pidfile.c -o $@ 

lib/pidfile.po: lib/pidfile.c
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIC -c $(srcdir)/lib/pidfile.c -o $@
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

lib/version.o: lib/version.c include/version.h
	@echo Compiling $*.c
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIE -c $(srcdir)/lib/version.c -o $@ 

lib/version.po: lib/version.c include/version.h
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIC -c $(srcdir)/lib/version.c -o $@
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

smbd/build_options.o: smbd/build_options.c Makefile include/config.h include/build_env.h include/proto.h
	@echo Compiling $*.c
	@$(CC) $(FLAGS) $(PATH_FLAGS) -fPIE -c smbd/build_options.c -o $@

smbd/build_options.c: include/config.h.in script/mkbuildoptions.awk
	@echo Generating $@
	@dir=smbd $(MAKEDIR) && $(AWK) -f $(srcdir)/script/mkbuildoptions.awk > $(builddir)/smbd/build_options.c < $(srcdir)/include/config.h.in

.c.po:
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@if test -n "$(CC_CHECKER)"; then \
	  echo "Checking  $*.c with '$(CC_CHECKER)' and -fPIC";\
	  $(CC_CHECKER) -I. -I$(srcdir) $(FLAGS) -fPIC -c $< -o $*.po;\
	 fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) -fPIC -c $< -o $*.po
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

bin/.dummy:
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then :; else \
	  dir=bin $(MAKEDIR); fi
	@: >> $@ || : > $@ # what a fancy emoticon!

bin/smbd: $(SMBD_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBD_OBJ) $(LDFLAGS) $(LDAP_LIBS) \
		$(KRB5LIBS) $(DYNEXP) $(PRINT_LIBS) $(AUTH_LIBS) \
		$(ACL_LIBS) $(PASSDB_LIBS) $(LIBS) $(POPT_OBJS)   

bin/nmbd: $(NMBD_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(NMBD_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/swat: $(SWAT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SWAT_OBJ) $(LDFLAGS) $(DYNEXP) $(PRINT_LIBS) \
	  $(AUTH_LIBS) $(LIBS) $(PASSDB_LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/rpcclient: $(RPCCLIENT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(PASSDB_LIBS) $(RPCCLIENT_OBJ) \
		$(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) \
		$(KRB5LIBS) $(LDAP_LIBS)

bin/smbclient: $(CLIENT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(CLIENT_OBJ) $(LDFLAGS) $(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbctool: $(TOOL_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(TOOL_OBJ) $(LDFLAGS) $(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) -Lbin -lsmbclient

bin/net: $(NET_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(NET_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(PASSDB_LIBS) $(TERMLDFLAGS) $(TERMLIBS)

bin/profiles: $(PROFILES_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(PROFILES_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS)

bin/smbspool: $(CUPS_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(CUPS_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbmount: $(MOUNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(MOUNT_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbmnt: $(MNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(MNT_OBJ) $(DYNEXP) $(LDFLAGS) 

bin/smbumount: $(UMOUNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(UMOUNT_OBJ) $(DYNEXP) $(LDFLAGS)

bin/mount.cifs: $(CIFS_MOUNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(CIFS_MOUNT_OBJ) $(DYNEXP) $(LDFLAGS)

bin/umount.cifs: $(CIFS_UMOUNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(CIFS_UMOUNT_OBJ) $(DYNEXP) $(LDFLAGS)

bin/testparm: $(TESTPARM_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(TESTPARM_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/smbstatus: $(STATUS_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(STATUS_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) \
	$(POPT_OBJS)

bin/smbcontrol: $(SMBCONTROL_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) -DUSING_SMBCONTROL $(FLAGS) -pie -o $@ \
	$(SMBCONTROL_OBJ) $(DYNEXP) $(LDFLAGS) \
	$(LIBS)  $(POPT_OBJS)

bin/smbtree: $(SMBTREE_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBTREE_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

#bin/smbpasswd: $(SMBPASSWD_OBJ) $(POPT_OBJS) bin/.dummy
#	@echo Linking $@
#	@$(CC) $(FLAGS) -pie -o $@ $(SMBPASSWD_OBJ) $(LDFLAGS) $(PASSDB_LIBS) \
#		$(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

MY_PASS_OBJ = libsmb/smbdes.o lib/md4.o lib/arc4.o lib/snprintf.o

bin/smbpasswd: utils/owrt_smbpasswd.o $(MY_PASS_OBJ)
	@echo Linking $@
	@$(CC) $(FLAGS)  -o $@ utils/owrt_smbpasswd.o $(MY_PASS_OBJ)

bin/pdbedit: $(PDBEDIT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(PDBEDIT_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(PASSDB_LIBS) $(LDAP_LIBS)

bin/smbget: $(SMBGET_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBGET_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS)  $(KRB5LIBS) $(LDAP_LIBS) $(NSCD_LIBS)

bin/samtest: $(SAMTEST_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SAMTEST_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(PASSDB_LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/nmblookup: $(NMBLOOKUP_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(NMBLOOKUP_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(LDAP_LIBS)

bin/smbtorture: $(SMBTORTURE_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBTORTURE_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/talloctort: $(TALLOCTORT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(TALLOCTORT_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS)

bin/masktest: $(MASKTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(MASKTEST_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/msgtest: $(MSGTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(MSGTEST_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbcacls: $(SMBCACLS_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBCACLS_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) \
	$(KRB5LIBS) $(LDAP_LIBS) $(NSCD_LIBS)

bin/smbcquotas: $(SMBCQUOTAS_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBCQUOTAS_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) \
	$(KRB5LIBS) $(LDAP_LIBS) $(NSCD_LIBS)

bin/eventlogadm: $(EVTLOGADM_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(EVTLOGADM_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS)

bin/locktest: $(LOCKTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LOCKTEST_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/nsstest: $(NSSTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(NSSTEST_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS)  $(LDAP_LIBS)

bin/vfstest: $(VFSTEST_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(VFSTEST_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(PRINT_LIBS) $(AUTH_LIBS) $(ACL_LIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)   $(NSCD_LIBS)

bin/smbiconv: $(SMBICONV_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBICONV_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/log2pcap: $(LOG2PCAP_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LOG2PCAP_OBJ) $(LDFLAGS) $(DYNEXP) $(POPT_OBJS) $(LIBS)

bin/locktest2: $(LOCKTEST2_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LOCKTEST2_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/rpctorture: $(RPCTORTURE_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(RPCTORTURE_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/debug2html: $(DEBUG2HTML_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(DEBUG2HTML_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS)

bin/smbfilter: $(SMBFILTER_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBFILTER_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbw_sample: $(SMBW_OBJ) utils/smbw_sample.o bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBW_OBJ) utils/smbw_sample.o $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbsh: $(SMBSH_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(SMBSH_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS)

bin/smbwrapper.so: $(PICOBJS) bin/.dummy
	@echo Linking shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PICOBJS) $(LIBS) \
		$(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`

bin/libsmbclient.so: $(LIBSMBCLIENT_PICOBJS)
	@echo Linking libsmbclient shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(LIBSMBCLIENT_PICOBJS) $(LDFLAGS) $(LIBS) \
		$(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`.$(LIBSMBCLIENT_MAJOR)

bin/libsmbclient.a: $(LIBSMBCLIENT_PICOBJS)
	@echo Linking libsmbclient non-shared library $@
	@-$(AR) -rc $@ $(LIBSMBCLIENT_PICOBJS)

bin/libsmbsharemodes.so: $(LIBSMBSHAREMODES_PICOBJS)
	@echo Linking libsmbsharemodes shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(LIBSMBSHAREMODES_PICOBJS) $(LDFLAGS) $(LIBS) \
		$(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`.$(LIBSMBSHAREMODES_MAJOR)

bin/libsmbsharemodes.a: $(LIBSMBSHAREMODES_PICOBJS)
	@echo Linking libsmbsharemodes non-shared library $@
	@-$(AR) -rc $@ $(LIBSMBSHAREMODES_PICOBJS)

bin/libmsrpc.so: $(CAC_PICOBJS)
	@echo Linking libmsrpc shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CAC_PICOBJS) $(LDFLAGS) $(LIBS) \
	-Wl,-soname=`basename $@`.$(LIBMSRPC_MAJOR)

bin/libmsrpc.a: $(CAC_PICOBJS)
	@echo Linking libmsrpc non-shared library $@
	@-$(AR) -rc $@ $(CAC_PICOBJS)

# This is probably wrong for anything other than the GNU linker. 
bin/libbigballofmud.so: $(LIBBIGBALLOFMUD_PICOBJS)
	@echo Linking bigballofmud shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(LIBBIGBALLOFMUD_PICOBJS) $(LIBS) \
		$(PASSDB_LIBS) $(IDMAP_LIBS) $(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`.$(LIBBIGBALLOFMUD_MAJOR)
	ln -snf libbigballofmud.so bin/libbigballofmud.so.0

# It would be nice to build a static bigballofmud too, but when I try
# I get linker errors about dl_open and similar things.  I'm not sure if
# it can be fixed or if they just can't be called from a static
# library.

libsmbclient: $(LIBSMBCLIENT)
libsmbsharemodes: $(LIBSMBSHAREMODES)
libmsrpc: $(LIBMSRPC)

bin/librpc_lsarpc.so: $(RPC_LSA_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_LSA_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_samr.so: $(RPC_SAMR_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SAMR_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_srvsvc.so: $(RPC_SVC_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SVC_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_svcctl.so: $(RPC_SVCCTL_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SVCCTL_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_ntsvcs.so: $(RPC_NTSVCS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_NTSVCS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_wkssvc.so: $(RPC_WKS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_WKS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_NETLOGON.so: $(RPC_NETLOG_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_NETLOG_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_winreg.so: $(RPC_REG_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_REG_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_lsa_ds.so: $(RPC_LSA_DS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_LSA_DS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_spoolss.so: $(RPC_SPOOLSS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SPOOLSS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_eventlog.so: $(RPC_EVENTLOG_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_EVENTLOG_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_netdfs.so: $(RPC_DFS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_DFS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_echo.so: $(RPC_ECHO_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_ECHO_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/winbindd: $(WINBINDD_OBJ) $(POPT_OBJS) bin/.dummy
	@echo "Linking $@"
	@$(CC) $(FLAGS) -pie -o $@ $(WINBINDD_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) \
		$(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(PASSDB_LIBS)

# Please don't add .o files to libnss_winbind, libnss_wins, or the pam_winbind
# libraries.  Add to the appropriate PICOBJ variable instead.

nsswitch/libnss_winbind.so: $(WINBIND_NSS_PICOBJS)
	@echo "Linking $@"
	@$(SHLD) $(WINBIND_NSS_LDSHFLAGS) -o $@ $(WINBIND_NSS_PICOBJS) \
		 -Wl,-soname=`basename $@`.2

nsswitch/libnss_wins.so: $(WINBIND_WINS_NSS_PICOBJS)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(WINBIND_WINS_NSS_PICOBJS) \
		$(LDAP_LIBS) $(KRB5LIBS) \
		-Wl,-soname=`basename $@`.2

bin/pam_winbind.so: $(PAM_WINBIND_OBJ:.o=.po) bin/.dummy
	@echo "Linking shared library $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PAM_WINBIND_OBJ:.o=.po) \
		-Wl,-soname=`basename $@` -lpam 

bin/builtin.so: $(AUTH_BUILTIN_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_BUILTIN_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/domain.so: $(AUTH_DOMAIN_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_DOMAIN_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/script.so: $(AUTH_SCRIPT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_SCRIPT_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/smbserver.so: $(AUTH_SERVER_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_SERVER_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/winbind.so: $(AUTH_WINBIND_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_WINBIND_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/unix.so: $(AUTH_UNIX_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_UNIX_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/sam.so: $(AUTH_SAM_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_SAM_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/ldapsam.so: passdb/pdb_ldap.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) $(LDAP_LIBS) -o $@ passdb/pdb_ldap.po \
		-Wl,-soname=`basename $@`

bin/tdbsam.so: passdb/pdb_tdb.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ passdb/pdb_tdb.po \
		-Wl,-soname=`basename $@`

bin/smbpasswd.so: passdb/pdb_smbpasswd.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ passdb/pdb_smbpasswd.po \
		-Wl,-soname=`basename $@`

bin/rid.so: sam/idmap_rid.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ sam/idmap_rid.po \
		-Wl,-soname=`basename $@`

bin/ad.so: sam/idmap_ad.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ sam/idmap_ad.po \
		-Wl,-soname=`basename $@`

bin/weird.so: $(DEVEL_HELP_WEIRD_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(DEVEL_HELP_WEIRD_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/CP850.so: $(CP850_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CP850_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/CP437.so: $(CP437_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CP437_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/macosxfs.so: $(CHARSET_MACOSXFS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CHARSET_MACOSXFS_OBJ:.o=.po) \
		-framework CoreFoundation -Wl,-soname=`basename $@`

bin/audit.so: $(VFS_AUDIT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_AUDIT_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/extd_audit.so: $(VFS_EXTD_AUDIT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_EXTD_AUDIT_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/full_audit.so: $(VFS_FULL_AUDIT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_FULL_AUDIT_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/recycle.so: $(VFS_RECYCLE_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_RECYCLE_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/netatalk.so: $(VFS_NETATALK_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_NETATALK_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/fake_perms.so: $(VFS_FAKE_PERMS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_FAKE_PERMS_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/default_quota.so: $(VFS_DEFAULT_QUOTA_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_DEFAULT_QUOTA_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/readonly.so: $(VFS_READONLY_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_READONLY_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/shadow_copy.so: $(VFS_SHADOW_COPY_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_SHADOW_COPY_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/cap.so: $(VFS_CAP_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_CAP_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/expand_msdfs.so: $(VFS_EXPAND_MSDFS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_EXPAND_MSDFS_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/afsacl.so: $(VFS_AFSACL_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_AFSACL_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/catia.so: $(VFS_CATIA_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_CATIA_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`


bin/wbinfo: $(WBINFO_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(WBINFO_OBJ) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/ntlm_auth: $(NTLM_AUTH_OBJ) $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) \
		$(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(DYNEXP) $(NTLM_AUTH_OBJ) \
		$(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(LIBS) \
		$(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(NSCD_LIBS)

bin/pam_smbpass.so: $(PAM_SMBPASS_PICOOBJ)
	@echo "Linking shared library $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PAM_SMBPASS_PICOOBJ) -lpam $(DYNEXP) $(LIBS) $(LDAP_LIBS) $(KRB5LIBS)

bin/tdbbackup: $(TDBBACKUP_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(DYNEXP) $(LIBS) $(TDBBACKUP_OBJ) 

bin/tdbtool: $(TDBTOOL_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(DYNEXP) $(LIBS) $(TDBTOOL_OBJ) 

bin/tdbdump: $(TDBDUMP_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(LDFLAGS) $(DYNEXP) $(LIBS) $(TDBDUMP_OBJ) 

bin/t_strcmp: bin/libbigballofmud.so torture/t_strcmp.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) $(LIBS) torture/t_strcmp.o -L ./bin -lbigballofmud

bin/t_strstr: bin/libbigballofmud.so torture/t_strstr.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) $(LIBS) torture/t_strstr.o -L ./bin -lbigballofmud

bin/t_strappend: bin/libbigballofmud.so torture/t_strappend.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) $(LIBS) torture/t_strappend.o -L ./bin -lbigballofmud

bin/t_stringoverflow: bin/libbigballofmud.so torture/t_stringoverflow.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) torture/t_stringoverflow.o -L./bin -lbigballofmud

bin/t_doschar: bin/libbigballofmud.so torture/t_doschar.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) $(LIBS) torture/t_doschar.o -L ./bin -lbigballofmud
bin/t_push_ucs2: bin/libbigballofmud.so torture/t_push_ucs2.o
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) $(LIBS) torture/t_push_ucs2.o -L ./bin -lbigballofmud

bin/t_snprintf: lib/snprintf.c
	$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) -DTEST_SNPRINTF lib/snprintf.c -lm

bin/timelimit: script/tests/timelimit.o
	@echo Linking $@
	@$(CC) $(FLAGS) -pie -o $@ $(DYNEXP) script/tests/timelimit.o

install: installservers installbin installcifsmount installman installscripts installdat installswat installmodules installclientlib installlibmsrpc 

install-everything: install installmodules

# DESTDIR is used here to prevent packagers wasting their time
# duplicating the Makefile. Remove it and you will have the privelege
# of package each samba release for muliple versions of multiple
# distributions and operating systems, or at least supplying patches
# to all the packaging files required for this, prior to committing
# the removal of DESTDIR. Do not remove it even though you think it
# is not used

installdirs:
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(BINDIR) $(SBINDIR) $(LIBDIR) $(VARDIR) $(PRIVATEDIR) $(PIDDIR) $(LOCKDIR) $(MANDIR)

installservers: all installdirs
	@$(SHELL) script/installbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(SBINDIR) $(SBIN_PROGS)

installbin: all installdirs
	@$(SHELL) script/installbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(BINDIR) $(BIN_PROGS)

installcifsmount: bin/mount.cifs bin/umount.cifs
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) $(ROOTSBINDIR)
	@$(SHELL) script/installbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(ROOTSBINDIR) bin/mount.cifs bin/umount.cifs

# Some symlinks are required for the 'probing' of modules. 
# This mechanism should go at some point..
installmodules: modules installdirs
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(VFSLIBDIR) $(VFS_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(PDBLIBDIR) $(PDB_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(RPCLIBDIR) $(RPC_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(IDMAPLIBDIR) $(IDMAP_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(CHARSETLIBDIR) $(CHARSET_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(AUTHLIBDIR) $(AUTH_MODULES)
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(PDBLIBDIR) ldapsam.so ldapsam_compat.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) rhosts.so hostsequiv.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) sam.so sam_ignoredomain.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) domain.so trustdomain.so ntdomain.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) builtin.so guest.so fixed_challenge.so name_to_ntstatus.so

installscripts: installdirs
	@$(SHELL) $(srcdir)/script/installscripts.sh $(INSTALLPERMS) $(DESTDIR)$(BINDIR) $(SCRIPTS)

installdat: installdirs
	@$(SHELL) $(srcdir)/script/installdat.sh $(DESTDIR) $(LIBDIR) $(srcdir)

installmsg: installdirs
	@$(SHELL) $(srcdir)/script/installmsg.sh $(DESTDIR) $(LIBDIR) $(srcdir)

installswat: installdirs installmsg
	@$(SHELL) $(srcdir)/script/installswat.sh $(DESTDIR) $(SWATDIR) $(srcdir)

installclientlib: installdirs libsmbclient
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) $(LIBDIR)
	-$(INSTALLLIBCMD_SH) bin/libsmbclient.so $(DESTDIR)$(LIBDIR)
	-$(INSTALLLIBCMD_A) bin/libsmbclient.a $(DESTDIR)$(LIBDIR)
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) ${prefix}/include
	-$(INSTALLCMD) $(srcdir)/include/libsmbclient.h $(DESTDIR)${prefix}/include

installlibmsrpc: installdirs libmsrpc
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) $(LIBDIR)
	-$(INSTALLLIBCMD_SH) bin/libmsrpc.so $(DESTDIR)$(LIBDIR)
	-$(INSTALLLIBCMD_A) bin/libmsrpc.a $(DESTDIR)$(LIBDIR)
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) ${prefix}/include
	-$(INSTALLCMD) $(srcdir)/include/libmsrpc.h $(DESTDIR)${prefix}/include

installpammodules: $(PAM_MODULES)
	@$(SHELL) $(srcdir)/script/installdirs.sh $(INSTALLPERMS) $(DESTDIR) $(PAMMODULESDIR)
	@for module in $(PAM_MODULES); do \
		echo "Installing module $${module} as $(DESTDIR)/$(PAMMODULESDIR)/$${module}.so "; \
		$(INSTALLCMD) -m $(INSTALLPERMS) bin/$${module}.so \
			"$(DESTDIR)/$(PAMMODULESDIR)"; \
	done

# Python extensions

PYTHON_OBJS = $(PARAM_OBJ) $(LIB_NONSMBD_OBJ) $(LIBSMB_OBJ) $(RPC_PARSE_OBJ) \
	$(LIBMSRPC_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
	$(SECRETS_OBJ) $(KRBCLIENT_OBJ) $(SMBLDAP_OBJ)

PYTHON_PICOBJS = $(PYTHON_OBJS:.o=.po)

python_ext: $(PYTHON_PICOBJS)
	@if test -z "$(PYTHON)"; then \
		echo Use the option --with-python to configure python; \
		exit 1; \
	fi
	PYTHON_OBJS="$(PYTHON_PICOBJS)" \
	PYTHON_CFLAGS="$(CFLAGS) $(CPPFLAGS) $(FLAGS)" \
	LIBS="$(LDFLAGS) $(LIBS) $(PASSDB_LIBS) $(IDMAP_LIBS) $(KRB5LIBS) $(LDAP_LIBS)" \
		$(PYTHON) python/setup.py build

python_install: $(PYTHON_PICOBJS)
	@if test -z "$(PYTHON)"; then \
		echo Use the option --with-python to configure python; \
		exit 1; \
	fi
	PYTHON_OBJS="$(PYTHON_PICOBJS)" \
	PYTHON_CFLAGS="$(CFLAGS) $(CPPFLAGS)" \
	LIBS="$(LDFLAGS) $(LIBS)" \
		$(PYTHON) python/setup.py install --root=$(DESTDIR)

python_clean:
	@-if test -n "$(PYTHON)"; then $(PYTHON) python/setup.py clean; fi

# revert to the previously installed version
revert:
	@$(SHELL) $(srcdir)/script/revert.sh $(SBINDIR) $(SBIN_PROGS) 
	@$(SHELL) $(srcdir)/script/revert.sh $(BINDIR) $(BIN_PROGS) $(SCRIPTS)

installman: installdirs
	@$(SHELL) $(srcdir)/script/installman.sh $(DESTDIR)$(MANDIR) $(srcdir) C ""

.PHONY: showlayout

showlayout:
	@echo "Samba will be installed into:"
	@echo "  prefix:      $(prefix)"
	@echo "  bindir:      $(BINDIR)"
	@echo "  sbindir:     $(SBINDIR)"
	@echo "  libdir:      $(LIBDIR)"
	@echo "  vardir:      $(VARDIR)"
	@echo "  mandir:      $(MANDIR)"
	@echo "  privatedir:  $(PRIVATE_DIR)"
	@echo "  configdir:   $(CONFIGDIR)"
	@echo "  lockdir:     $(LOCKDIR)"
	@echo "  piddir:      $(PIDDIR)"
	@echo "  swatdir:     $(SWATDIR)"


uninstall: uninstallman uninstallservers uninstallbin uninstallcifsmount uninstallscripts uninstalldat uninstallswat uninstallmodules uninstallclientlib uninstalllibmsrpc 

uninstallman:
	@$(SHELL) $(srcdir)/script/uninstallman.sh $(DESTDIR)$(MANDIR) $(srcdir) C

uninstallservers:
	@$(SHELL) script/uninstallbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(SBINDIR) $(SBIN_PROGS)

uninstallbin:
	@$(SHELL) script/uninstallbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(BINDIR) $(BIN_PROGS)

uninstallcifsmount:
	@$(SHELL) script/uninstallbin.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(ROOTSBINDIR) bin/mount.cifs bin/umount.cifs

uninstallmodules:
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(VFSLIBDIR) $(VFS_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(PDBLIBDIR) $(PDB_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(RPCLIBDIR) $(RPC_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(IDMAPLIBDIR) $(IDMAP_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(CHARSETLIBDIR) $(CHARSET_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR) $(prefix) $(AUTHLIBDIR) $(AUTH_MODULES)

uninstallscripts:
	@$(SHELL) $(srcdir)/script/uninstallscripts.sh $(INSTALLPERMS) $(DESTDIR)$(BINDIR) $(SCRIPTS)

uninstalldat:
	@$(SHELL) $(srcdir)/script/uninstalldat.sh $(DESTDIR) $(LIBDIR) $(srcdir)

uninstallmsg:
	@$(SHELL) $(srcdir)/script/uninstallmsg.sh $(DESTDIR) $(LIBDIR) $(srcdir)

uninstallswat: uninstallmsg
	@$(SHELL) $(srcdir)/script/uninstallswat.sh $(DESTDIR) $(SWATDIR) $(srcdir)

uninstallclientlib: installdirs libsmbclient
	-$(UNINSTALLLIBCMD_SH) $(DESTDIR)$(LIBDIR)/libsmbclient.so
	-$(UNINSTALLLIBCMD_A) $(DESTDIR)$(LIBDIR)/libsmbclient.a
	-rm -f $(DESTDIR)${prefix}/include/libsmbclient.h

uninstalllibmsrpc: installdirs libmsrpc
	-$(UNINSTALLLIBCMD_SH) $(DESTDIR)$(LIBDIR)/libmsrpc.so
	-$(UNINSTALLLIBCMD_A) $(DESTDIR)$(LIBDIR)/libmsrpc.a
	-rm -f $(DESTDIR)${prefix}/include/libmsrpc.h

uninstallpammodules:
	@for module in $(PAM_MODULES); do \
		echo "Removing $(DESTDIR)/$(PAMMODULESDIR)/$${module}.so "; \
		rm -f "$(DESTDIR)/$(PAMMODULESDIR)/$${module}.so"; \
	done

# Toplevel clean files
TOPFILES=dynconfig.o dynconfig.po

clean: delheaders python_clean
	-rm -f core */*~ *~ */*.o */*.po */*.so \
		$(TOPFILES) $(BIN_PROGS) $(SBIN_PROGS) $(ROOT_SBIN_PROGS) \
		$(MODULES) $(TORTURE_PROGS) $(LIBSMBCLIENT) \
		$(LIBSMBSHAREMODES) $(EVERYTHING_PROGS) $(LIBMSRPC) \
		.headers.stamp */src/*.o */src/*.po
	-rm -rf t_dir

# Making this target will just make sure that the prototype files
# exist, not necessarily that they are up to date.  Since they're
# removed by "make clean" this will always be run when you do anything
# afterwards.
proto_exists: include/proto.h include/build_env.h \
	nsswitch/winbindd_proto.h web/swat_proto.h \
	client/client_proto.h utils/net_proto.h utils/ntlm_auth_proto.h smbd/build_options.c

delheaders:
	@echo Removing prototype headers
	@rm -f include/proto.h include/build_env.h  \
		nsswitch/winbindd_proto.h web/swat_proto.h \
		client/client_proto.h utils/net_proto.h \
		smbd/build_options.c utils/ntlm_auth_proto.h \
		utils/passwd_proto.h

MKPROTO_SH = $(srcdir)/script/mkproto.sh

include/proto.h: smbd/build_options.c
	@echo Building include/proto.h
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _PROTO_H_ $(builddir)/include/proto.h \
	  $(PROTO_OBJ)

include/build_env.h: script/build_env.sh
	@echo Building include/build_env.h
	@$(SHELL) $(srcdir)/script/build_env.sh $(srcdir) $(builddir) $(CC) \
	  > $(builddir)/include/build_env.h

nsswitch/winbindd_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _WINBINDD_PROTO_H_ $(builddir)/nsswitch/winbindd_proto.h \
	  $(WINBINDD_OBJ1)

web/swat_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _SWAT_PROTO_H_ $(builddir)/web/swat_proto.h \
	  $(SWAT_OBJ1)

client/client_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _CLIENT_PROTO_H_ $(builddir)/client/client_proto.h \
	  $(CLIENT_OBJ1)

utils/net_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _NET_PROTO_H_ $(builddir)/utils/net_proto.h \
	  $(NET_OBJ1)

utils/passwd_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _PASSWD_PROTO_H_ $(builddir)/utils/passwd_proto.h \
	  utils/passwd_util.o

utils/ntlm_auth_proto.h:
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _NTLM_AUTH_PROTO_H_ $(builddir)/utils/ntlm_auth_proto.h \
	  $(NTLM_AUTH_OBJ1)

# "make headers" or "make proto" calls a subshell because we need to
# make sure these commands are executed in sequence even for a
# parallel make.
headers:
	$(MAKE) delheaders; \
	$(MAKE) smbd/build_options.c; \
	$(MAKE) include/proto.h; \
	$(MAKE) include/build_env.h; \
	$(MAKE) nsswitch/winbindd_proto.h; \
	$(MAKE) web/swat_proto.h; \
	$(MAKE) client/client_proto.h; \
	$(MAKE) utils/ntlm_auth_proto.h; \
	$(MAKE) utils/net_proto.h; \
	$(MAKE) utils/passwd_proto.h;

proto: headers 

.PHONY: headers proto

etags:
	etags `find $(srcdir) -name "*.[ch]" | grep -v /CVS/`

ctags:
	ctags `find $(srcdir) -name "*.[ch]" | grep -v /CVS/`

realclean: clean delheaders
	-rm -f config.log bin/.dummy script/findsmb script/gen-8bit-gap.sh
	-rm -f script/installbin.sh script/uninstallbin.sh

distclean: realclean
	-rm -f include/stamp-h
	-rm -f include/config.h Makefile
	-rm -f config.status config.cache so_locations
	-rm -rf .deps TAGS

realdistclean: distclean
	-rm -f include/config.h.in
	-rm -f include/version.h
	-rm -f configure

# this target is really just for my use. It only works on a limited
# range of machines and is used to produce a list of potentially
# dead (ie. unused) functions in the code. (tridge)
finddead:
	nm */*.o |grep 'U ' | awk '{print $$2}' | sort -u > nmused.txt
	nm */*.o |grep 'T ' | awk '{print $$3}' | sort -u > nmfns.txt
	comm -13 nmused.txt nmfns.txt 


# when configure.in is updated, reconfigure
$(srcdir)/configure: $(srcdir)/configure.in
	@echo "WARNING: you need to rerun ./autogen.sh"

config.status: $(srcdir)/configure
	@echo "WARNING: you need to run ./configure"

Makefile: $(srcdir)/Makefile.in config.status
	@echo "WARNING: you need to run ./config.status"

######################################################################
# Samba Testing Framework

# Check for PAM module problems.  Specifically, check that every module we
# built can actually be loaded by a minimal PAM-aware application.
test_pam_modules: pam_modules
	@echo "Testing $(PAM_MODULES) "
	@for module in $(PAM_MODULES); do \
		./script/tests/dlopen.sh -lpam -ldl bin/$${module}.so \
			|| exit 1; \
	done

##
## Targets for 'make test'
##
test: all torture timelimit
	@echo Running Test suite
	@sh ./script/tests/selftest.sh ${selftest_prefix}/st all "${smbtorture4_path}"

valgrindtest: all torture timelimit
	@echo Running Test suite with valgrind
	@NMBD_VALGRIND="xterm -n smbd -e valgrind -q --db-attach=yes --num-callers=30" \
	 SMBD_VALGRIND="xterm -n smbd -e valgrind -q --db-attach=yes --num-callers=30" \
	 VALGRIND="valgrind -q --num-callers=30 --log-file=${selftest_prefix}/st/valgrind.log" \
	 ./script/tests/selftest.sh ${selftest_prefix}/st all "${smbtorture4_path}"
