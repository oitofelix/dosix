/*
  dos.c -- DOS interface routines

  Copyright (C) 2020 Bruno Félix Rezende Ribeiro <oitofelix@gnu.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _DOSIX_LIBC_SRC
#define _GNU_SOURCE


/* headers */

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <search.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dos.h>
#include <share.h>
#include <conio.h>
#include <dosix/stdlib.h>
#include <dosix/fcntl.h>


/* DOS interrupt services enumeration */

/* Interrupts */

#define INT21_MAIN_DOS_API 0x21
#define INT2F_MULTIPLEX 0x2f

/* Sub-functions */

#define INT21_AH_GETCHE 0x01
#define INT21_AH_PUTCH 0x02
#define INT21_AH_GETCH 0x08
#define INT21_AH_WRITE_STDOUT 0x09
#define INT21_AH_SET_DTA_ADDR 0x1a
#define INT21_AH_SETVECT 0x25
#define INT21_AH_GETDATE 0x2a
#define INT21_AH_SETDATE 0x2b
#define INT21_AH_GETTIME 0x2c
#define INT21_AH_SETTIME 0x2d
#define INT21_AH_GET_DTA_ADDR 0x2f
#define INT21_AH_GETVECT 0x35
#define INT21_AH_CREAT 0x3c
#define INT21_AH_OPEN 0x3d
#define INT21_AH_CLOSE 0x3e
#define INT21_AH_ALLOCMEM 0x48
#define INT21_AH_FREEMEM 0x49
#define INT21_AH_SETBLOCK 0x4a
#define INT21_AH_FINDFIRST 0x4e
#define INT21_AH_FINDNEXT 0x4f
#define INT21_AH_EXTERR 0x59
#define INT21_AH_CREATNEW 0x5b
#define INT21_BH_EXTERR 0x00
#define INT21_BL_EXTERR 0x00
#define INT21_AH_FILE_METADATA 0x43
#define INT21_AL_FILE_METADATA_GETFILEATTR 0x00
#define INT21_AL_FILE_METADATA_SETFILEATTR 0x01
#define INT21_AH_FILE_TIME 0x57
#define INT21_AL_FILE_TIME_GETFTIME 0x00
#define INT21_AL_FILE_TIME_SETFTIME 0x01
#define INT2F_AH_DOS_INTERNAL 0x12
#define INT2F_AL_DOS_INTERNAL_EXTERR_SET 0x22


/* type definitions */

struct allocmem {
  void *address;
  size_t length;
};

struct media_id /* media ID structure */
{
  char volume_label[12]; /* ASCIZ volume label of required disk */
  unsigned serial_number; /* serial number (DOS 4.0+) */
};

union dta_t {
  struct _find_t find_t;	/* Used by findfirst and findnext */
};


/* forward declarations */

static
void
_dosk_int21_main_dos_api
(cpu_t *);

static
void
_dosk_int2f_multiplex
(cpu_t *);


/* global public variables */

/* Holds extended error code for libc usage.  Set by
   __doskexterr_set */
int __near _doserrno;


/* global private variables */

static void *_dosk__allocmem_tree;
static struct _DOSERROR errorinfo;
static struct media_id media_id;
static union dta_t _dosk_dta;
static union dta_t *_dosk_current_dta = &_dosk_dta;
static syscall_t _dosk_vect[UINT8_MAX] =
  {
   [INT21_MAIN_DOS_API] = _dosk_int21_main_dos_api,
   [INT2F_MULTIPLEX] = _dosk_int2f_multiplex
  };


/* error codes */

/*** DOS extended error code ***/
/* DOS 2.0+ */
#define EXTERR_NO_ERROR 0x00 /* no error */
#define EXTERR_FN_NUM_INVAL 0x01 /* function number invalid */
#define EXTERR_FILE_NOT_FOUND 0x02 /* file not found */
#define EXTERR_PATH_NOT_FOUND 0x03 /* path not found */
#define EXTERR_TOO_MANY_OPEN_FILES 0x04 /* too many open files
					   (no handles available) */
