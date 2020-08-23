/*
  _dosexterr.h -- Get extended error information (DOS 3.0+)

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

#ifndef _INC__DOSEXTERR
#define _INC__DOSEXTERR

#include "cpu.h"
#include "include/dos.h"

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

struct media_id			/* media ID structure */
{
  char volume_label[12];     /* ASCIZ volume label of required disk */
  unsigned serial_number;    /* serial number (DOS 4.0+) */
};

int
_dosexterr
(struct _DOSERROR *_errorinfo);

void
_dosk86exterr
(cpu_t *cpu);

void
__doskexterr_set
(struct _DOSERROR *new_errorinfo);

void
__dosk86exterr_set
(cpu_t *cpu);

#endif
