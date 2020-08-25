/*
  _dosexterr.c -- Get extended error information (DOS 3.0+)

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

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include "_dosexterr.h"
#include "INT.h"
#include "include/dos.h"
#include "include/_stdlib.h"

static struct _DOSERROR errorinfo;
static struct media_id media_id;

int
_dosexterr
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

void
_dosk86exterr
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT21_AH_EXTERR);
  assert (cpu->h.bh == INT21_BH_EXTERR);
  assert (cpu->l.bl == INT21_BL_EXTERR);
  struct _DOSERROR errorinfo = {0};
  _dosexterr (&errorinfo);
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

void
__dosk86exterr_set
(cpu_t *cpu)
{
  assert (cpu);
  assert (cpu->h.ah == INT2F_AH_DOS_INTERNAL);
  assert (cpu->l.al == INT2F_AL_DOS_INTERNAL_EXTERR_SET);
  __doskexterr_set (_MK_FP (cpu->r.ss, cpu->r.si), 0);
}