#define EXTERR_ACCESS_DENIED 0x05 /* access denied */
#define EXTERR_INVAL_HANDLE 0x06 /* invalid handle */
#define EXTERR_MCB_DESTROYED 0x07 /* memory control block destroyed */
#define EXTERR_INSUF_MEM 0x08 /* insufficient memory */
#define EXTERR_MBA_INVAL 0x09 /* memory block address invalid */
#define EXTERR_ENV_INVAL 0x0a /* environment invalid (usually >32K in length) */
#define EXTERR_FMT_INVAL 0x0b /* format invalid */
#define EXTERR_ACCESS_CODE_INVAL 0x0c /* access code invalid */
#define EXTERR_DATA_INVAL 0x0d /* data invalid */
#define EXTERR_RESERVED_0x0e 0x0e /* 0x0e reserved;  */
#define EXTERR_FIXUP_OVERFLOW 0x0e /* (PTS-DOS 6.51+, S/DOS 1.0+) fixup overflow */
#define EXTERR_INVAL_DRV_0x0f 0x0f /* invalid drive */
#define EXTERR_ATTEMPT_RD_CWD 0x10 /* attempted to remove current directory */
#define EXTERR_NOT_SAME_DEVICE 0x11 /* not same device */
#define EXTERR_NO_MORE_FILES 0x12 /* no more files */
/* Checking for extended error codes */
#define EXTERR_NONEXTERR_LIMIT 0x12
/* DOS 3.0+ (INT 24 errors) */
#define EXTERR_DSK_WRITE_PROTECTED 0x13 /* disk write-protected */
#define EXTERR_UNKNOWN_UNIT 0x14 /* unknown unit */
#define EXTERR_DRV_NOT_READY 0x15 /* drive not ready */
#define EXTERR_UNKNOWN_CMD 0x16 /* unknown command */
#define EXTERR_DATA_ERROR 0x17 /* data error (CRC) */
#define EXTERR_BAD_REQ_STRUCT_LEN 0x18 /* bad request structure length */
#define EXTERR_SEEK_ERROR 0x19 /* seek error */
#define EXTERR_UNKNOWN_MEDIA_TYPE 0x1a /* unknown media type (non-DOS disk) */
#define EXTERR_SECTOR_NOT_FOUND 0x1b /* sector not found */
#define EXTERR_PRT_OUT_OF_PAPER 0x1c /* printer out of paper */
#define EXTERR_WRITE_FAULT 0x1d /* write fault */
#define EXTERR_READ_FAULT 0x1e /* read fault */
#define EXTERR_GEN_FAIL 0x1f /* general failure */
#define EXTERR_SHARING_VIOLATION 0x20 /* sharing violation */
#define EXTERR_LOCK_VIOLATION 0x21 /* lock violation */
#define EXTERR_DSK_CHANGE_INVAL 0x22 /* disk change invalid (ES:DI -> media ID
					structure)(see #01681) */
#define EXTERR_FCB_UNAVAILABLE 0x23 /* FCB unavailable */
#define EXTERR_BAD_FAT 0x23 /* (PTS-DOS 6.51+, S/DOS 1.0+) bad FAT */
#define EXTERR_SHARING_BUFFER_OVERFLOW 0x24 /* sharing buffer overflow */
/* DOS 4.0+ */
#define EXTERR_CODE_PAGE_MISMATCH 0x25 /* code page mismatch */
#define EXTERR_CANT_COMPLETE_FILE_OP 0x26 /* cannot complete file operation
					     (EOF / out of input) */
#define EXTERR_INSUF_DSK_SPACE 0x27 /* insufficient disk space */
#define EXTERR_RESERVED_0x28 0x28 /* 0x28 reserved */
#define EXTERR_RESERVED_0x29 0x29 /* 0x29 reserved */
#define EXTERR_RESERVED_0x2a 0x2a /* 0x2a reserved */
#define EXTERR_RESERVED_0x2b 0x2b /* 0x2b reserved */
#define EXTERR_RESERVED_0x2c 0x2c /* 0x2c reserved */
#define EXTERR_RESERVED_0x2d 0x2d /* 0x2d reserved */
#define EXTERR_RESERVED_0x2e 0x2e /* 0x2e reserved */
#define EXTERR_RESERVED_0x2f 0x2f /* 0x2f reserved */
#define EXTERR_RESERVED_0x30 0x30 /* 0x30 reserved */
#define EXTERR_RESERVED_0x31 0x31 /* 0x31 reserved */
/* OEM network errors (INT 24) */
#define EXTERR_NET_REQ_NOT_SUPPORTED 0x32 /* network request not supported */
#define EXTERR_REMOTE_COMPUTER_NOT_LISTENING 0x33 /* remote computer not listening */
#define EXTERR_DUP_NAME_ON_NET 0x34 /* duplicate name on network */
#define EXTERR_NET_NAME_NOT_FOUND_0x35 0x35 /* network name not found */
#define EXTERR_NET_BUSY 0x36 /* network busy */
#define EXTERR_NET_DEV_NO_LONGER_EXISTS 0x37 /* network device no longer exists */
#define EXTERR_NET_BIOS_CMD_LIMIT_EXCEEDED 0x38 /* network BIOS command limit
						   exceeded */
#define EXTERR_NET_ADAPT_HW_ERROR 0x39 /* network adapter hardware error */
#define EXTERR_INCORRECT_RESP_FROM_NET 0x3a /* incorrect response from network */
#define EXTERR_UNEXPECTED_NET_ERROR 0x3b /* unexpected network error */
#define EXTERR_INCOMPAT_REMOTE_ADAPT 0x3c /* incompatible remote adapter */
#define EXTERR_PRINT_QUEUE_FULL 0x3d /* print queue full */
#define EXTERR_QUEUE_NOT_FULL 0x3e /* queue not full */
#define EXTERR_NOT_ENOUGH_SPACE_TO_PRINT_FILE 0x3f /* not enough space to print file */
#define EXTERR_NET_NAME_DEL 0x40 /* network name was deleted */
#define EXTERR_NET_ACCESS_DENIED 0x41 /* network: Access denied */
#define EXTERR_CP_SWITCH_NOT_POSSIBLE 0x41 /* (DOS 3.0+ [maybe 3.3+???])
					      codepage switching not possible */
#define EXTERR_NET_DEV_TYPE_INCORRECT 0x42 /* network device type incorrect */
#define EXTERR_NET_NAME_NOT_FOUND_0x43 0x43 /* network name not found */
#define EXTERR_NET_NAME_LIMIT_EXCEEDED 0x44 /* network name limit exceeded */
#define EXTERR_NET_BIOS_SESSION_LIMIT_EXCEEDED 0x45 /* network BIOS session limit
						       exceeded */
#define EXTERR_TEMPORARILY_PAUSED 0x46 /* temporarily paused */
#define EXTERR_NET_REQ_NOT_ACCEPT 0x47 /* network request not accepted */
#define EXTERR_NET_PRINT_DSK_REDIR_PAUSED 0x48 /* network print/disk redirection
						  paused */
#define EXTERR_NET_SW_NOT_INSTALLED 0x49 /* network software not installed */
#define EXTERR_NET_INVAL_NET_VER 0x49 /* (LANtastic) invalid network version */
#define EXTERR_UNEXPECT_ADAPT_CLOSE 0x4a /* unexpected adapter close */
#define EXTERR_ACCOUNT_EXPIRED 0x4a /* (LANtastic) account expired */
#define EXTERR_PASSWD_EXPIRED 0x4b /* (LANtastic) password expired */
#define EXTERR_LOGIN_ATTEMPT_INVAL 0x4c /* (LANtastic) login attempt invalid
					   at this time */
#define EXTERR_DSK_LIMIT_EXCEEDED_NET_NODE 0x4d /* (LANtastic v3+) disk limit
						   exceeded on network node */
#define EXTERR_NOT_LOGGED_NET_NODE 0x4e /* (LANtastic v3+) not logged in to
					   network node */
#define EXTERR_RESERVED_0x4f 0x4f /* 0x4f reserved */
/* end of errors reportable via INT 24 */
#define EXTERR_FILE_EXISTS 0x50 /* file exists */
#define EXTERR_DUP_FCB 0x51 /* (undoc) duplicated FCB */
#define EXTERR_CANNOT_MKDIR 0x52 /* cannot make directory */
#define EXTERR_FAIL_CRITIC_ERR_HANDLER 0x53 /* fail on INT 24h */
/* network-related errors (non-INT 24) */
/* DOS 3.3+ */
#define EXTERR_TOO_MANY_REDIRS 0x54 /* too many redirections / out of structures */
#define EXTERR_DUP_REDIR 0x55 /* duplicate redirection / already assigned */
#define EXTERR_INVAL_PASSWD 0x56 /* invalid password */
#define EXTERR_INVAL_PARM 0x57 /* invalid parameter */
#define EXTERR_NET_WRITE_FAULT 0x58 /* network write fault */
/* DOS 4.0+ */
#define EXTERR_FN_NOT_SUPPORTED_NET 0x59 /* function not supported on network
					    / no process slots available */
#define EXTERR_SYS_COMP_NOT_INSTALL 0x5a /* required system component not installed
					    / not frozen */
/* DOS 4.0+ and NetWare4 */
#define EXTERR_TIMER_SRV_TABLE_OVERFLOWED 0x5b /* timer server table overflowed */
#define EXTERR_DUP_TIMER_SRV_TABLE 0x5c /* duplicate in timer service table */
#define EXTERR_NO_ITEMS_TO_WORK_ON 0x5d /* no items to work on */
#define EXTERR_UNKNOWN_0x5e 0x5e /* 0x5e unknown */
#define EXTERR_INVAL_SYS_CALL 0x5f /* interrupted / invalid system call */
#define EXTERR_UNKNOWN_0x60 0x60 /* 0x60 unknown */
#define EXTERR_UNKNOWN_0x61 0x61 /* 0x61 unknown */
#define EXTERR_UNKNOWN_0x62 0x62 /* 0x62 unknown */
#define EXTERR_UNKNOWN_0x63 0x63 /* 0x63 unknown */
#define EXTERR_UNKNOWN_ERROR 0x64 /* (MSCDEX) unknown error */
#define EXTERR_OPEN_SEMAPHORE_LIMIT_EXCEEDED 0x64 /* open semaphore limit exceeded */
#define EXTERR_NOT_READY 0x65 /* (MSCDEX) not ready */
#define EXTERR_EXCL_SEMAPHORE_ALREADY_OWNED 0x65 /* exclusive semaphore is already
						    owned */
#define EXTERR_EMS_NO_LONGER_VALID 0x66 /* (MSCDEX) EMS memory no longer valid */
#define EXTERR_SEMAPHORE_SET_CLOSE_ATTEMPT 0x66 /* semaphore was set when close
						   attempted */
#define EXTERR_NOT_HIGH_SIERRA_OR_ISO9660 0x67 /* (MSCDEX) not High Sierra
						  or ISO-9660 format */
#define EXTERR_TOO_MANY_EXCL_SEMAPHORE_REQS 0x67 /* too many exclusive semaphore
						    requests */
#define EXTERR_DOOR_OPEN 0x68 /* (MSCDEX) door open */
#define EXTERR_OPER_INVAL_INT_HANDLER 0x68 /* operation invalid from interrupt
					      handler */
#define EXTERR_SEMAPHORE_OWNER_DIED 0x69 /* semaphore owner died */
#define EXTERR_SEMAPHORE_LIMIT_EXCEEDED 0x6a /* semaphore limit exceeded */
#define EXTERR_DSK_CHANGED 0x6b /* insert drive B: Disk into A:
				   / disk changed */
#define EXTERR_DRV_LOCK_OTHER_PROC 0x6c /* drive locked by another process */
#define EXTERR_BROKEN_PIPE 0x6d /* broken pipe */
/* DOS 5.0+ and NetWare4 */
#define EXTERR_PIPE_OPEN_FAIL 0x6e /* pipe open/create failed */
#define EXTERR_PIPE_BUFFER_OVERFLOWED 0x6f /* pipe buffer overflowed */
#define EXTERR_DSK_FULL 0x70 /* disk full */
#define EXTERR_NO_MORE_SEARCH_HANDLES 0x71 /* no more search handles */
#define EXTERR_INVAL_TARGET_HANDLE_DUP2 0x72 /* invalid target handle for dup2 */
#define EXTERR_BAD_USR_VIRTUAL_ADDRESS 0x73 /* bad user virtual address
					       / protection violation */
#define EXTERR_VIOKBD_REQ 0x74 /* (DOS 5.0+) VIOKBD request */
#define EXTERR_ERR_CONS_IO 0x74 /* (NetWare4) error on console I/O */
#define EXTERR_UNKNOWN_CATEGORY_IOCTL 0x75 /* unknown category code for IOCTL */
#define EXTERR_INVAL_VERIFY_FLAG 0x76 /* invalid value for verify flag */
#define EXTERR_LVL4_DRV_NOT_FOUND_DOS_IOCTL 0x77 /* level four driver not found
						    by DOS IOCTL */
#define EXTERR_INVAL_FN_NUM 0x78 /* invalid / unimplemented function number */
#define EXTERR_SEMAPHORE_TIMEOUT 0x79 /* semaphore timeout */
#define EXTERR_RETURN_DATA_BUFFER_TOO_SMALL_ 0x7a /* buffer too small to hold
						     return data */
#define EXTERR_INVAL_CHAR 0x7b /* invalid character or bad file-system name */
#define EXTERR_UNIMPL_INFO_LVL 0x7c /* unimplemented information level */
#define EXTERR_NO_VOL_LABEL_FOUND 0x7d /* no volume label found */
#define EXTERR_MOD_HANDLE_NOT_FOUND 0x7e /* module handle not found */
#define EXTERR_PROC_ADDR_NOT_FOUND 0x7f /* procedure address not found */
#define EXTERR_CWAIT_FOUND_NO_CHILDREN 0x80 /* CWait found no children */
#define EXTERR_CWAIT_CHILDREN_STILL_RUNNING 0x81 /* CWait children still running */
#define EXTERR_INVAL_OPER_DIRECT_DSK_ACCESS_HANDLE 0x82 /* invalid operation for
							   direct disk-access handle */
#define EXTERR_ATTEMPT_SEEK_NEG_OFFSET 0x83 /* attempted seek to negative offset */
#define EXTERR_ATTEMPT_SEEK_DEV_PIPE 0x84 /* attempted to seek on device or pipe */
/* JOIN/SUBST errors */
#define EXTERR_DRV_ALREADY_JOINED_DRVS 0x85 /* drive already has JOINed drives */
#define EXTERR_DRV_ALREADY_JOINED 0x86 /* drive is already JOINed */
#define EXTERR_DRV_ALREADY_SUBSTED 0x87 /* drive is already SUBSTed */
#define EXTERR_CANT_DEL_DRV_NOT_JOINED 0x88 /* can not delete drive which
					       is not JOINed */
#define EXTERR_CANT_DEL_DRV_NOT_SUBSTED 0x89 /* can not delete drive which
						is not SUBSTed */
#define EXTERR_CANT_JOIN_JOINED_DRV 0x8a /* can not JOIN to a JOINed drive */
#define EXTERR_CANT_SUBST_SUBSTED_DRV 0x8b /* can not SUBST to a SUBSTed drive */
#define EXTERR_CANT_JOIN_SUBSTED_DRV 0x8c /* can not JOIN to a SUBSTed drive */
#define EXTERR_CANT_SUBST_JOINED_DRV 0x8d /* can not SUBST to a JOINed drive */
#define EXTERR_DRV_BUSY 0x8e /* drive is busy */
#define EXTERR_CANT_JOIN_SUBST_SAME_DRIVE 0x8f /* can not JOIN/SUBST to same drive */
#define EXTERR_DIR_ROOT 0x90 /* directory must not be root directory */
#define EXTERR_JOIN_DIR_NOT_EMPTY 0x91 /* can only JOIN to empty directory */
#define EXTERR_PATH_IN_USE_SUBST 0x92 /* path is already in use for SUBST */
#define EXTERR_PATH_IN_USE_JOIN 0x93 /* path is already in use for JOIN */
#define EXTERR_PATH_IN_USE_OTHER_PROC 0x94 /* path is in use by another process */
#define EXTERR_DIR_PREV_SUBSTED 0x95 /* directory previously SUBSTituted */
#define EXTERR_SYS_TRACE_ERR 0x96 /* system trace error */
#define EXTERR_INVAL_EVENT_COUNT_DOSMUXSEMWAIT 0x97 /* invalid event count for
						       DosMuxSemWait */
#define EXTERR_TOO_MANY_WAIT_MUTEX 0x98 /* too many waiting on mutex */
#define EXTERR_INVAL_LIST_FORMAT 0x99 /* invalid list format */
#define EXTERR_VOL_LABEL_TOO_LARGE 0x9a /* volume label too large */
#define EXTERR_UNABLE_CREATE_TCB 0x9b /* unable to create another TCB */
#define EXTERR_SIGNAL_REFUSED 0x9c /* signal refused */
#define EXTERR_SEGMENT_DISCARDED 0x9d /* segment discarded */
#define EXTERR_SEGMENT_NOT_LOCKED 0x9e /* segment not locked */
#define EXTERR_INVAL_THREAD_ID_ADDR 0x9f /* invalid thread-ID address */
/* ----- */
#define EXTERR_BAD_ARGUMENTS 0xa0 /* (DOS 5.0+) bad arguments */
#define EXTERR_BAD_ENV_PTR 0xa0 /* (NetWare4) bad environment pointer */
#define EXTERR_INVAL_PATH_EXEC 0xa1 /* invalid pathname passed to EXEC */
#define EXTERR_SIGNAL_ALREADY_PENDING 0xa2 /* signal already pending */
#define EXTERR_UNCERTAIN_MEDIA 0xa3 /* (DOS 5.0+) uncertain media */
#define EXTERR_ERR_124_MAP_0xa3 0xa3 /* (NetWare4) ERROR_124 mapping */
#define EXTERR_MAX_NUM_THREADS_REACHED 0xa4 /* (DOS 5.0+) maximum number of
					       threads reached */
#define EXTERR_NO_MORE_PROC_SLOTS 0xa4 /* (NetWare4) no more process slots */
#define EXTERR_ERR_124_MAP_0xa5 0xa5 /* (NetWare4) ERROR_124 mapping */
/* unknown range */
#define EXTERR_UNKNOWN_0xa6 0xa6 /* 0xa6 unknown */
#define EXTERR_UNKNOWN_0xa7 0xa7 /* 0xa7 unknown */
#define EXTERR_UNKNOWN_0xa8 0xa8 /* 0xa8 unknown */
#define EXTERR_UNKNOWN_0xa9 0xa9 /* 0xa9 unknown */
#define EXTERR_UNKNOWN_0xaa 0xaa /* 0xaa unknown */
#define EXTERR_UNKNOWN_0xab 0xab /* 0xab unknown */
#define EXTERR_UNKNOWN_0xac 0xac /* 0xac unknown */
#define EXTERR_UNKNOWN_0xad 0xad /* 0xad unknown */
#define EXTERR_UNKNOWN_0xae 0xae /* 0xae unknown */
#define EXTERR_UNKNOWN_0xaf 0xaf /* 0xaf unknown */
/* MS-DOS 7.0 */
#define EXTERR_VOL_NOT_LOCK 0xb0 /* volume is not locked */
#define EXTERR_VOL_LOCK_DRV 0xb1 /* volume is locked in drive */
#define EXTERR_VOL_NOT_REMOVABLE 0xb2 /* volume is not removable */
#define EXTERR_UNKNOWN_0xb3 0xb3 /* 0xb3 unknown */
#define EXTERR_LOCK_COUNT_EXCEEDED 0xb4 /* lock count has been exceeded */
#define EXTERR_INVAL_SEG_NUM 0xb4 /* (NetWare4) invalid segment number */
#define EXTERR_VAL_EJECT_REQ_FAIL 0xb5 /* a valid eject request failed */
#define EXTERR_INVAL_CALL_GATE 0xb5 /* (DOS 5.0-6.0,NetWare4) invalid call gate */
/* DOS 5.0+ and NetWare4 */
#define EXTERR_INVAL_ORD 0xb6 /* invalid ordinal */
#define EXTERR_SHARED_SEG_ALREADY_EXISTS 0xb7 /* shared segment already exists */
#define EXTERR_NO_CHILD_PROC_WAIT 0xb8 /* no child process to wait for */
#define EXTERR_NOWAIT_CHILD_RUN 0xb9 /* NoWait specified and child still running */
#define EXTERR_INVAL_FLAG_NUM 0xba /* invalid flag number */
#define EXTERR_SEMAPHORE_DONT_EXIST 0xbb /* semaphore does not exist */
#define EXTERR_INVAL_START_CODE_SEG 0xbc /* invalid starting code segment */
#define EXTERR_INVAL_STACK_SEG 0xbd /* invalid stack segment */
#define EXTERR_INVAL_MOD_TYPE 0xbe /* invalid module type
				      (DLL can not be used as application) */
#define EXTERR_INVAL_EXE_SIGN 0xbf /* invalid EXE signature */
#define EXTERR_EXE_MARK_INVAL 0xc0 /* EXE marked invalid */
#define EXTERR_BAD_EXE_FORMAT 0xc1 /* bad EXE format (e.g. DOS-mode program) */
#define EXTERR_ITER_DATA_EXCEEDS_64K 0xc2 /* iterated data exceeds 64K */
#define EXTERR_INVAL_MIN_ALLOC_SIZE 0xc3 /* invalid minimum allocation size */
#define EXTERR_DYN_LINK_INVAL_RING 0xc4 /* dynamic link from invalid Ring */
#define EXTERR_IOPL_NOT_ENABLED 0xc5 /* IOPL not enabled */
#define EXTERR_INVAL_SEG_DESC_PRIV_LVL 0xc6 /* invalid segment descriptor
					       privilege level */
#define EXTERR_AUTO_DATA_SEG_EXCEEDS_64K 0xc7 /* automatic data segment exceeds 64K */
#define EXTERR_RING2_SEG_MUST_MOV 0xc8 /* Ring2 segment must be moveable */
#define EXTERR_RELOC_CHAIN_EXCEEDS_SEG_LIMIT 0xc9 /* relocation chain exceeds
						     segment limit */
#define EXTERR_INF_LOOP_RELOC_CHAIN 0xca /* infinite loop in relocation chain */
/* NetWare4 */
#define EXTERR_ENV_VAR_NOT_FOUND 0xcb /* environment variable not found */
#define EXTERR_NOT_CURRENT_COUNTRY 0xcc /* not current country */
#define EXTERR_NO_SIGNAL_SENT 0xcd /* no signal sent */
#define EXTERR_FILE_NAME_NOT_83 0xce /* file name not 8.3 */
#define EXTERR_RING2_STACK_IN_USE 0xcf /* Ring2 stack in use */
#define EXTERR_META_EXP_TOO_LONG 0xd0 /* meta expansion is too long */
#define EXTERR_INVAL_SIGNAL_NUM 0xd1 /* invalid signal number */
#define EXTERR_INACT_THREAD 0xd2 /* inactive thread */
#define EXTERR_FILE_SYS_INFO_NOT_AVAIL 0xd3 /* file system information
					       not available */
#define EXTERR_LOCKED_ERR 0xd4 /* locked error */
#define EXTERR_ATTEMPT_EXEC_NON_FAMILY_CALL_IN_DOS 0xd5 /* attempted to execute
							   non-family API call
							   in DOS mode */
#define EXTERR_TOO_MANY_MODS 0xd6 /* too many modules */
#define EXTERR_NEST_NOT_ALLOWED 0xd7 /* nesting not allowed */
/* unknown range */
#define EXTERR_UNKNOWN_0xd8 0xd8 /* 0xd8 unknown */
#define EXTERR_UNKNOWN_0xd9 0xd9 /* 0xd9 unknown */
#define EXTERR_UNKNOWN_0xda 0xda /* 0xda unknown */
#define EXTERR_UNKNOWN_0xdb 0xdb /* 0xdb unknown */
#define EXTERR_UNKNOWN_0xdc 0xdc /* 0xdc unknown */
#define EXTERR_UNKNOWN_0xdd 0xdd /* 0xdd unknown */
#define EXTERR_UNKNOWN_0xde 0xde /* 0xde unknown */
#define EXTERR_UNKNOWN_0xdf 0xdf /* 0xdf unknown */
#define EXTERR_UNKNOWN_0xe0 0xe0 /* 0xe0 unknown */
#define EXTERR_UNKNOWN_0xe1 0xe1 /* 0xe1 unknown */
#define EXTERR_UNKNOWN_0xe2 0xe2 /* 0xe2 unknown */
#define EXTERR_UNKNOWN_0xe3 0xe3 /* 0xe3 unknown */
#define EXTERR_UNKNOWN_0xe4 0xe4 /* 0xe4 unknown */
#define EXTERR_UNKNOWN_0xe5 0xe5 /* 0xe5 unknown */
/* NetWare4 */
#define EXTERR_BAD_OPER 0xe6 /* non-existent pipe, or bad operation */
#define EXTERR_PIPE_BUSY 0xe7 /* pipe is busy */
#define EXTERR_NO_DATA_AVAIL_NONBLOCK_READ 0xe8 /* no data available for
						   nonblocking read */
#define EXTERR_PIPE_DISCONNECT_SRV 0xe9 /* pipe disconnected by server */
#define EXTERR_MORE_DATA_AVAIL 0xea /* more data available */
/* unknown range */
#define EXTERR_UNKNOWN_0xeb 0xeb /* 0xeb unknown */
#define EXTERR_UNKNOWN_0xec 0xec /* 0xec unknown */
#define EXTERR_UNKNOWN_0xed 0xed /* 0xed unknown */
#define EXTERR_UNKNOWN_0xee 0xee /* 0xee unknown */
#define EXTERR_UNKNOWN_0xef 0xef /* 0xef unknown */
#define EXTERR_UNKNOWN_0xf0 0xf0 /* 0xf0 unknown */
#define EXTERR_UNKNOWN_0xf1 0xf1 /* 0xf1 unknown */
#define EXTERR_UNKNOWN_0xf2 0xf2 /* 0xf2 unknown */
#define EXTERR_UNKNOWN_0xf3 0xf3 /* 0xf3 unknown */
#define EXTERR_UNKNOWN_0xf4 0xf4 /* 0xf4 unknown */
#define EXTERR_UNKNOWN_0xf5 0xf5 /* 0xf5 unknown */
#define EXTERR_UNKNOWN_0xf6 0xf6 /* 0xf6 unknown */
#define EXTERR_UNKNOWN_0xf7 0xf7 /* 0xf7 unknown */
#define EXTERR_UNKNOWN_0xf8 0xf8 /* 0xf8 unknown */
#define EXTERR_UNKNOWN_0xf9 0xf9 /* 0xf9 unknown */
#define EXTERR_UNKNOWN_0xfa 0xfa /* 0xfa unknown */
#define EXTERR_UNKNOWN_0xfb 0xfb /* 0xfb unknown */
#define EXTERR_UNKNOWN_0xfc 0xfc /* 0xfc unknown */
#define EXTERR_UNKNOWN_0xfd 0xfd /* 0xfd unknown */
#define EXTERR_UNKNOWN_0xfe 0xfe /* 0xfe unknown */
/* NetWare4 */
#define EXTERR_INVAL_DRV_0xff 0xff /* invalid drive */
/* don’t change mark */
#define EXTERR_DONT_CHANGE 0xff

/*** DOS Error Class ***/
#define ERRCLASS_NONE 0x00 /* none */
#define ERRCLASS_OUT_OF_RESOURCE 0x01 /* out of resource
					 (storage space or I/O channels) */
#define ERRCLASS_TMP_SITUATION 0x02 /* temporary situation (file or record lock) */
#define ERRCLASS_ACCESS_DENIED 0x03 /* authorization
				       / permission problem (denied access) */
#define ERRCLASS_INTERN_SYS_ERROR 0x04 /* internal system error
					  (system software bug) */
#define ERRCLASS_HW_FAIL 0x05 /* hardware failure */
#define ERRCLASS_SYS_FAIL 0x06 /* system failure
				  (configuration file missing or incorrect) */
#define ERRCLASS_APP_PROG_ERROR 0x07 /* application program error */
#define ERRCLASS_NOT_FOUND 0x08 /* not found */
#define ERRCLASS_BAD_FORMAT 0x09 /* bad format */
#define ERRCLASS_LOCKED 0x0a /* locked */
#define ERRCLASS_MEDIA_ERROR 0x0b /* media error */
#define ERRCLASS_ALREADY_EXISTS 0x0c /* already exists / collision with
					existing item */
#define ERRCLASS_UNKNOWN 0x0d /* unknown / other */
#define ERRCLASS_CANNOT 0x0e /* (undoc) cannot */
#define ERRCLASS_TIME 0x0f /* (undoc) time */
/* don’t change mark */
#define ERRCLASS_DONT_CHANGE 0xff

/*** DOS Suggested Action ***/
#define ERRACT_NONE 0x00 /* none */
#define ERRACT_RETRY 0x01 /* retry */
#define ERRACT_DELAYED_RETRY 0x02 /* delayed retry (after pause) */
#define ERRACT_PROMPT_USR_REENTER_INPUT 0x03 /* prompt user to reenter input */
#define ERRACT_ABORT_AFTER_CLEANUP 0x04 /* abort after cleanup */
#define ERRACT_IMMEDIATE_ABORT 0x05 /* immediate abort ("panic") */
#define ERRACT_IGNORE 0x06 /* ignore */
#define ERRACT_RETRY_AFTER_USR_INTERV 0x07 /* retry after user intervention */
/* don’t change mark */
#define ERRACT_DONT_CHANGE 0xff

/*** DOS Error Locus ***/
#define ERRLOCUS_NONE 0x00 /* none */
#define ERRLOCUS_UNKNOWN 0x01 /* unknown or not appropriate */
#define ERRLOCUS_BLOCK_DEV 0x02 /* block device (disk error) */
#define ERRLOCUS_NET_RELATED 0x03 /* network related */
#define ERRLOCUS_SERIAL_DEV 0x04 /* serial device (timeout) */
#define ERRLOCUS_CHAR_DEV 0x04 /* (PTS-DOS 6.51+ & S/DOS 1.0+) character device */
#define ERRLOCUS_MEM_RELATED 0x05 /* memory related */
/* don’t change mark */
#define ERRLOCUS_DONT_CHANGE 0xff


/* _dosexterr */

int
_dosix__dosexterr
(struct _DOSERROR *_errorinfo)
{
  switch (errno)
    {
    case 0:			/* report errorinfo as is */
      break;
    case EPERM:
      errorinfo.exterror = EXTERR_ACCESS_DENIED;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOENT:
      errorinfo.exterror = EXTERR_FILE_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case ESRCH:
      errorinfo.exterror = EXTERR_PROC_ADDR_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EINTR:
      errorinfo.exterror = EXTERR_INVAL_SYS_CALL;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_RETRY;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EIO:
      errorinfo.exterror = EXTERR_READ_FAULT;
      errorinfo.errclass = ERRCLASS_HW_FAIL;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENXIO:
      errorinfo.exterror = EXTERR_LVL4_DRV_NOT_FOUND_DOS_IOCTL;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case E2BIG:
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOEXEC:
      errorinfo.exterror = EXTERR_BAD_EXE_FORMAT;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EBADF:
      errorinfo.exterror = EXTERR_INVAL_HANDLE;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ECHILD:
      errorinfo.exterror = EXTERR_CWAIT_FOUND_NO_CHILDREN;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EDEADLK:
      errorinfo.exterror = EXTERR_LOCK_VIOLATION;
      errorinfo.errclass = ERRCLASS_LOCKED;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOMEM:
      errorinfo.exterror = EXTERR_INSUF_MEM;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      break;
    case EACCES:
      errorinfo.exterror = EXTERR_ACCESS_DENIED;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EFAULT:
      errorinfo.exterror = EXTERR_MBA_INVAL;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      break;
    case ENOTBLK:
      errorinfo.exterror = EXTERR_INVAL_DRV_0x0f;
      errorinfo.errclass = ERRCLASS_MEDIA_ERROR;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EBUSY:
      errorinfo.exterror = EXTERR_DRV_BUSY;
      errorinfo.errclass = ERRCLASS_LOCKED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EEXIST:
      errorinfo.exterror = EXTERR_FILE_EXISTS;
      errorinfo.errclass = ERRCLASS_ALREADY_EXISTS;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EXDEV:
      errorinfo.exterror = EXTERR_NOT_SAME_DEVICE;
      errorinfo.errclass = ERRCLASS_CANNOT;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case ENODEV:
      errorinfo.exterror = EXTERR_UNKNOWN_MEDIA_TYPE;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOTDIR:
      errorinfo.exterror = EXTERR_PATH_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EISDIR:
      errorinfo.exterror = EXTERR_FILE_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EINVAL:
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EMFILE:
      errorinfo.exterror = EXTERR_TOO_MANY_OPEN_FILES;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENFILE:
      errorinfo.exterror = EXTERR_TOO_MANY_OPEN_FILES;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOTTY:
      errorinfo.exterror = EXTERR_UNKNOWN_CATEGORY_IOCTL;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_CHAR_DEV;
      break;
    case ETXTBSY:
      errorinfo.exterror = EXTERR_CANT_COMPLETE_FILE_OP;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EFBIG:
      errorinfo.exterror = EXTERR_INSUF_DSK_SPACE;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case ENOSPC:
      errorinfo.exterror = EXTERR_DSK_FULL;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case ESPIPE:
      errorinfo.exterror = EXTERR_ATTEMPT_SEEK_DEV_PIPE;
      errorinfo.errclass = ERRCLASS_CANNOT;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EROFS:
      errorinfo.exterror = EXTERR_DSK_WRITE_PROTECTED;
      errorinfo.errclass = ERRCLASS_CANNOT;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EMLINK:
    case ETOOMANYREFS:
    case ELOOP:
      errorinfo.exterror = EXTERR_TOO_MANY_REDIRS;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EPIPE:
      errorinfo.exterror = EXTERR_BROKEN_PIPE;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EDOM:
      errorinfo.exterror = EXTERR_DATA_INVAL;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ERANGE:
      errorinfo.exterror = EXTERR_FIXUP_OVERFLOW;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#if EWOULDBLOCK != EAGAIN
    case EWOULDBLOCK:
      errorinfo.exterror = EXTERR_FCB_UNAVAILABLE;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EAGAIN:
      errorinfo.exterror = EXTERR_FCB_UNAVAILABLE;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#else
    case EAGAIN:
      errorinfo.exterror = EXTERR_FCB_UNAVAILABLE;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
    case EINPROGRESS:
      errorinfo.exterror = EXTERR_NET_BUSY;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EALREADY:
      errorinfo.exterror = EXTERR_NET_BUSY;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOTSOCK:
      errorinfo.exterror = EXTERR_NET_DEV_TYPE_INCORRECT;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EMSGSIZE:
      errorinfo.exterror = EXTERR_NET_WRITE_FAULT;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EPROTOTYPE:
    case ENOPROTOOPT:
      errorinfo.exterror = EXTERR_NET_REQ_NOT_SUPPORTED;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
#if ENOTSUP != EOPNOTSUPP
    case ENOTSUP:
      errorinfo.exterror = EXTERR_INVAL_PARM;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EOPNOTSUPP:
      errorinfo.exterror = EXTERR_FN_NOT_SUPPORTED_NET;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
#else
    case ENOTSUP:
      errorinfo.exterror = EXTERR_INVAL_PARM;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
    case EPROTONOSUPPORT:
    case ESOCKTNOSUPPORT:
    case EPFNOSUPPORT:
    case EAFNOSUPPORT:
      errorinfo.exterror = EXTERR_FN_NOT_SUPPORTED_NET;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EADDRINUSE:
      errorinfo.exterror = EXTERR_DUP_NAME_ON_NET;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EADDRNOTAVAIL:
      errorinfo.exterror = EXTERR_NET_NAME_NOT_FOUND_0x35;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENETDOWN:
    case ENETUNREACH:
      errorinfo.exterror = EXTERR_NET_DEV_NO_LONGER_EXISTS;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENETRESET:
      errorinfo.exterror = EXTERR_REMOTE_COMPUTER_NOT_LISTENING;
      errorinfo.errclass = ERRCLASS_MEDIA_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ECONNABORTED:
      errorinfo.exterror = EXTERR_NET_NAME_DEL;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ECONNRESET:
      errorinfo.exterror = EXTERR_UNEXPECT_ADAPT_CLOSE;
      errorinfo.errclass = ERRCLASS_UNKNOWN;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOBUFS:
      errorinfo.exterror = EXTERR_INSUF_MEM;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      break;
    case EISCONN:
      errorinfo.exterror = EXTERR_DUP_REDIR;
      errorinfo.errclass = ERRCLASS_ALREADY_EXISTS;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOTCONN:
    case EDESTADDRREQ:
    case EHOSTDOWN:
    case EHOSTUNREACH:
      errorinfo.exterror = EXTERR_REMOTE_COMPUTER_NOT_LISTENING;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ESHUTDOWN:
      errorinfo.exterror = EXTERR_NET_DEV_NO_LONGER_EXISTS;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRCLASS_CANNOT;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ETIMEDOUT:
      errorinfo.exterror = EXTERR_UNEXPECT_ADAPT_CLOSE;
      errorinfo.errclass = ERRCLASS_TIME;
      errorinfo.action = ERRACT_RETRY;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ECONNREFUSED:
      errorinfo.exterror = EXTERR_NET_REQ_NOT_ACCEPT;
      errorinfo.errclass = ERRCLASS_UNKNOWN;
      errorinfo.action = ERRACT_DELAYED_RETRY;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENAMETOOLONG:
      errorinfo.exterror = EXTERR_NET_NAME_LIMIT_EXCEEDED;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ENOTEMPTY:
      errorinfo.exterror = EXTERR_JOIN_DIR_NOT_EMPTY;
      errorinfo.errclass = ERRCLASS_CANNOT;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
#ifdef EPROCLIM
    case EPROCLIM:
      errorinfo.exterror = EXTERR_NO_MORE_PROC_SLOTS;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
    case EUSERS:
      errorinfo.exterror = EXTERR_TOO_MANY_OPEN_FILES;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EDQUOT:
      errorinfo.exterror = EXTERR_DSK_LIMIT_EXCEEDED_NET_NODE;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case ESTALE:
      errorinfo.exterror = EXTERR_INVAL_HANDLE;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
    case EREMOTE:
      errorinfo.exterror = EXTERR_DRV_LOCK_OTHER_PROC;
      errorinfo.errclass = ERRCLASS_LOCKED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
#ifdef EBADRPC
    case EBADRPC:
      errorinfo.exterror = EXTERR_BAD_REQ_STRUCT_LEN;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef ERPCMISMATCH
    case ERPCMISMATCH:
      errorinfo.exterror = EXTERR_NET_INVAL_NET_VER;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef EPROGUNAVAIL
    case EPROGUNAVAIL:
      errorinfo.exterror = EXTERR_NET_INVAL_NET_VER;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef EPROGMISMATCH
    case EPROGMISMATCH:
      errorinfo.exterror = EXTERR_NET_INVAL_NET_VER;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef EPROCUNAVAIL
    case EPROCUNAVAIL:
      errorinfo.exterror = EXTERR_FN_NUM_INVAL;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
    case ENOLCK:
      errorinfo.exterror = EXTERR_LOCK_COUNT_EXCEEDED;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#ifdef EFTYPE
    case EFTYPE:
      errorinfo.exterror = EXTERR_FMT_INVAL;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      break;
#endif
#ifdef EAUTH
    case EAUTH:
      errorinfo.exterror = EXTERR_LOGIN_ATTEMPT_INVAL;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef ENEEDAUTH
    case ENEEDAUTH:
      errorinfo.exterror = EXTERR_LOGIN_ATTEMPT_INVAL;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
    case ENOSYS:
      errorinfo.exterror = EXTERR_INVAL_FN_NUM;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EILSEQ:
      errorinfo.exterror = EXTERR_INVAL_CHAR;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_CHAR_DEV;
      break;
#ifdef EBACKGROUND
    case EBACKGROUND:
      errorinfo.exterror = EXTERR_OPER_INVAL_INT_HANDLER;
      errorinfo.errclass = ERRCLASS_SYS_FAIL;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_CHAR_DEV;
      break;
#endif
#ifdef EDIED
    case EDIED:
      errorinfo.exterror = EXTERR_SEMAPHORE_OWNER_DIED;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
#endif
#ifdef ED
    case ED:
      errorinfo.exterror = EXTERR_NO_ERROR;
      errorinfo.errclass = ERRCLASS_NONE;
      errorinfo.action = ERRACT_NONE;
      errorinfo.locus = ERRLOCUS_NONE;
      break;
#endif
#ifdef EGREGIOUS
    case EGREGIOUS:
      errorinfo.exterror = EXTERR_NO_ERROR;
      errorinfo.errclass = ERRCLASS_NONE;
      errorinfo.action = ERRACT_NONE;
      errorinfo.locus = ERRLOCUS_NONE;
      break;
#endif
#ifdef EIEIO
    case EIEIO:
      errorinfo.exterror = EXTERR_NO_ERROR;
      errorinfo.errclass = ERRCLASS_NONE;
      errorinfo.action = ERRACT_NONE;
      errorinfo.locus = ERRLOCUS_NONE;
      break;
#endif
#ifdef EGRATUITOUS
    case EGRATUITOUS:
      errorinfo.exterror = EXTERR_NO_ERROR;
      errorinfo.errclass = ERRCLASS_NONE;
      errorinfo.action = ERRACT_NONE;
      errorinfo.locus = ERRLOCUS_NONE;
      break;
#endif
    case EBADMSG:
    case ENOMSG:
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EIDRM:
      errorinfo.exterror = EXTERR_NET_NAME_DEL;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EMULTIHOP:
      errorinfo.exterror = EXTERR_NET_DEV_TYPE_INCORRECT;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENODATA:
      errorinfo.exterror = EXTERR_NO_DATA_AVAIL_NONBLOCK_READ;
      errorinfo.errclass = ERRCLASS_TMP_SITUATION;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOLINK:
      errorinfo.exterror = EXTERR_NET_DEV_NO_LONGER_EXISTS;
      errorinfo.errclass = ERRCLASS_HW_FAIL;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOSR:
      errorinfo.exterror = EXTERR_INSUF_MEM;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ENOSTR:
      errorinfo.exterror = EXTERR_NET_DEV_TYPE_INCORRECT;
      errorinfo.errclass = ERRCLASS_BAD_FORMAT;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case EOVERFLOW:
      errorinfo.exterror = EXTERR_SHARING_BUFFER_OVERFLOW;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case EPROTO:
      errorinfo.exterror = EXTERR_UNEXPECTED_NET_ERROR;
      errorinfo.errclass = ERRCLASS_MEDIA_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_NET_RELATED;
      break;
    case ETIME:
      errorinfo.exterror = EXTERR_TEMPORARILY_PAUSED;
      errorinfo.errclass = ERRCLASS_TIME;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    case ECANCELED:
      errorinfo.exterror = EXTERR_MORE_DATA_AVAIL;
      errorinfo.errclass = ERRCLASS_UNKNOWN;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    default:			/* should never get here */
      assert (false);
      errorinfo.exterror = EXTERR_ACCESS_CODE_INVAL;
      errorinfo.errclass = ERRCLASS_UNKNOWN;
      errorinfo.action = ERRACT_IGNORE;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      break;
    }
  if (_errorinfo) *_errorinfo = errorinfo;
  return errorinfo.exterror;
}

static
void
_dosk86exterr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_EXTERR);
  assert (cpu->h.bh == INT21_BH_EXTERR);
  assert (cpu->l.bl == INT21_BL_EXTERR);
  struct _DOSERROR errorinfo = {0};
  _dosix__dosexterr (&errorinfo);
  cpu->r.ax = errorinfo.exterror;
  cpu->h.bh = errorinfo.errclass;
  cpu->l.bl = errorinfo.action;
  cpu->h.ch = errorinfo.locus;
  if (errorinfo.exterror == EXTERR_DSK_CHANGE_INVAL)
    {
      cpu->r.es = _FP_SEG (&media_id);
      cpu->r.di = _FP_OFF (&media_id);
    }
}

static
int
__doskexterr_set
(struct _DOSERROR *new_errorinfo, int _errno)
{
  if (new_errorinfo->exterror != EXTERR_DONT_CHANGE)
    errorinfo.exterror = new_errorinfo->exterror;
  if (new_errorinfo->errclass != ERRCLASS_DONT_CHANGE)
    errorinfo.errclass = new_errorinfo->errclass;
  if (new_errorinfo->action != ERRACT_DONT_CHANGE)
    errorinfo.action = new_errorinfo->action;
  if (new_errorinfo->locus != ERRLOCUS_DONT_CHANGE)
    errorinfo.locus = new_errorinfo->locus;
  /* update _doserrno on behalf of libc */
  _doserrno = errorinfo.exterror;
  /* note: use zero _errno to prevent _dosexterr from interpreting a
     previous libc error, so it reports the current exterr */
  errno = _errno;
  return errorinfo.exterror;
}

static
void
__dosk86exterr_set
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT2F_AH_DOS_INTERNAL);
  assert (cpu->l.al == INT2F_AL_DOS_INTERNAL_EXTERR_SET);
  __doskexterr_set (_MK_FP (cpu->r.ss, cpu->r.si), 0);
}


/* DTA */

static
union dta_t *
_dosk_get_dta_addr
(void)
{
  return _dosk_current_dta;
}

static
void
_dosk86_get_dta_addr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GET_DTA_ADDR);
  union dta_t *dta_ptr = _dosk_get_dta_addr ();
  cpu->r.es = _FP_SEG (dta_ptr);
  cpu->r.bx = _FP_OFF (dta_ptr);
}

static
void
_dosk_set_dta_addr
(union dta_t *target_dta)
{
  _dosk_current_dta = target_dta;
}

static
void
_dosk86_set_dta_addr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SET_DTA_ADDR);
  _dosk_set_dta_addr (_MK_FP (cpu->r.ds, cpu->r.dx));
}


/* _dos_allocmem */

static
unsigned
free_paragraphs
(uintptr_t *count)
{
  assert (count);
  struct _DOSERROR errorinfo = {0};
  uintptr_t free_pages = sysconf (_SC_AVPHYS_PAGES);
  if (free_pages == -1)
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  uintptr_t page_size = sysconf (_SC_PAGESIZE);
  if (page_size == -1)
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  *count = (free_pages * page_size) / 16;
  return 0;
}

static
unsigned
_dosk__allocmem_error
(uintptr_t *count)
{
  assert (count);
  struct _DOSERROR errorinfo = {0};
  unsigned err = free_paragraphs (count);
  if (err) return err;
  return _dosix__dosexterr (&errorinfo);
}

static
int
_dosk__allocmem_cmp
(const void *_a,
 const void *_b)
{
  const struct allocmem *a = (const struct allocmem *) _a;
  const struct allocmem *b = (const struct allocmem *) _b;
  return a->address - b->address;
}

unsigned
_dosix__dos_allocmem
(size_t size,
 uintptr_t *seg)
{
  assert (seg);
  struct _DOSERROR errorinfo = {0};
  if (! size) return 0;
  struct allocmem *allocmem_ptr = malloc (sizeof (*allocmem_ptr));
  if (! allocmem_ptr)
    return _dosk__allocmem_error (seg);
  allocmem_ptr->length = size * 16;
  allocmem_ptr->address = mmap (NULL,
				allocmem_ptr->length,
				PROT_WRITE | PROT_WRITE | PROT_EXEC,
				MAP_PRIVATE | MAP_ANON,
				0, 0);
  if (allocmem_ptr->address == MAP_FAILED)
    {
      free (allocmem_ptr);
      return _dosk__allocmem_error (seg);
    }
  else
    {
      struct allocmem **_allocmem_ptr = tsearch (allocmem_ptr,
						 &_dosk__allocmem_tree,
						 &_dosk__allocmem_cmp);
      if (! _allocmem_ptr)
	{
	  free (allocmem_ptr);
	  return _dosk__allocmem_error (seg);
	}
      assert (allocmem_ptr == *_allocmem_ptr);
      *seg = (uintptr_t) allocmem_ptr->address;
      return 0;
    }
}

static
void
_dosk86_allocmem
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_ALLOCMEM);
  uintptr_t seg;
  cpu->r.ax = _dosix__dos_allocmem (cpu->r.bx,
				    &seg);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  if (cpu->r.flags)
    cpu->r.bx = seg;
  else
    cpu->r.ax = seg;
}


/* _dos_setblock */

unsigned
_dosix__dos_setblock
(size_t size,
 uintptr_t seg,
 size_t *maxsize)
{
  assert (maxsize);
  struct _DOSERROR errorinfo = {0};
  if (! size) return 0;
  struct allocmem allocmem =
    {
     .address = (void *) seg,
     .length = 16 * size
    };
  struct allocmem **allocmem_ptr = tfind (&allocmem,
					  &_dosk__allocmem_tree,
					  &_dosk__allocmem_cmp);
  if (! allocmem_ptr)
    {
      errno = EFAULT;
      return _dosix__dosexterr (&errorinfo);
    }
  assert (allocmem.address == (*allocmem_ptr)->address);
  allocmem.address = mremap (allocmem.address,
			     (*allocmem_ptr)->length,
			     allocmem.length,
			     0);
  if (allocmem.address == (void *) -1)
    return _dosk__allocmem_error (maxsize);
  assert (allocmem.address == (void *) seg);
  **allocmem_ptr = allocmem;
  return 0;
}

static
void
_dosk86_setblock
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETBLOCK);
  uintptr_t maxsize;
  cpu->r.ax = _dosix__dos_setblock (cpu->r.bx,
				    cpu->r.es,
				    &maxsize);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.bx = cpu->r.flags ? maxsize : cpu->r.bx;
}


/* _dos_freemem */

unsigned
_dosix__dos_freemem
(uintptr_t seg)
{
  assert (seg);
  struct _DOSERROR errorinfo = {0};
  struct allocmem allocmem =
    {
     .address = (void *) seg,
     .length = 0
    };
  struct allocmem **allocmem_ptr = tfind (&allocmem,
					  &_dosk__allocmem_tree,
					  &_dosk__allocmem_cmp);
  if (! allocmem_ptr)
    {
      errno = EFAULT;
      return _dosix__dosexterr (&errorinfo);
    }
  assert (allocmem.address == (*allocmem_ptr)->address);
  if (munmap (allocmem.address,
	      (*allocmem_ptr)->length))
    {
      errorinfo.exterror = EXTERR_MCB_DESTROYED;
      errorinfo.errclass = ERRCLASS_INTERN_SYS_ERROR;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      return __doskexterr_set (&errorinfo, EINVAL);
    }
  void *pnode = tdelete (&allocmem,
			 &_dosk__allocmem_tree,
			 &_dosk__allocmem_cmp);
  assert (pnode);
  free (*allocmem_ptr);
  return 0;
}

static
void
_dosk86_freemem
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FREEMEM);
  cpu->r.ax = _dosix__dos_freemem (cpu->r.es);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* _dos_creat, _dos_creatnew */

static
unsigned
__dos_creat
(const char * filename,
 unsigned attrib,
 int *handle,
 int flags)
{
  assert (filename), assert (handle);
  struct _DOSERROR errorinfo = {0};
  mode_t mode = S_IRWXU
    & ((attrib & _A_RDONLY)
       ? ~S_IWUSR
       : ~0);
  int fd = open (filename,
		 flags,
		 mode);
  if (fd < 0)
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  *handle = fd;
  return 0;
}

unsigned
_dosix__dos_creat
(const char * filename,
 unsigned attrib,
 int *handle)
{
  return __dos_creat (filename,
		      attrib,
		      handle,
		      O_RDWR | O_CREAT | O_TRUNC);
}

static
void
_dosk86_creat
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CREAT);
  int handle;
  cpu->r.ax = _dosix__dos_creat (_MK_FP (cpu->r.ds, cpu->r.dx),
				 cpu->r.cx,
				 &handle);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.ax = cpu->r.ax ? cpu->r.ax : handle;
}

unsigned
_dosix__dos_creatnew
(const char * filename,
 unsigned attrib,
 int *handle)
{
  return __dos_creat (filename,
		      attrib,
		      handle,
		      O_RDWR | O_CREAT | O_TRUNC | O_EXCL);
}

static
void
_dosk86_creatnew
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CREATNEW);
  int handle;
  cpu->r.ax = _dosix__dos_creat (_MK_FP (cpu->r.ds, cpu->r.dx),
				 cpu->r.cx,
				 &handle);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.ax = cpu->r.ax ? cpu->r.ax : handle;
}


/* _dos_open */

unsigned
_dosix__dos_open
(const char *path,
 unsigned mode,
 int *handle)
{
  assert (path), assert (handle);
  struct _DOSERROR errorinfo = {0};
  int flags = (mode & _O_RDONLY ? O_RDONLY : 0)
    | (mode & _O_WRONLY ? O_WRONLY  : 0)
    | (mode & _O_RDWR ? O_RDWR : 0);
  int fd = open (path, flags);
  if (fd < 0)
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  if (! (mode & _SH_DENYNO))
    {
      struct flock flock =
	{
	 /* No use for _SH_CMPAT? */
	 .l_type = (mode & _SH_DENYRW ? F_WRLCK : 0)
	 | (mode & _SH_DENYWR ? F_RDLCK : 0)
	 | (mode & _SH_DENYRD ? F_WRLCK : 0),
	 .l_whence = SEEK_SET,
	 .l_start = 0,
	 .l_len = 0		/* Lock the entire file */
	};
      if (fcntl (fd, F_SETLK, &flock) == -1)
	return _dosix__dosexterr (&errorinfo);	/* TODO? better error handling */
    }
  if (mode & _O_NOINHERIT)
    {
      int flags = fcntl (fd, F_GETFD, 0);
      if (flags == -1)
	return _dosix__dosexterr (&errorinfo);	/* TODO? better error handling */
      flags |= FD_CLOEXEC;
      if (fcntl (fd, F_SETFD, flags) == -1)
	return _dosix__dosexterr (&errorinfo);	/* TODO? better error handling */
    }
  *handle = fd;
  return 0;
}

static
void
_dosk86_open
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_OPEN);
  /* Documentation says:

     CL = attribute mask of files to look for (server call only)

     What should we do about it?  Now it’s simply ignored. */
  int handle;
  cpu->r.ax = _dosix__dos_open (_MK_FP (cpu->r.ds, cpu->r.dx),
				cpu->l.al,
				&handle);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
  cpu->r.ax = cpu->r.ax ? cpu->r.ax : handle;
}


/* _dos_close */

unsigned
_dosix__dos_close
(int handle)
{
  struct _DOSERROR errorinfo = {0};
  if (close (handle))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  return 0;
}

static
void
_dosk86_close
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_CLOSE);
  cpu->r.ax = _dosix__dos_close (cpu->r.bx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* _dos_getfileattr */

unsigned
_dosix__dos_getfileattr
(const char *path,
 unsigned *attrib)
{
  struct _DOSERROR errorinfo = {0};
  struct stat fs;
  if (stat (path, &fs))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  unsigned _attrib = 0;
  if (S_ISDIR (fs.st_mode))
    _attrib = _A_SUBDIR;
  else
    {
      _attrib = _A_NORMAL | _A_ARCH;
      if (access (path, W_OK))
	switch (errno)
	  {
	  case EROFS:
	  case EACCES:
	    _attrib |= _A_RDONLY;
	    break;
	  case ENOENT:
	  default:
	    return _dosix__dosexterr (&errorinfo);
	  }
    }
  *attrib = _attrib;
  return 0;
}

static
void
_dosk86_getfileattr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_METADATA);
  assert (cpu->l.al == INT21_AL_FILE_METADATA_GETFILEATTR);
  unsigned attrib;
  cpu->r.ax = _dosix__dos_getfileattr (_MK_FP (cpu->r.ds, cpu->r.dx),
				       &attrib);
  cpu->r.cx = attrib;
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* _dos_setfileattr */

unsigned
_dosix__dos_setfileattr
(const char *path,
 unsigned attrib)
{
  struct _DOSERROR errorinfo;
  struct stat fs;
  if (stat (path, &fs))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */

  mode_t mode = fs.st_mode;

  if (attrib & _A_RDONLY)
    mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
  else mode |= (S_IWUSR | S_IWGRP | S_IWOTH);

  if (chmod (path, mode))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */

  return 0;
}

static
void
_dosk86_setfileattr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_METADATA);
  assert (cpu->l.al == INT21_AL_FILE_METADATA_SETFILEATTR);
  cpu->r.ax = _dosix__dos_setfileattr (_MK_FP (cpu->r.ds, cpu->r.dx),
				       cpu->r.cx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* time conversion */

static
unsigned
__unixtime_struct
(const struct _dosdate_t *date,
 const struct _dostime_t *time,
 struct timeval *tv)
{
  assert (date), assert (time), assert (tv);
  struct _DOSERROR errorinfo = {0};
  struct tm tm =
    {
     .tm_sec = time->second,
     .tm_min = time->minute,
     .tm_hour = time->hour,
     .tm_mday = date->day,
     .tm_mon = date->month - 1,
     .tm_year = date->year - 1900,
     .tm_wday = date->dayofweek,
    };
  time_t utime = mktime (&tm);
  if (utime == (time_t) -1)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, EINVAL);
    }
  tv->tv_sec = utime;
  tv->tv_usec = time->hsecond * 10000;
  return 0;
}

static
unsigned
__dostime_struct
(const struct timeval *tv,
 struct _dosdate_t *date,
 struct _dostime_t *time)
{
  assert (tv);
  struct _DOSERROR errorinfo = {0};
  struct tm *tm = localtime (&tv->tv_sec);
  if (! tm)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (date)
    *date = (struct _dosdate_t)
      {
       .day = tm->tm_mday,
       .month = tm->tm_mon + 1,
       .year = 1900 + tm->tm_year,
       .dayofweek = tm->tm_wday
      };
  if (time)
    *time = (struct _dostime_t)
      {
       .hour = tm->tm_hour,
       .minute = tm->tm_min,
       .second = tm->tm_sec,
       .hsecond = tv->tv_usec / 10000
      };
  return 0;
}

/* Convert DOS date-time encoded integers to Unix time */
static
unsigned
__unixtime_int
(unsigned date,
 unsigned time,
 time_t *utime)
{
  struct tm tm =
    {
     .tm_sec = (time & ((1 << 5) - 1)) * 2,
     .tm_min = time >> 5 & ((1 << 6) - 1),
     .tm_hour = time >> 11,
     .tm_mday = date & ((1 << 5) - 1),
     .tm_mon = (date >> 5 & ((1 << 4) - 1)) - 1,
     .tm_year = (date >> 9) + (1980 - 1900),
     .tm_isdst = -1
    };
  time_t _utime = mktime (&tm);
  struct _DOSERROR errorinfo = {0};
  if (_utime == (time_t) (-1))
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (utime)
    *utime = _utime;
  return 0;
}

/* Convert Unix time to DOS date-time encoded integers */
static
unsigned
__dostime_int
(const time_t *utime,
 unsigned *date,
 unsigned *time)
{
  assert (utime);
  struct _DOSERROR errorinfo = {0};
  struct tm *tm = localtime (utime);
  if (! tm)
    {
      errorinfo.exterror = EXTERR_BAD_ARGUMENTS;
      errorinfo.errclass = ERRCLASS_APP_PROG_ERROR;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_UNKNOWN;
      return __doskexterr_set (&errorinfo, 0);
    }
  if (date)
    *date = tm->tm_year - (1980 - 1900) << 9
      | tm->tm_mon + 1 << 5
      | tm->tm_mday;
  if (time)
    *time = tm->tm_hour << 11
      | tm->tm_min << 5
      | tm->tm_sec / 2;
  return 0;
}


/* _dos_getftime */

unsigned
_dosix__dos_getftime
(int handle,
 unsigned *date,
 unsigned *time)
{
  struct _DOSERROR errorinfo = {0};
  struct stat fs;
  if (fstat (handle, &fs))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  unsigned err = __dostime_int (&fs.st_mtime,
				date,
				time);
  return err ? err: 0;
}

static
void
_dosk86_getftime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_TIME);
  assert (cpu->l.al == INT21_AL_FILE_TIME_GETFTIME);
  unsigned date, time;
  cpu->r.ax = _dosix__dos_getftime (cpu->r.bx,
				    &date,
				    &time);
  cpu->r.cx = time;
  cpu->r.dx = date;
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* _dos_setftime */

unsigned
_dosix__dos_setftime
(int handle,
 unsigned date,
 unsigned time)
{
  time_t _time;
  struct _DOSERROR errorinfo = {0};
  unsigned err = __unixtime_int (date, time, &_time);
  if (err) return err;
  struct stat fs;
  if (fstat (handle, &fs))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  struct timeval tvp[2] =
    {
     {
      .tv_sec = fs.st_atime,
      .tv_usec = 0
     },
     {
      .tv_sec = _time,
      .tv_usec = 0
     }
    };
  if (futimes (handle, tvp))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  return 0;
}

static
void
_dosk86_setftime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FILE_TIME);
  assert (cpu->l.al == INT21_AL_FILE_TIME_SETFTIME);
  cpu->r.ax = _dosix__dos_setftime (cpu->r.bx,
				    cpu->r.dx,
				    cpu->r.cx);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}


/* _dos_find functions */

static
unsigned
_dosk_findnext
(void)
{
  union dta_t *cdta = _dosk_current_dta;
  for (;
       cdta->find_t._gl_pathi < cdta->find_t._glob.gl_pathc;
       cdta->find_t._gl_pathi++)
    {
      char *filename = cdta->find_t._glob.gl_pathv[cdta->find_t._gl_pathi];
      unsigned attrib;
      if (_dosix__dos_getfileattr (filename, &attrib))
	continue; /* ignore files for which attributes can’t be queried */
      struct stat fs;
      if (stat (filename, &fs))
	continue;		/* ignore files that can’t be stat’ed */
      /* check if file matches the given attributes */
      if (cdta->find_t._attrib & attrib
	  || (! S_ISDIR (fs.st_mode)
	      && (cdta->find_t._attrib & (_A_HIDDEN | _A_RDONLY
					  | _A_SUBDIR | _A_SYSTEM)
		  || ! cdta->find_t._attrib)))
	{
	  cdta->find_t.attrib = attrib;
	  unsigned err = __dostime_int (&fs.st_mtime,
					&cdta->find_t.wr_date,
					&cdta->find_t.wr_time);
	  if (err) return err;
	  cdta->find_t.size = fs.st_size;
	  memccpy (cdta->find_t.name, filename, 0,
		   sizeof (cdta->find_t.name));	/* memcpy is safer than strcpy */

	  cdta->find_t._gl_pathi++;
	  return 0;
	}
    };

  assert (cdta->find_t._gl_pathi
	  == cdta->find_t._glob.gl_pathc);

  /* Notice that if the caller does’t consume all results the
     glob_t object will leak */
  globfree (&cdta->find_t._glob);
  errorinfo.exterror = EXTERR_NO_MORE_FILES;
  errorinfo.errclass = ERRCLASS_NOT_FOUND;
  errorinfo.action = ERRACT_IGNORE;
  errorinfo.locus = ERRLOCUS_BLOCK_DEV;
  return __doskexterr_set (&errorinfo, 0);
}

static
void
_dosk86_findnext
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FINDNEXT);
  cpu->r.ax = _dosk_findnext ();
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}

unsigned
_dosix__dos_findnext
(struct _find_t *fileinfo)
{
  union dta_t *prev_dta = _dosk_get_dta_addr ();
  _dosk_set_dta_addr ((union dta_t *) fileinfo);
  unsigned ret = _dosk_findnext ();
  _dosk_set_dta_addr (prev_dta);
  if (ret) errno = ENOENT;
  return ret;
}

static int
errfunc
(const char *filename,
 int error_code)
{
  struct _DOSERROR errorinfo = {0};
  switch (error_code)
    {
    case EACCES:
      errorinfo.exterror = EXTERR_ACCESS_DENIED;
      errorinfo.errclass = ERRCLASS_ACCESS_DENIED;
      errorinfo.action = ERRACT_ABORT_AFTER_CLEANUP;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      return __doskexterr_set (&errorinfo, 0);
      break;
    case ENOENT:
      errorinfo.exterror = EXTERR_PATH_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      return __doskexterr_set (&errorinfo, 0);
      break;
    case EIO:
      errorinfo.exterror = EXTERR_READ_FAULT;
      errorinfo.errclass = ERRCLASS_MEDIA_ERROR;
      errorinfo.action = ERRACT_RETRY_AFTER_USR_INTERV;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      return __doskexterr_set (&errorinfo, 0);
      break;
    default:		/* Get generic error handling based on libc */
      errno = error_code;
      return 1;
      break;
    }
}

static
unsigned
_dosk_findfirst
(char *filename,
 unsigned attrib,
 unsigned append_flag)
{
  struct _DOSERROR errorinfo = {0};
  union dta_t *cdta = _dosk_current_dta;
  glob_t *pglob = &cdta->find_t._glob;
  /* TODO: DOS expects ‘*.*’ to be interpreted as Unix’s ’*’ */
  /* TODO: DOS expects file-system to be case-insensitive */
  switch (glob (filename, GLOB_NOESCAPE | GLOB_NOSORT, &errfunc, pglob))
    {
    case 0:			/* success */
      break;
    case GLOB_ABORTED:
      globfree (pglob);
      return _dosix__dosexterr (&errorinfo);
    case GLOB_NOMATCH:
      globfree (pglob);
      errorinfo.exterror = EXTERR_FILE_NOT_FOUND;
      errorinfo.errclass = ERRCLASS_NOT_FOUND;
      errorinfo.action = ERRACT_PROMPT_USR_REENTER_INPUT;
      errorinfo.locus = ERRLOCUS_BLOCK_DEV;
      return __doskexterr_set (&errorinfo, 0);
    case GLOB_NOSPACE:
      globfree (pglob);
      errorinfo.exterror = EXTERR_INSUF_MEM;
      errorinfo.errclass = ERRCLASS_OUT_OF_RESOURCE;
      errorinfo.action = ERRACT_IMMEDIATE_ABORT;
      errorinfo.locus = ERRLOCUS_MEM_RELATED;
      return __doskexterr_set (&errorinfo, 0);
    default:			/* should never get here */
      assert (false);
    };
  assert (cdta->find_t._glob.gl_pathc > 0);
  cdta->find_t._gl_pathi = 0;
  cdta->find_t._filename = filename;
  cdta->find_t._attrib = attrib;
  return _dosk_findnext ();
}

static
void
_dosk86_findfirst
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_FINDFIRST);
  cpu->r.ax = _dosk_findfirst (_MK_FP (cpu->r.ds, cpu->r.dx),
			       cpu->r.cx,
			       cpu->l.al);
  cpu->r.flags = cpu->r.ax ? 1 : 0;
}

unsigned
_dosix__dos_findfirst
(const char *filename,
 unsigned attrib,
 struct _find_t *fileinfo)
{
  union dta_t *prev_dta = _dosk_get_dta_addr ();
  _dosk_set_dta_addr ((union dta_t *) fileinfo);
  unsigned ret = _dosk_findfirst ((char *) filename, attrib, 0);
  _dosk_set_dta_addr (prev_dta);
  if (ret) errno = ENOENT;
  return ret;
}

void
_dosix__dos_findclose
(struct _find_t *fileinfo)
{
  return;
}


/* _dos_getdate */

void
_dosix__dos_getdate
(struct _dosdate_t *date)
{
  assert (date);
  struct timeval tv;
  if (gettimeofday (&tv, NULL))
    return;			/* fail silently */
  __dostime_struct (&tv, date, NULL);
}

static
void
_dosk86_getdate
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETDATE);
  struct _dosdate_t date;
  _dosix__dos_getdate (&date);
  cpu->r.cx = date.year;
  cpu->h.dh = date.month;
  cpu->l.dl = date.day;
  cpu->l.al = date.dayofweek; /* DOS 1.10+ */
}


/* _dos_setdate */

unsigned
_dosix__dos_setdate
(struct _dosdate_t *date)
{
  assert (date);
  struct _DOSERROR errorinfo = {0};
  struct _dostime_t time;
  _dosix__dos_gettime (&time);
  struct timeval tv;
  unsigned err = __unixtime_struct (date, &time, &tv);
  if (err) return err;
  if (settimeofday (&tv, NULL))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  return 0;
}

static
void
_dosk86_setdate
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETDATE);
  struct _dosdate_t date =
    {
     .year = cpu->r.cx,
     .month = cpu->h.dh,
     .day = cpu->l.dl
    };
  /* AL = result
     00h successful
     FFh invalid time, system time unchanged */
  cpu->l.al = _dosix__dos_setdate (&date) ? 0xff : 0x00;
}


/* _dos_gettime */

void
_dosix__dos_gettime
(struct _dostime_t *time)
{
  assert (time);
  struct timeval tv;
  if (gettimeofday (&tv, NULL))
    return;			/* fail silently */
  __dostime_struct (&tv, NULL, time);
}

static
void
_dosk86_gettime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETTIME);
  struct _dostime_t time;
  _dosix__dos_gettime (&time);
  cpu->h.ch = time.hour;
  cpu->l.cl = time.minute;
  cpu->h.dh = time.second;
  cpu->l.dl = time.hsecond;
}


/* _dos_settime */

unsigned
_dosix__dos_settime
(struct _dostime_t *time)
{
  assert (time);
  struct _DOSERROR errorinfo = {0};
  struct _dosdate_t date;
  _dosix__dos_getdate (&date);
  struct timeval tv;
  unsigned err = __unixtime_struct (&date, time, &tv);
  if (err) return err;
  if (settimeofday (&tv, NULL))
    return _dosix__dosexterr (&errorinfo); /* TODO? better error handling */
  return 0;
}

static
void
_dosk86_settime
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETTIME);
  struct _dostime_t time =
    {
     .hour = cpu->h.ch,
     .minute = cpu->l.cl,
     .second = cpu->h.dh,
     .hsecond = cpu->l.dl
    };
  /* AL = result
     00h successful
     FFh invalid time, system time unchanged */
  cpu->l.al = _dosix__dos_settime (&time) ? 0xff : 0x00;
}


/* _dos_getvect */

syscall_t
_dos_getvect
(unsigned intnum)
{
  assert (intnum < UINT8_MAX);
  return _dosk_vect[intnum];
}

static
void
_dosk86_getvect
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETVECT);
  syscall_t syscall = _dos_getvect (cpu->l.al);
  cpu->r.es = _FP_SEG (syscall);
  cpu->r.bx = _FP_OFF (syscall);
}


/* _dos_setvect */

void
_dosix__dos_setvect
(unsigned intnum,
 syscall_t syscall)
{
  assert (syscall);
  assert (intnum < UINT8_MAX);
  _dosk_vect[intnum] = syscall;
}

static
void
_dosk86_setvect
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_SETVECT);
  _dosix__dos_setvect (cpu->l.al,
		       _MK_FP (cpu->r.ds, cpu->r.dx));
}


/* _dosk_write_stdout */

static
unsigned
_dosk_write_stdout
(const char *string)
{
  for (size_t i = 0; string[i] != '$'; i++)
    {
      int ret = putchar (string[i]);
      if (ret == EOF) break;
    }
  return '$';
}

static
void
_dosk86_write_stdout
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_WRITE_STDOUT);
  cpu->r.ax = _dosk_write_stdout (_MK_FP (cpu->r.ds,
					  cpu->r.dx));
}


/* libc-based kernel services */

static
void
_dosk86_getch
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETCH);
  cpu->l.al = _dosix__getch ();
}

static
void
_dosk86_getche
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_GETCHE);
  cpu->l.al = _dosix__getche ();
}

static
void
_dosk86_putch
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_PUTCH);
  cpu->l.al = _dosix__putch (cpu->l.dl);
}


/* call_syscall */

static
void
call_syscall
(uint8_t intnum,
 cpu_t *cpu,
 syscall_t syscall)
{
  if (! syscall)
    errx (86,
	  "Not implemented: "
	  "INT %02Xh "
	  "AX=%04Xh "
	  "BX=%04Xh "
	  "CX=%04Xh "
	  "DX=%04Xh ",
	  intnum,
	  cpu->r.ax,
	  cpu->r.bx,
	  cpu->r.cx,
	  cpu->r.dx);
  else
    syscall (cpu);
}


/* interrupt */

static
void
interrupt
(uint8_t intnum,
 cpu_t *cpu)
{
  assert (cpu);
  call_syscall (intnum,
		cpu,
		_dos_getvect(intnum));

}


/* intr */

void
_dosix_intr
(int intnum,
 union REGPACK *regs)
{
  cpu_t cpu = {0};
  if (regs)
    cpu = (cpu_t)
      {
       .r =
       {
	.ax = regs->x.ax,
	.bx = regs->x.bx,
	.cx = regs->x.cx,
	.dx = regs->x.dx,
	.bp = regs->x.bp,
	.si = regs->x.si,
	.di = regs->x.di,
	.ds = regs->x.ds,
	.es = regs->x.es,
	.flags = regs->x.flags
       }
      };
  interrupt (intnum, &cpu);
}


/* _int86x */

int
_dosix__int86x
(int intnum,
 union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  cpu_t cpu = {0};
  if (inregs)
    cpu = (cpu_t)
      {
       .r.ax = inregs->x.ax,
       .r.bx = inregs->x.bx,
       .r.cx = inregs->x.cx,
       .r.dx = inregs->x.dx,
       .r.si = inregs->x.si,
       .r.di = inregs->x.di,
       .r.flags = inregs->x.cflag
      };
  if (segregs)
    {
      cpu.r.cs = segregs->cs;
      cpu.r.ds = segregs->ds;
      cpu.r.es = segregs->es;
      cpu.r.ss = segregs->ss;
    };
  interrupt (intnum, &cpu);
  if (outregs)
    {
      *outregs = (_REGS)
	{
	 .x =
	 {
	  .ax = cpu.r.ax,
	  .bx = cpu.r.bx,
	  .cx = cpu.r.cx,
	  .dx = cpu.r.dx,
	  .si = cpu.r.si,
	  .di = cpu.r.di,
	  .cflag = cpu.r.flags
	 }
	};
      if (cpu.r.flags)
	_doserrno = cpu.r.ax;
    }
  if (segregs)
    *segregs = (_SREGS)
      {
       .es = cpu.r.es,
       .cs = cpu.r.cs,
       .ss = cpu.r.ss,
       .ds = cpu.r.ds
      };
  return cpu.r.ax;
}


/* _int86 */

int
_dosix__int86
(int intnum,
 union _REGS *inregs,
 union _REGS *outregs)
{
  return _dosix__int86x (intnum,
			 inregs,
			 outregs,
			 NULL);
}


/* _intdosx */

int
_dosix__intdosx
(union _REGS *inregs,
 union _REGS *outregs,
 struct _SREGS *segregs)
{
  return _dosix__int86x (INT21_MAIN_DOS_API,
			 inregs,
			 outregs,
			 segregs);
}


/* _intdos */

int
_dosix__intdos
(union _REGS *inregs,
 union _REGS *outregs)
{
  return _dosix__intdosx (inregs,
			  outregs,
			  NULL);
}


/* _bdos */

int
_dosix__bdos
(int dosfunc,
 unsigned int dosdx,
 unsigned int dosal)
{
  union _REGS inregs;
  union _REGS outregs;
  inregs.h.ah = dosfunc;
  inregs.x.dx = dosdx;
  inregs.h.al = dosal;
  return _dosix__intdos (&inregs,
			 &outregs);
}


/* _dosk_int21_main_dos_api */

static
void
_dosk_int21_main_dos_api
(cpu_t *cpu)
{
  assert (cpu);
  syscall_t syscall = NULL;
  switch (cpu->h.ah) /* AH */
    {
    case INT21_AH_FILE_METADATA: /* 0x43 */
      switch (cpu->l.al) /* AL */
	{
	case INT21_AL_FILE_METADATA_GETFILEATTR: /* 0x00 */
	  syscall = _dosk86_getfileattr;
	  break;
	case INT21_AL_FILE_METADATA_SETFILEATTR: /* 0x01 */
	  syscall = _dosk86_setfileattr;
	  break;
	}
      break;
    case INT21_AH_FILE_TIME: /* 0x57 */
      switch (cpu->l.al) /* AL */
	{
	case INT21_AL_FILE_TIME_GETFTIME: /* 0x00 */
	  syscall = _dosk86_getftime;
	  break;
	case INT21_AL_FILE_TIME_SETFTIME: /* 0x01 */
	  syscall = _dosk86_setftime;
	  break;
	}
      break;
    case INT21_AH_GETCHE: /* 0x01 */
      syscall = _dosk86_getche;
      break;
    case INT21_AH_PUTCH: /* 0x02 */
      syscall = _dosk86_putch;
      break;
    case INT21_AH_GETCH: /* 0x08 */
      syscall = _dosk86_getch;
      break;
    case INT21_AH_WRITE_STDOUT: /* 0x09 */
      syscall = _dosk86_write_stdout;
      break;
    case INT21_AH_SET_DTA_ADDR: /* 0x1a */
      syscall = _dosk86_set_dta_addr;
      break;
    case INT21_AH_SETVECT: /* 0x25 */
      syscall = _dosk86_setvect;
      break;
    case INT21_AH_GETDATE: /* 0x2a */
      syscall = _dosk86_getdate;
      break;
    case INT21_AH_SETDATE: /* 0x2b */
      syscall = _dosk86_setdate;
      break;
    case INT21_AH_GETTIME: /* 0x2c */
      syscall = _dosk86_gettime;
      break;
    case INT21_AH_SETTIME: /* 0x2d */
      syscall = _dosk86_settime;
      break;
    case INT21_AH_GET_DTA_ADDR: /* 0x2f */
      syscall = _dosk86_get_dta_addr;
      break;
    case INT21_AH_GETVECT: /* 0x35 */
      syscall = _dosk86_getvect;
      break;
    case INT21_AH_CREAT: /* 0x3c */
      syscall = _dosk86_creat;
      break;
    case INT21_AH_OPEN:	/* 0x3d */
      syscall = _dosk86_open;
      break;
    case INT21_AH_CLOSE: /* 0x3e */
      syscall = _dosk86_close;
      break;
    case INT21_AH_ALLOCMEM: /* 0x48 */
      syscall = _dosk86_allocmem;
      break;
    case INT21_AH_FREEMEM: /* 0x49 */
      syscall = _dosk86_freemem;
      break;
    case INT21_AH_SETBLOCK: /* 0x4a */
      syscall = _dosk86_setblock;
      break;
    case INT21_AH_FINDFIRST: /* 0x4e */
      syscall = _dosk86_findfirst;
      break;
    case INT21_AH_FINDNEXT: /* 0x4f */
      syscall = _dosk86_findnext;
      break;
    case INT21_AH_EXTERR: /* 0x59 */
      switch (cpu->h.bh) /* BH */
	{
	case INT21_BH_EXTERR: /* 0x00 */
	  switch (cpu->l.bl) /* BL */
	    {
	    case INT21_BL_EXTERR: /* 0x00 */
	      syscall = _dosk86exterr;
	      break;
	    }
	  break;
	}
      break;
    case INT21_AH_CREATNEW: /* 0x5b */
      syscall = _dosk86_creatnew;
      break;
    }
  call_syscall (INT21_MAIN_DOS_API,
		cpu,
		syscall);
}


/* _dosk_int2f_multiplex */

static
void
_dosk_int2f_multiplex
(cpu_t *cpu)
{
  assert (cpu);
  syscall_t syscall = NULL;
  switch (cpu->h.ah) /* AH */
    {
    case INT2F_AH_DOS_INTERNAL: /* 0x12 */
      switch (cpu->l.al) /* AL */
	{
	case INT2F_AL_DOS_INTERNAL_EXTERR_SET: /* 0x22 */
	  syscall = __dosk86exterr_set;
	  break;
	}
      break;
    };
  call_syscall (INT2F_MULTIPLEX,
		cpu,
		syscall);
}
